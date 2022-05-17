import canopen

# Start with creating a network representing one CAN bus
network = canopen.Network()

# Add some nodes with corresponding Object Dictionaries
node = canopen.RemoteNode(1, 'C:\Repos\E_Moto\Firmware\SevconController\AC24ls.eds')
network.add_node(node)

# Connect to the CAN bus
# Arguments are passed to python-can's can.Bus() constructor
# (see https://python-can.readthedocs.io/en/latest/bus.html).
#network.connect()
# network.connect(bustype='socketcan', channel='can0')
# network.connect(bustype='kvaser', channel=0, bitrate=250000)
network.connect(bustype='pcan', channel='PCAN_USBBUS1', bitrate=500000)
# network.connect(bustype='ixxat', channel=0, bitrate=250000)
# network.connect(bustype='vector', app_name='CANalyzer', channel=0, bitrate=250000)
# network.connect(bustype='nican', channel='CAN0', bitrate=250000)

# Read a variable using SDO
name = node.sdo['Product name'].raw
print(name)

node.sdo["Password Entry"][3].raw = 0
node.sdo["Password Entry"][2].raw = 0x4bdf

access_level = node.sdo["Password Entry"][1].raw

pw = node.sdo["Password Key"].raw

print(access_level)
