#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
E_Moto Debugger GUI

In this script, we set up the GUI and initialize the Serial Communication

Author: Zach Levenberg
Last modified: December 2016
"""
try:
    from tkinter import *
    from tkinter import ttk
except ImportError:
    from Tkinter import Tk, Text, BOTH, W, N, E, S, Frame, Button, Label, OptionMenu, StringVar, Menu, IntVar, CheckButton

try:
    import queue
except ImportError:
    import Queue as queue

from SerialComPorts import SerialCom_Thread

import binascii


connectionState = "disconnected"

                
                
class myScreen(ttk.Frame):
  
    def __init__(self, parent):
        ttk.Frame.__init__(self, parent)   
        self.pack(expand=Y, fill=BOTH)
         
        self.parent = parent
        self.initUI()
        
        
    def initUI(self):

      
        self.parent.title("Welcome to the E-Moto Debugger Tool!")
        self.pack(fill=BOTH, expand=True)
        
        menubar = Menu(self)
        filemenu = Menu(menubar, tearoff=0)
        filemenu.add_command(label="New")
        filemenu.add_command(label="Open")
        filemenu.add_command(label="Save")
        filemenu.add_command(label="Save as...")
        filemenu.add_command(label="Close")
        filemenu.add_separator()
        filemenu.add_command(label="Exit", command=self.quit)
        menubar.add_cascade(label="File", menu=filemenu)
        editmenu = Menu(menubar, tearoff=0)
        editmenu.add_command(label="Undo")
        
        editmenu.add_separator()
        
        editmenu.add_command(label="Cut")
        editmenu.add_command(label="Copy")
        editmenu.add_command(label="Paste")
        editmenu.add_command(label="Delete")
        editmenu.add_command(label="Select All")
        
        menubar.add_cascade(label="Edit", menu=editmenu)
        helpmenu = Menu(menubar, tearoff=0)
        helpmenu.add_command(label="Help Index")
        helpmenu.add_command(label="About...")
        menubar.add_cascade(label="Help", menu=helpmenu)
        self.parent.config(menu=menubar)
        
        #Create a grid of size rowsxcols
        rows=14
        cols=6
        self.rowconfigure(1, weight=0)
        self.rowconfigure(2, weight=0, minsize=50)
        self.rowconfigure(3, weight=0, minsize=50)
        self.rowconfigure(4, weight=0, minsize=50)
        self.rowconfigure(5, weight=0, minsize=50)
        self.rowconfigure(6, weight=1, minsize=50)
        for i in range(7,rows):
                self.rowconfigure(i, weight=0)
        for i in range(1,cols-2):
            self.columnconfigure(i, weight=1)
        self.columnconfigure(cols-1, weight=0, minsize=150, pad=5)
        self.columnconfigure(cols, weight=0, minsize=150, pad=5)
        #Status Message
        self.connectionStatus = 'Status = Disconnected'
        self.connectionStatusLabel = Label(self, text=self.connectionStatus)
        self.connectionStatusLabel.grid(row=1, column=1, sticky = "W")

        #Activate Button
        self.connectButton = ttk.Button(self, text="Connect", state="disabled", width=20)
        self.connectButton["command"] = self.openIt
        self.connectButton.grid(row=2, column=cols-1, columnspan=2, rowspan=2)
        
        # Dropdown Menu COM PORTS
        choices = []
        choices.append("NONE")        
        self.dropVar1=StringVar()
        self.dropVar1.set("Port") # default choice
        self.dropMenu1 = ttk.OptionMenu(self, self.dropVar1, *choices, command=self.comMenu)
        self.dropMenu1.config(width=6)
        self.dropMenu1.grid(column=cols-1,row=4)
        
        # Dropdown Menu Baud
        choices2 = ['9600', '14400', '19200', '38400', '57600', '115200', '230400', '460800']
        self.dropVar2=StringVar()
        self.dropVar2.set("Baud") # default choice
        self.dropMenu2 = ttk.OptionMenu(self, self.dropVar2, *choices2, command=self.baudMenu)
        self.dropMenu2.config(width=6)
        self.dropMenu2.grid(column=cols,row=4)
        
        #Refresh button
        self.refreshButton = ttk.Button(self, text="Scan for Devices", width=30)
        self.refreshButton["command"] = self.updateComs
        self.refreshButton.grid(row=5, column=cols-1, columnspan=2)
        
        #Create Serial Console Text Area
        self.consoleCount = 0
        self.consoleTextArea = Text(self, bg="black", fg="#00ff00")
        self.consoleTextArea.grid(row=2, column=0, columnspan=cols-2, rowspan=rows-1, sticky="NESW")
        #Clear button
        self.clearButton = ttk.Button(self, text="Clear Console", width=20)
        self.clearButton["command"] = self.clearConsole
        self.clearButton.grid(row=rows, column=cols-3, columnspan=1, sticky="E")
        
        #Send button
        self.sendButton = ttk.Button(self, text="Send")
        self.sendButton["command"] = self.say_hi
        self.sendButton.grid(row=rows, column=cols, sticky="E")
        self.sendTextArea = Text(self, height=1, width=15)
        self.sendTextArea.grid(row=rows, column=cols-1, columnspan=2)
        
        self.B1var = IntVar()
        self.B1 = Checkbutton(self, text = "FRAMEWORK_debug_EN ", variable = self.B1var, command = self.B1_func, onvalue = 1, offvalue = 0)        
        self.B1.grid(row=rows-6, column=cols-1, sticky="W");
        self.B1.select()
        
        self.B2var = IntVar()
        self.B2 = Checkbutton(self, text = "TOUCHSCREEN_debug_EN", variable = self.B2var, command = self.B2_func, onvalue = 1, offvalue = 0)
        self.B2.grid(row=rows-5, column=cols-1, sticky="W");
        self.B2.select()
        
        self.B3 = ttk.Button(self, text="Send SPI (16bits)")
        self.B3["command"] = self.B3_func
        self.B3.grid(row=rows-4, column=cols-1, sticky="W");
        self.SPI_hex = Text(self, height=1, width=4)
        self.SPI_hex.grid(row=rows-4, column=cols, sticky="W")

        self.B4var = IntVar()
        self.B4 = Checkbutton(self, text = "5V_SW_EN", variable = self.B4var, command = self.B4_func, onvalue = 1, offvalue = 0)
        self.B4.grid(row=rows-3, column=cols-1, sticky="W");
        
        self.B5var = IntVar()
        self.B5 = Checkbutton(self, text = "12V_SW_EN", variable = self.B5var, command = self.B5_func, onvalue = 1, offvalue = 0)
        self.B5.grid(row=rows-2, column=cols-1, sticky="W");
        
        #Create Serail Thread without opening a port
        self.myqueue = queue.Queue()
        self.serialCommunication = SerialCom_Thread(self.myqueue)
        self.serialCommunication.start()



    def comMenu(self,value):
        self.dropVar1.set(value)
        if value != "NONE" and self.dropVar2.get() != "Baud":
            self.connectButton["state"] = 'active'
        else:
            self.connectButton["state"] = 'disabled'
        
    def baudMenu(self,value):
        self.dropVar2.set(value)
        if self.dropVar1.get() != "Port" and self.dropVar1.get() != "NONE" and value != "Baud":
            self.connectButton["state"] = 'active'
        else:
            self.connectButton["state"] = 'disabled'
        
    def updateComs(self):
        self.disconnect()
        self.dropMenu1['menu'].delete(0,'end')
        self.dropVar1.set("Port")
        self.dropVar2.set("Baud")
        self.connectButton["state"] = 'disabled'
        choices = self.serialCommunication.getPort()
        if len(choices) is 0:   
            choices.append("NONE")
        for val in choices:
            self.dropMenu1['menu'].add_command(label=val,command=lambda v=self,l=val:v.comMenu(l))
        
    def serial_read(self):
        while self.myqueue.qsize():
            try:
                self.consoleTextArea.insert('end', self.myqueue.get())
                self.consoleTextArea.see('end')
                self.consoleCount = self.consoleCount+1
                if self.consoleCount >= 1024:
                    self.consoleTextArea.delete(1.0,1.1)
                    self.consoleCount = self.consoleCount-1
            except self.myqueue.Empty:
                pass
        self.after(1, self.serial_read)
    
    def say_hi(self):
        stringToSend = self.sendTextArea.get(0.0, 'end')
        self.serialCommunication.writeToCom(stringToSend.encode())
        
    def B1_func(self):
        if self.B1var.get() == 1:
            hex_string = bytearray.fromhex("00 01 0A")
        else:
            hex_string = bytearray.fromhex("00 00 0A")
        self.serialCommunication.writeToCom(hex_string)
        
    def B2_func(self):
        if self.B2var.get() == 1:
            hex_string = bytearray.fromhex("01 01 0A")
        else:
            hex_string = bytearray.fromhex("01 00 0A")
        self.serialCommunication.writeToCom(hex_string)
        
    def B3_func(self):
        mystring = "02"+self.SPI_hex.get(1.0,1.4)+"0A"
        hex_string = bytes.fromhex(mystring)
        self.serialCommunication.writeToCom(hex_string)
        
    def B4_func(self):
        if self.B4var.get() == 1:
            hex_string = bytearray.fromhex("04 01 0A")
        else:
            hex_string = bytearray.fromhex("04 00 0A")
        self.serialCommunication.writeToCom(hex_string)
        
    def B5_func(self):
        if self.B5var.get() == 1:
            hex_string = bytearray.fromhex("05 01 0A")
        else:
            hex_string = bytearray.fromhex("05 00 0A")
        self.serialCommunication.writeToCom(hex_string)
        
    
    def openIt(self):
        if self.connectionState == "disconnected":
            self.connect()
            self.serial_read()
        elif self.connectionState == "connected":
            self.disconnect()
    
    def clearConsole(self):
        self.consoleTextArea.delete(0.0, 'end')
        self.consoleCount = 0

    
    def connect(self):
        settings = self.serialCommunication.openPort(self.dropVar1.get(), self.dropVar2.get())
        self.consoleTextArea.insert('end', settings)
        self.connectButton["relief"] = 'sunken'
        self.connectButton["text"] = "Disconnect"
        self.serialCommunication.resume()
        self.connectionState = "connected"
        self.connectionStatusLabel.config(text="Status = Connected")
        
    def disconnect(self):
        self.connectionState = "disconnected"
        self.connectButton["relief"] = 'raised'
        self.connectButton["text"] = "Connect"
        self.serialCommunication.closePort()
        self.serialCommunication.pause()
        self.connectionStatusLabel.config(text="Status = Disconnected")

    def quit(self):
        self.disconnect();
        self.parent.destroy()
              

def main():
    root = Tk()
    myScreen(root)
    root.mainloop()  


if __name__ == '__main__':
    main()  