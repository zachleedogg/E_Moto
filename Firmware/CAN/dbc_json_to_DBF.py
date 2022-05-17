import json
import easygui

file = 'C:\\Repos\\E_Moto\\Firmware\\CAN\\dbc.json' #easygui.fileopenbox("select json dbc file", default='C:\\Repos\\E_Moto\\Firmware\\CAN\\dbc.json')

dbc_dict = json.load(open(file))

print("//******************************BUSMASTER Messages and signals Database ******************************//\n")
print("[DATABASE_VERSION] 1.3\n")
print("[PROTOCOL] CAN\n")
print("[BUSMASTER_VERSION] [3.2.2]\n")
num_mess = 0
for node in dbc_dict["NODE"]:
    for message in node['messages']:
        num_mess += 1
print("[NUMBER_OF_MESSAGES] {}\n".format(num_mess))

base_address = 0x1

for node in dbc_dict["NODE"]:

    for message in node['messages']:
        id = message["id"]
        if id is None:
            id = base_address
            base_address +=1
        print("[START_MSG] {}_{},{},{},{},{},{}".format(node['name'], message['name'], id, "8", len(message['signals']),1, "X" if message.get("x_id") else "S"))
        offset = 0
        for signal in message['signals']:
            print("[START_SIGNALS] {},{},{},{},{},{},{},{},{},{},{},{}".format(signal['name'],signal['length'],str(int(offset/8) + 1),str(offset%8),"U",2**int(signal['length'])-1,0,1,0.000000,1.00000,"",""))
            offset += int(signal['length'])
        print("[END_MSG]\n")
