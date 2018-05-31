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

try:
    numberOfNodes = len(data["NODE"])
    for node in range(0,numberOfNodes):
        print("Generating dbc.h and dbc.c for " + str(data["NODE"][node]["name"]))
        thisNode = node
        hfile = "generated/" + str(data["NODE"][node]["name"]) + "_dbc.h"
        cfile = "generated/" + str(data["NODE"][node]["name"]) + "_dbc.c"
        dot_h = open(hfile,'w')
        dot_c = open(cfile,'w')
        
        #Headers and Includes
        dot_h.write("#ifndef "+str(data["NODE"][node]["name"]) +"_DBC_H\n")
        dot_h.write("#define "+str(data["NODE"][node]["name"]) +"_DBC_H\n\n")
        dot_h.write("#include <stdint.h>\n")
        dot_h.write("#include \"bolt_CAN.h\"\n\n")
        dot_c.write("#include \""+str(data["NODE"][node]["name"]) + "_dbc.h\"\n\n")

        dot_c.write("/*Private struct for keeping TX message data within message struct*/\n")
        dot_c.write("typedef struct {\n\tCAN_message_S txM;\n\tCAN_payload_S txP;\n} CAN_message_TX_S;")

        #find number of CAN nodes and create enum
        numberOfNodes = len(data["NODE"])
        dot_h.write("typedef enum{\n")
        for i in range(0,numberOfNodes):
            dot_h.write("    " + str(data["NODE"][i]["name"]) + ",\n")
        dot_h.write("} CAN_nodes_E;")



        #loop through each Node and Parse each message
        for i in range(0,numberOfNodes):
            #create a comment header for each Node
            dot_h.write("\n\n\n/**********************************************************\n")
            dot_h.write(" * " + str(data["NODE"][i]["name"]) + " NODE MESSAGES\n */\n")
            dot_c.write("\n\n\n/**********************************************************\n")
            dot_c.write(" * " + str(data["NODE"][i]["name"]) + " NODE MESSAGES\n */\n")
            numberOfMessages = len(data["NODE"][i]["messages"])
            print("Node " + data["NODE"][i]["name"] + " has " + str(numberOfMessages) + " messages")
            
            #loop through each message for a given Node.
            for j in range(0,numberOfMessages):
                
                #define the CAN ID
                dot_h.write("#define CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "_ID")
                if(data["NODE"][i]["messages"][j]["id"] == "NA"):
                    canID = (0b111 << 8) | (i << 4) | (j+1)   
                else:
                    canID =  int(data["NODE"][i]["messages"][j]["id"])
                dot_h.write(" " + hex(canID) + "\n")
                #create a struct for the signals in each message      
                dot_h.write("typedef struct{\n")
                print("\t" + data["NODE"][i]["messages"][j]["name"])
                dot_c.write("static CAN_message_TX_S CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "={\n")
                dot_c.write("\t.txM = \n\t{\n\t\t.canID = " + str(canID) + ",\n\t\t.payload = 0\n\t},\n\t.txP =\n\t{\n\t\t.word0 = 0,\n\t\t.word1 = 0,\n\t\t.word2 = 0,\n\t\t.word3 = 0\n\t}\n};\n\n")
                dot_c.write("typedef union {\n\tCAN_payload_S packedMessage;\n")
                dot_c.write("\tCAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "_S signals;\n")
                dot_c.write("} CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "_U;\n\n")
                dot_c.write("static CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "_U * CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "_P;\n\n")
                
                #loop through each signal and parse the data
                numberOfSignals = len(data["NODE"][i]["messages"][j]["signals"])
                for k in range(0,numberOfSignals):
                    dot_h.write("\tuint16_t " + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                        + ": " + str(data["NODE"][i]["messages"][j]["signals"][k]["length"]) + ";\n")
                    print("\t\t" + data["NODE"][i]["messages"][j]["signals"][k]["name"])
                dot_h.write("} CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "_S;\n\n")
                
                if(i == thisNode):
                    #loop through signals again and create populate functions        
                    for k in range(0,numberOfSignals):
                        dot_h.write("void CAN_" + str(data["NODE"][i]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                            + "_set(uint16_t " + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])+ ");\n")
                        dot_c.write("void CAN_" + str(data["NODE"][i]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                            + "_set(uint16_t " + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])+ "){\n")
                        dot_c.write("\tCAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_P->signals." + str(data["NODE"][i]["messages"][j]["signals"][k]["name"]) + " = " + str(data["NODE"][i]["messages"][j]["signals"][k]["name"]) + ";\n}\n")
                    dot_h.write("\n")
                    dot_c.write("\n")
                else:
                    #loop through signals again and create retrieve functions        
                    for k in range(0,numberOfSignals):
                        dot_h.write("uint16_t CAN_" + str(data["NODE"][i]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                            + "_get(void);\n")
                        dot_c.write("uint16_t CAN_" + str(data["NODE"][i]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                            + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                            + "_get(void){\n")
                        dot_c.write("\treturn CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_P->signals." + str(data["NODE"][i]["messages"][j]["signals"][k]["name"]) + ";\n}\n")
                    dot_h.write("\n")
                    dot_c.write("\n")
                if(i == thisNode):
                    #finally, write the send fucntion that is called to send the meesage to the CANbus
                    dot_h.write("void CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_send(void);\n\n\n")
                    dot_c.write("void CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_send(void){\n")
                    dot_c.write("\tCAN_write(CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ".txM);\n}\n\n")

        #write the initializer function and close header guards
        dot_h.write("void CAN_DBC_init();\n\n")
        dot_h.write("\n\n#endif /*"+str(data["NODE"][node]["name"]) +"_DBC_H*/\n")
        dot_c.write("void CAN_DBC_init() {\n")
        for i in range(0, numberOfNodes):
            if(i ==  thisNode):
                numberOfMessages = len(data["NODE"][i]["messages"])
                for j in range(0, numberOfMessages):
                    dot_c.write("\tCAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ".txM.payload = &CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ".txP;\n")
                    dot_c.write("\tCAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_P = (CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_U*) & CAN_"  + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ".txP;\n\n")
            else:
                numberOfMessages = len(data["NODE"][i]["messages"])
                for j in range(0, numberOfMessages):
                    dot_c.write("\tCAN_configureMailbox(&CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ".txM);\n")
                    dot_c.write("\tCAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_P = (CAN_" + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + "_U*) CAN_"  + str(data["NODE"][i]["name"]) + "_" + str(data["NODE"][i]["messages"][j]["name"]) + ".txM.payload;\n\n")
        dot_c.write("}\n")
        dot_h.close
        dot_c.close
except:
    s = input("error durring parsing, press enter to quit")
    quit()
s = input("DBC files succefully generated! press enter to quit")
quit()