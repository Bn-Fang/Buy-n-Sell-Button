import config, sys
from binance.client import Client
from binance.enums import *
from tkinter.messagebox import showinfo
quantity = 0
price = 0
Stock = str(sys.argv[1])
QTY = float(sys.argv[2])
unit = int(sys.argv[3])
client = Client(config.api_key, config.api_secret, tld='us')
price = client.get_symbol_ticker(symbol=Stock)['price']
lotsize = client.get_symbol_info(Stock)['filters'][2]['stepSize']
minNotional = client.get_symbol_info(Stock)['filters'][3]['minNotional']
if (unit == 0):
    quantity = QTY%float(lotsize)
else:
    quantity = QTY
quantity = round(quantity, 5)
if (float(price)*quantity < float(minNotional)):
    showinfo("Error", "Your order is too small.")
    exit(1)
print("You are about to buy " + str(quantity) + " shares of " + Stock + " at $" + str(price) + " per share. Is this correct? (y/n)")
try:
    order = client.order_market_buy(    
        symbol=Stock,
        quantity=quantity)
except:
    showinfo("Error", "There was an error with your order. Please try again.")
    exit()
# add order to csv
file1 = open("orders.txt", "a")  # append mode
file1.writelines("BUY " + Stock + " " + str(quantity) + " " + str(price) + "\n")
file1.close()
