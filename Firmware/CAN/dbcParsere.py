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
    
f = open("generated/dbc.h",'w')

    

with open('testing.json') as data_file:    
    data = json.load(data_file)
    
#Header Guards
f.write("#ifndef DBC_H\n")
f.write("#define DBC_H\n\n")
f.write("#include <stdint.h>\n\n")

f.write("typedef struct {\n")
f.write("   uint16_t word0;\n")
f.write("   uint16_t word1;\n")
f.write("   uint16_t word2;\n")
f.write("   uint16_t word3;\n} words;\n\n")

#find number of CAN nodes and create enum
numberOfNodes = len(data["NODE"])
f.write("typedef enum{\n")
for i in range(0,numberOfNodes):
    f.write("    " + str(data["NODE"][i]["name"]) + ",\n")
f.write("} CAN_nodes_E;")

#loop through each Node and Parse each message
for i in range(0,numberOfNodes):
    #create a comment header for each Node
    f.write("\n\n\n/**********************************************************\n")
    f.write(" * " + str(data["NODE"][i]["name"]) + " node stuff\n */\n")
    numberOfMessages = len(data["NODE"][i]["messages"])
    print("Node " + data["NODE"][i]["name"] + " has " + str(numberOfMessages) + " messages")
    
    #loop through each message for a given Node.
    for j in range(0,numberOfMessages):
        
        #define the CAN ID
        f.write("#define CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"])
        canID = (0b111 << 8) | (i << 4) | j   
        f.write(" " + hex(canID) + "\n")
        #create a union with 4 16-bit words        
        f.write("typedef union{\n")
        f.write("    words packedMessage;\n")
        print("    " + data["NODE"][i]["messages"][j]["name"])
        
        #loop through each signal and parse the data
        numberOfSignals = len(data["NODE"][i]["messages"][j]["signals"])
        for k in range(0,numberOfSignals):
            f.write("    uint16_t " + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                + ": " + str(data["NODE"][i]["messages"][j]["signals"][k]["length"]) + ";\n")
            print("        " + data["NODE"][i]["messages"][j]["signals"][k]["name"])
        f.write("} CAN_" + str(data["NODE"][i]["name"]) + "_" + data["NODE"][i]["messages"][j]["name"] + "_U;\n\n")
        
        #loop through signals again and create populate functions        
        for k in range(0,numberOfSignals):
            f.write("void CAN_" + str(data["NODE"][i]["name"]) + "_"
                + str(data["NODE"][i]["messages"][j]["name"]) + "_"
                + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])
                + "(uint16_t* mailbox, uint16_t " + str(data["NODE"][i]["messages"][j]["signals"][k]["name"])+ ");\n\n")
            
            
f.write("\n\n#endif /*DBC_H*/\n")
    
f.close