# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 22:27:37 2016

@author: kid group
"""
import serial
from serial import SerialException
from serial.tools.list_ports import comports
import sys

from time import sleep
import threading
try:
    import queue
except ImportError:
    import Queue as queue


class SerialCom_Thread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.paused = False
        # Explicitly using Lock over RLock since the use of self.paused
        # break reentrancy anyway, and I believe using Lock could allow
        # one thread to pause the worker, while another resumes; haven't
        # checked if Condition imposes additional limitations that would 
        # prevent that. In Python 2, use of Lock instead of RLock also
        # boosts performance.
        #self.pause_cond = threading.Condition(threading.Lock())
        self.serQueue = queue.Queue()
        self.lock = threading.Lock()
        self.s = serial.Serial(bytesize=serial.EIGHTBITS, timeout=0.5)
        self.pause()

    def run(self):
        while True:
            if self.paused is False:
                try:
                    if self.s.inWaiting():
                        self.lock.acquire()
                        text = self.s.read(self.s.inWaiting())
                        self.serQueue.put_nowait(text)
                        self.lock.release()
                except serial.SerialException:
                    pass

    def pause(self):
        if self.paused is False:
            self.paused = True
            # If in sleep, we acquire immediately, otherwise we wait for thread
            # to release condition. In race, worker will still see self.paused
            # and begin waiting until it's set back to False
            #self.pause_cond.acquire()

    #should just resume the thread
    def resume(self):
        if self.paused is True:
            self.paused = False
            # Notify so thread will wake after lock released
            #self.pause_cond.notify()
            # Now release the lock
            #self.pause_cond.release()
        
    def closePort(self):
        if self.s.isOpen():
            self.s.__del__()
            self.pause()
        else:
            pass
            
    def openPort(self, com, baud):
        if self.s.isOpen():
            pass
        else:
            self.s.port = com
            self.s.baudrate = baud
            self.s.open()
        self.s.reset_input_buffer()
        self.resume()
        return self.s.getSettingsDict();
            
    def getPort(self):
        result = list(comports())
        comList = []
        for p in range(0,len(result)):
            comList.append(result[p][0])
        print(comList)
        return comList
    """
        if sys.platform.startswith('win'):
            ports = []
            for i in range(256):
                ports.append('COM%s' % (i + 1))
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')
        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.__del__()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
    """
        
        
    def writeToCom(self, anystring):
        sent = self.s.write(anystring)
        print("just sent " + str(sent) + "bytes")
        
    def readFromCom(self):
        try:
            char = self.serQueue.get_nowait()
        except queue.Empty:
            char = None
        return char

   
    def serialDataReady(self):
        return self.serQueue.qsize()

        
    def clearBuffer(self):
        self.s.reset_input_buffer()
        while not self.serQueue.empty():
            self.serQueue.get()
        self.s.reset_output_buffer()