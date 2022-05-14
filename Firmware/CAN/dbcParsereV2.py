# -*- coding: utf-8 -*-
"""
Created on Sun Nov 13 18:12:47 2016

@author: kid group
"""

import json
import os

directory = "./generated"

if not os.path.exists(directory):
    os.makedirs(directory)
    

try:
    with open('dbc.json') as data_file:    
        data = json.load(data_file)
except:
    print("dbc json error found, press enter to quit.")
    s = input();
    quit();
print("CAN DBC file successfully found.\n")



numberOfNodes = len(data["NODE"])
for node in range(0,numberOfNodes):
    send_message_dict = {}
    print("Generating dbc.h and dbc.c for " + str(data["NODE"][node]["name"]))
    thisNode = node
    hfile = "generated/" + str(data["NODE"][node]["name"]).lower() + "_dbc.h"
    cfile = "generated/" + str(data["NODE"][node]["name"]).lower() + "_dbc.c"
    dot_h = open(hfile,'w')
    dot_c = open(cfile,'w')

    #Headers and Includes
    dot_h.write("#ifndef "+str(data["NODE"][node]["name"]) +"_DBC_H\n")
    dot_h.write("#define "+str(data["NODE"][node]["name"]) +"_DBC_H\n\n")
    dot_h.write("#include <stdint.h>\n")
    dot_h.write("#include \"bolt_CAN.h\"\n")
    dot_c.write("#include \""+str(data["NODE"][node]["name"]) + "_dbc.h\"\n")
    dot_c.write("#include \"utils.h\"\n")

    #find number of CAN nodes and create enum
    numberOfNodes = len(data["NODE"])
    dot_h.write("typedef enum{\n")
    for i in range(0,numberOfNodes):
        dot_h.write("    " + str(data["NODE"][i]["name"]) + ",\n")
    dot_h.write("} CAN_nodes_E;\n\n")

    #loop through each Node and Parse each message
    for i in range(0,numberOfNodes):
        this_node_name = data["NODE"][i]["name"]
        #create a comment header for each Node
        dot_h.write("/**********************************************************\n")
        dot_h.write(" * " + this_node_name + " NODE MESSAGES\n */\n")
        dot_c.write("/**********************************************************\n")
        dot_c.write(" * " + this_node_name + " NODE MESSAGES\n */\n")
        numberOfMessages = len(data["NODE"][i]["messages"])
        print("Node " + this_node_name + " has " + str(numberOfMessages) + " messages")

        #loop through each message for a given Node.
        for j in range(0, numberOfMessages):
            this_message_name = data["NODE"][i]["messages"][j]["name"]
            if i == thisNode:
                pass
            # add messages if node is a consumer, or if the consumer field doesn't exist.
            elif not data["NODE"][i]["messages"][j].get("consumers"):
                pass
            elif data["NODE"][thisNode]["name"] in data["NODE"][i]["messages"][j].get("consumers"):
                pass
            else:
                continue

            #define the CAN ID
            ID_name = "CAN_" + this_node_name + "_" + this_message_name
            if data["NODE"][i]["messages"][j]["id"] is None:
                canID = (0b111 << 8) | (i << 4) | (j+1)
            else:
                canID = int(data["NODE"][i]["messages"][j]["id"])
            dot_c.write("#define " + ID_name + "_ID " + hex(canID) + "\n\n")
            if data["NODE"][i]["messages"][j].get("x_id"):
                canXID = data["NODE"][i]["messages"][j].get("x_id")
            else:
                canXID = 0

            print("\t" + this_message_name)
            payload_init = ".payload = 0"
            if i == thisNode:
                dot_c.write("static CAN_payload_S " + ID_name + "_payload __attribute__((aligned(sizeof(CAN_payload_S))));\n")
                payload_init = ".payload = &" + ID_name + "_payload"

            dot_c.write("static CAN_message_S " + ID_name + "={\n")
            dot_c.write("\t.canID = " + ID_name + "_ID" + ",\n\t.canXID = " + hex(canXID) + ",\n\t" + payload_init
                        + ",\n\t.canMessageStatus = 0\n};\n\n")
            if i != thisNode:
                dot_c.write("uint8_t " + ID_name + "_checkDataIsFresh(void){\n\treturn CAN_checkDataIsFresh(&" + ID_name + ");\n}\n")


            #loop through each signal and parse the data
            numberOfSignals = len(data["NODE"][i]["messages"][j]["signals"])
            offset = 0
            for k in range(0,numberOfSignals):
                dot_c.write(
                    "#define " + ID_name.upper() + "_" + str(data["NODE"][i]["messages"][j]["signals"][k]["name"].upper() + "_RANGE ")
                    + str(data["NODE"][i]["messages"][j]["signals"][k]["length"]) + "\n")
                dot_c.write(
                    "#define " + ID_name.upper() + "_" + str(data["NODE"][i]["messages"][j]["signals"][k]["name"].upper() + "_OFFSET ")
                    + str(offset) + "\n")
                offset += int(data["NODE"][i]["messages"][j]["signals"][k]["length"])
                print("\t\t" + data["NODE"][i]["messages"][j]["signals"][k]["name"])
            dot_c.write("\n")

            if(i == thisNode):
                #loop through signals again and create populate functions
                for k in range(0, numberOfSignals):
                    this_signal_name = data["NODE"][i]["messages"][j]["signals"][k]["name"]
                    dot_h.write("void CAN_" + this_node_name + "_"
                        + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                        + this_signal_name
                        + "_set(uint16_t " + this_signal_name + ");\n")
                    dot_c.write("void CAN_" + this_node_name + "_"
                        + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                        + this_signal_name
                        + "_set(uint16_t " + this_signal_name + "){\n")
                    dot_c.write("\tset_bits((size_t*)CAN_" + this_node_name + "_" + this_message_name + ".payload, "
                                + ID_name.upper() + "_" + this_signal_name.upper() + "_OFFSET, "
                                + ID_name.upper() + "_" + this_signal_name.upper() + "_RANGE, "
                                + this_signal_name + ");\n}\n")
                this_message_freq = data["NODE"][i]["messages"][j].get("freq")
                if this_message_freq:
                    if not send_message_dict.get(str(this_message_freq)):
                        send_message_dict[str(this_message_freq)] = []
                    send_message_dict[str(this_message_freq)].append("CAN_" + this_node_name + "_" + this_message_name + "_send")
            else:
                #loop through signals again and create retrieve functions
                dot_h.write("uint8_t " + ID_name + "_checkDataIsFresh(void);\n")
                for k in range(0,numberOfSignals):
                    dot_h.write("uint16_t CAN_" + str(data["NODE"][i]["name"]) + "_"
                        + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                        + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                        + "_get(void);\n")
                    dot_c.write("uint16_t CAN_" + str(data["NODE"][i]["name"]) + "_"
                        + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                        + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                        + "_get(void){\n")
                    dot_c.write("\treturn get_bits((size_t*)CAN_" + str(data["NODE"][i]["name"]) + "_"
                                + str(data["NODE"][i]["messages"][j]["name"]) + ".payload, "
                                + ID_name.upper() + "_" + str(data["NODE"][i]["messages"][j]["signals"][k]["name"].upper()) + "_OFFSET, "
                                + ID_name.upper() + "_" + str(data["NODE"][i]["messages"][j]["signals"][k]["name"].upper()) + "_RANGE);\n}\n")

                dot_h.write("\n")
                dot_c.write("\n")
            if(i == thisNode):
                #finally, write the send fucntion that is called to send the meesage to the CANbus
                dot_h.write("void CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_send(void);\n\n\n")
                dot_c.write("void CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_send(void){\n")
                dot_c.write("\tCAN_write(CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ");\n}\n\n")

    #write the initializer function and close header guards
    dot_h.write("void CAN_DBC_init();\n\n")

    dot_c.write("void CAN_DBC_init(void) {\n")
    for i in range(0, numberOfNodes):
        if i == thisNode:
            continue
        else:
            numberOfMessages = len(data["NODE"][i]["messages"])
            for j in range(0, numberOfMessages):
                # add messages if node is a consumer, or if the consumer field doesn't exist.
                if not data["NODE"][i]["messages"][j].get("consumers"):
                    pass
                elif data["NODE"][thisNode]["name"] in data["NODE"][i]["messages"][j].get("consumers"):
                    pass
                else:
                    continue
                dot_c.write("\tCAN_configureMailbox(&CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ");\n")
    dot_c.write("}\n")
    for send_message_freq in send_message_dict:
        dot_h.write("void CAN_send_" + send_message_freq + "ms(void);\n")
        dot_c.write("\nvoid CAN_send_" + send_message_freq + "ms(void){\n")
        for message in send_message_dict[send_message_freq]:
            dot_c.write("\t{}();\n".format(message))
        dot_c.write("}\n")
    dot_h.write("\n\n#endif /*" + str(data["NODE"][node]["name"]) + "_DBC_H*/\n")
    dot_h.close
    dot_c.close

s = input("DBC files succefully generated! press enter to quit")
quit()