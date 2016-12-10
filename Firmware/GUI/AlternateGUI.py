# -*- coding: utf-8 -*-
"""
Created on Wed Dec  7 23:03:04 2016

@author: kid group
"""

from tkinter import *
from tkinter import ttk

class NotebookDemo(ttk.Frame):
    
    def __init__(self, isapp=True, name='notebookdemo'):
        ttk.Frame.__init__(self, name=name)
        self.pack(expand=Y, fill=BOTH)
        self.master.title('Notebook Demo')
        self._create_demo_panel()
        
    def _create_demo_panel(self):
        demoPanel = Frame(self, name='demo')
        demoPanel.pack(side=TOP, fill=BOTH, expand=Y)
                
        # create the notebook
        nb = ttk.Notebook(demoPanel, name='notebook')

        # extend bindings to top level window allowing
        #   CTRL+TAB - cycles thru tabs
        #   SHIFT+CTRL+TAB - previous tab
        #   ALT+K - select tab using mnemonic (K = underlined letter)
        nb.enable_traversal()
        
        nb.pack(fill=BOTH, expand=Y, padx=2, pady=3)
        self._create_descrip_tab(nb)
        self._create_disabled_tab(nb)
        self._create_text_tab(nb)
                       
    def _create_descrip_tab(self, nb):
        # frame to hold contentx
        frame = ttk.Frame(nb, name='descrip')
        
        # widgets to be displayed on 'Description' tab
        msg = ["Ttk is the new Tk themed widget set. One of the widgets ",
               "it includes is the notebook widget, which provides a set ",
               "of tabs that allow the selection of a group of panels, ",
               "each with distinct content. They are a feature of many ",
               "modern user interfaces. Not only can the tabs be selected ",
               "with the mouse, but they can also be switched between ",
               "using Ctrl+Tab when the notebook page heading itself is ",
               "selected. Note that the second tab is disabled, and cannot "
               "be selected."]
        
        lbl = ttk.Label(frame, wraplength='4i', justify=LEFT, anchor=N,
                        text=''.join(msg))
        neatVar = StringVar()
        btn = ttk.Button(frame, text='Neat!', underline=0, 
                         command=lambda v=neatVar: self._say_neat(v))
        neat = ttk.Label(frame, textvariable=neatVar, name='neat')
        
        # position and set resize behaviour
        lbl.grid(row=0, column=0, columnspan=2, sticky='new', pady=5)
        btn.grid(row=1, column=0, pady=(2,4))
        neat.grid(row=1, column=1,  pady=(2,4))
        frame.rowconfigure(1, weight=1)
        frame.columnconfigure((0,1), weight=1, uniform=1)
        
        # bind for button short-cut key
        # (must be bound to toplevel window)
        self.winfo_toplevel().bind('<Alt-n>', lambda e, v=neatVar: self._say_neat(v))
        
        # add to notebook (underline = index for short-cut character)
        nb.add(frame, text='Description', underline=0, padding=2)
        
    def _say_neat(self, v):
        v.set('Yeah, I know...')
        self.update()
        self.after(500, v.set(''))

    # =============================================================================
    def _create_disabled_tab(self, nb):
        # Populate the second pane. Note that the content doesn't really matter
        frame = ttk.Frame(nb)
        nb.add(frame, text='Disabled', state='disabled')

    # =============================================================================
    def _create_text_tab(self, nb):
        # populate the third frame with a text widget
        frame = ttk.Frame(nb)
        
        txt = Text(frame, wrap=WORD, width=40, height=10)
        vscroll = ttk.Scrollbar(frame, orient=VERTICAL, command=txt.yview)
        txt['yscroll'] = vscroll.set
        vscroll.pack(side=RIGHT, fill=Y)
        txt.pack(fill=BOTH, expand=Y)
        
        # add to notebook (underline = index for short-cut character)
        nb.add(frame, text='Text Editor', underline=0)
        
        
if __name__ == '__main__':
    NotebookDemo().mainloop()