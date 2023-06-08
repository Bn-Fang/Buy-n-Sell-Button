import serial, os
serialComm = serial.Serial('COM3', 115200)
serialComm.timeout = 1    
from tkinter.simpledialog import askstring

while True:
    answer1 = None
    ms = serialComm.readline().decode()
    if '#' in ms :
        purchase = ""
        ms = ms.split('#')[1]
        if '|' in ms: 
            purchase = ms.split("|")[0]
            stock = ms.split("|")[1]
            unit = ms.split("|")[2]
            qty = ms.split("|")[3]
        if purchase == "run BUY" : 
            p = os.system("python Buy.py" + " " + stock + " " + qty + " " + unit)
            text = "#"+str(p)+"#"
            serialComm.write(text.encode())
            print("purchased")
        elif purchase == "run SELL" : 
            p = os.system("python Sell.py" + " " + stock + " " + qty + " " + unit)
            text = "#"+str(p)+"#"
            serialComm.write(text.encode())
            print("SELL")
        elif ms == "run INPUT" : 
            while answer1 is None:
                answer1 = askstring("Input", "What Security would you like to trade?")
            serialComm.write(answer1.encode())
    if ms == "" : continue
    print(ms)
    