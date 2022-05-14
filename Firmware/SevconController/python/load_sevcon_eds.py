import canopen
import logging



logging.basicConfig(level=logging.DEBUG)

# Start with creating a network representing one CAN bus
network = canopen.Network()

# Add some nodes with corresponding Object Dictionaries
node = canopen.RemoteNode(1, '/path/to/object_dictionary.eds')
network.add_node(node)


# Connect to the CAN bus
# Arguments are passed to python-can's can.Bus() constructor
# (see https://python-can.readthedocs.io/en/latest/bus.html).
BAUD = 500000
# network.connect(bustype='socketcan', channel='can0')
# network.connect(bustype='kvaser', channel=0, bitrate=BAUD)
network.connect(bustype='pcan', channel='PCAN_USBBUS1', bitrate=BAUD)
# network.connect(bustype='ixxat', channel=0, bitrate=BAUD)
# network.connect(bustype='vector', app_name='CANalyzer', channel=0, bitrate=BAUD)
# network.connect(bustype='nican', channel='CAN0', bitrate=BAUD)

# Read a variable using SDO
device_name = node.sdo['Manufacturer device name'].raw
vendor_id = node.sdo[0x1018][1].raw

# # Read PDO configuration from node
# node.tpdo.read()
# node.rpdo.read()
# # Re-map TPDO[1]
# node.tpdo[1].clear()
# node.tpdo[1].add_variable('Statusword')
# node.tpdo[1].add_variable('Velocity actual value')
# node.tpdo[1].add_variable('Some group', 'Some subindex')
# node.tpdo[1].trans_type = 254
# node.tpdo[1].event_timer = 10
# node.tpdo[1].enabled = True
# # Save new PDO configuration to node
# node.tpdo[1].save()

print("connected!!!")

# Transmit SYNC every 100 ms
network.sync.start(0.1)

# Change state to operational (NMT start)
node.nmt.state = 'OPERATIONAL'

# Read a value from TPDO[1]
node.tpdo[1].wait_for_reception()
speed = node.tpdo[1]['Velocity actual value'].phys
val = node.tpdo['Some group.Some subindex'].raw

# Disconnect from CAN bus
network.sync.stop()
network.disconnect()