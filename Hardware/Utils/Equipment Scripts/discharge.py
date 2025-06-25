from pyvisa import ResourceManager
from time import sleep, perf_counter
from statistics import mode

SIX_PACK_VOLTAGE = 4.1

EIGHT_BANGER_BANK_VOLTAGE = 3.8

def main():
    rm = ResourceManager("C:/Windows/System32/visa32.dll")
    print(rm.list_resources())
    if 'USB0::0x0B3E::0x1006::NF002784::INSTR' in rm.list_resources():
        load = rm.open_resource('USB0::0x0B3E::0x1006::NF002784::INSTR')
    else:
        print("failed to connect")
        return

    print(load.query('*IDN?'))
    print(load.query('MEAS:VOLT?'))
    print(load.query('MEAS:CURR?'))

    load.write('CURR:AMPL 20')


    load.write('VOLT:PROT:UND 3.0')
    load.query('VOLT:PROT:UND?')

    load.write('VOLT:PROT:STAT ON')

    load.write('OUTP:STAT ON')

    while(1):
        load.write('OUTP:STAT ON')
        sleep(5)
        load.write('OUTP:STAT OFF')
        now = perf_counter()
        v_list = []
        while(now + 2 > perf_counter()):
            v = float(load.query('MEAS:VOLT?'))
            v_list.append(v)
            print(v)
            sleep(0.01)
        m = mode(v_list)
        print(m)
        if m <= EIGHT_BANGER_BANK_VOLTAGE:
            break

    load.write('VOLT:PROT:STAT OFF')
    print(load.query('*IDN?'))

    load.close()


if __name__ == "__main__":
    main()