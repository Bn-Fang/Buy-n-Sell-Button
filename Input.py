from tkinter import *
from tkinter import simpledialog
import serial

ws = Tk()
ws.title("Coin Button")
serialComm = serial.Serial('COM3', 115200)
serialComm.timeout = 1    
answer1 = None
while answer1 is None:
    answer1 = simpledialog.askstring("Input", "What Security would you like to trade?")
serialComm.write(answer1.encode())





