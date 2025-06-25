import os

schematic = "../DCDC_Controller.sch"

oldFile = open(schematic, 'r')
newFile = open("../temp.sch",'w')

thisLocationX = "0"
thisLocationY = "0"
newline = ""

for line in oldFile:
    if line.startswith("P"):
        line2 = line.strip('\n')
        words = line2.split(" ")
        thisLocationX = words[1]
        thisLocationY = words[2]
        newline = line
    elif line.startswith("F 4"):
        newline = line[:10] + str(thisLocationX) + " " + str(thisLocationY) + str(line[17:])
    else:
        newline = line
    
    
    
    newFile.write(newline)
os.remove("schematic)
os.rename("../temp.sch", schematic)
        
