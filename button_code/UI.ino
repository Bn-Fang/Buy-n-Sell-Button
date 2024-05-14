#include "FastLED.h"
#define LED_PIN 14
#define NUM_LEDS 3
CRGB leds[NUM_LEDS];
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define CLK 2
#define DT 13
#define SW 16
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
String stock = "ETHUSDT";
int NUM[7]={0,0,0,0,0,0,0};
int QTY = 10000020;
int ui = 0;
int decimal = 9;
bool type = false;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  // Set LEDs and rotary encoder pins as inputs  
	// Set encoder pins as inputs
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
	// Setup Serial Monitor
	Serial.begin(115200);
	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(2);
  display.clearDisplay();
  display.setTextWrap(false);
  display.display();
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  FastLED.show();
}

void loop() {
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  FastLED.show();
  double price =0.0;
  ui = Home(QTY, stock);
  waitForButton();
  Serial.print("out of home");
  if (ui == 0){
    Serial.print("selecting stock");
    stock = stockSel();
    waitForButton();
  } else if (ui == 1){
    Serial.print("selecting quantity");
    QTY = quantitySel(QTY);
    waitForButton();
  } else if (ui == 2){
    Serial.print("buy?");
    fill_solid(leds, NUM_LEDS, CRGB(0,0,100));
    FastLED.show();
    bool buy = awaitYN("Buy", "Back");
    waitForButton();
    if (buy == true){
      fill_solid(leds, NUM_LEDS, CRGB(100, 100, 0));
      FastLED.show();
      QTY -= 10000000;
      if (type ==true){
        if (decimal > 6) decimal = 6;
        price = QTY / pow(10, (6-decimal));
      }
      Serial.print("#run BUY|");
      Serial.print(stock);
      Serial.print("|");
      Serial.print(type);
      Serial.print("|");
      if (type == true){
        Serial.print(price,6);
      }else{
        Serial.print(QTY);
      }
      Serial.println("#");
      QTY +=10000000;
      String input ="";
      while (true){
        input = Serial.readStringUntil('\n');
        if (input !="") break;
      }
      if (input == "#0#"){
        fill_solid(leds, NUM_LEDS, CRGB(0, 100, 0));
        FastLED.show();
        displayText("Bought:)", 2);
      } else{
        fill_solid(leds, NUM_LEDS, CRGB(100, 0, 0));
        FastLED.show();
        displayText("Error:(", 2);
      }
      delay(3000);
      fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
      FastLED.show();
    }
  }else if (ui == 3){
    Serial.print("sell??");
    fill_solid(leds, NUM_LEDS, CRGB(0,0,100));
    FastLED.show();
    bool sell = awaitYN("Sell", "Back");
    waitForButton();
    if (sell == true){
      fill_solid(leds, NUM_LEDS, CRGB(100, 100, 0));
      FastLED.show();
      QTY -= 10000000;
      if (type ==true){
        if (decimal > 6) decimal = 6;
        price = QTY / pow(10, (6-decimal));
      }
      Serial.print("#run SELL|");
      Serial.print(stock);
      Serial.print("|");
      Serial.print(type);
      Serial.print("|");
      if (type == true){
        Serial.print(price,6);
      }else{
        Serial.print(QTY);
      }
      Serial.println("#");
      String input ="";
      QTY +=10000000;
      while (true){
        input = Serial.readStringUntil('\n');
        if (input !="") break;
      }
      if (input == "#0#"){
        fill_solid(leds, NUM_LEDS, CRGB(0, 100, 0));
        FastLED.show();
        displayText("Sold:)", 2);
      } else{
        fill_solid(leds, NUM_LEDS, CRGB(100, 0, 0));
        FastLED.show();
        displayText("Error:(", 2);
      }
      delay(3000);
      fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
      FastLED.show();
    }
  }
  waitForButton();
  if ((QTY>20000000)||(QTY<0) ){
    QTY = 10000020;
  }
  ui = 0;
}

String stockSel(){
  int response = digitalRead(SW);
  int choice = 0;
  int prev = -1;
  int cur = 0;
  int last = cur;
  while (response != 0){
    response = digitalRead(SW);
    cur = digitalRead(CLK);
    if (cur != last  && cur == 1){
      if (digitalRead(DT) != cur) {
        choice ++;
        if (choice > 3) choice = 3;
      } else {
        choice --;
        if (choice < 0) choice = 0;
      }
    }
    last = cur;
    // Serial.println(choice);
    displaySEL(prev, choice);
    prev = choice;
    delay(1);
  }
  waitForButton();
  String input = "";
  if (choice == 0){
    return "BTCUSDT";
  } else if (choice == 1){
    return "ETHUSDT";
  } else if (choice == 2){
    return "ADAUSDT";
  }else if (choice == 3){
    Serial.println("#run INPUT#");
    while (true){
      input = Serial.readStringUntil('\n');
      if (input !="") break;
    }
    return input;
  }
  return "ERROR";
}

void displaySEL(int prev, int choice){
  display.clearDisplay();
  if (choice == prev) return;
  String stockChoice[] = {"BTCUSDT", "ETHUSDT", "ADAUSDT", ". . ."};
  for(int i=0; i<4; i++){
    if (i == choice){
      display.fillRect(0, 16*choice, 128, 16, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(0, 16*choice);
      display.println(stockChoice[choice]);
    }else{
      display.setTextColor(WHITE);
      display.setCursor(0, 16*i);
      display.println(stockChoice[i]);
    }
  }
  display.display();
}

float quantitySel(int QTY){
  int num = QTY;
  num += 10000000;
  NUM[0] = (num / 1000000U) % 10;
  NUM[1] = (num / 100000U) % 10;
  NUM[2] = (num / 10000U) % 10;
  NUM[3] = (num / 1000U) % 10;
  NUM[4] = (num / 100U) % 10;
  NUM[5] = (num / 10U) % 10;
  NUM[6] = (num / 1U) % 10;
  for (int i =0; i<7; i++){
    Serial.println(NUM[i]);
  }
  int response = digitalRead(SW);
  int choice = decimal;
  int cur = 0;
  int last = cur;
  int confirm = 0;
  int inputType = false;
  int prev =-1;
  String input ="";
 
  type = awaitYN("QTY", "USD"); 
  if (type == false) decimal = 8;
  else{
    choice --;
    while (response != 0){
      response = digitalRead(SW);
      cur = digitalRead(CLK);
      if (cur != last  && cur == 1){
        if (digitalRead(DT) != cur) {
          choice ++;
          if (choice > 6) choice = 0;
        } else {
          choice --;
          if (choice < 0) choice = 6;
        }
      
      }
      last = cur;  
      displayDec(prev, choice);
      prev = choice;
      delay(1);
    } 
    waitForButton();
  }
  while(true){
    response = digitalRead(SW);
    choice = 0;
    cur = 0;
    last = cur;
    while (response != 0){// select digit
      response = digitalRead(SW);
      cur = digitalRead(CLK);
      if (cur != last  && cur == 1){
        if (digitalRead(DT) != cur) {
          choice ++;
          if (choice > 7) choice = 0;
        } else {
          choice --;
          if (choice < 0) choice = 7;
        }
      }
      last = cur;
      // Serial.println(choice);
      displayDig(prev, choice, decimal, NUM);
      prev = choice;
      delay(1);
    }
    if (choice == 7) break;
    waitForButton();
    int pos = choice;
    response = digitalRead(SW);
    cur = -1;
    last = cur;
    choice = NUM[pos] -1;
    while (response != 0){
      response = digitalRead(SW);
      cur = digitalRead(CLK);
      if (cur != last  && cur == 1){
        if (digitalRead(DT) != cur) {
          choice ++;
          if (choice > 9) choice = 0;
        } else {
          choice --;
          if (choice < 0) choice = 9;
        }
      }
      NUM[pos]= choice;
      last = cur;
      
      displayAll(prev, choice, pos, decimal, NUM);
      prev = choice;
      delay(1);
    }
    waitForButton();
  }
  waitForButton();
  choice = 0;
  for (int i =0; i < 7; i++){
    int temp = NUM[i];
    Serial.println(temp*(pow(10,6-i)));
    choice += (temp*(pow(10,6-i)));
  }
  choice += 10000000;
  Serial.println(choice);
  return choice;
}

void displayDec(int prev, int choice){
  if(prev == choice) return;
  display.clearDisplay();
  for (int i =0; i <7; i++){
    display.setTextColor(WHITE);
    display.setCursor(i*16 +2 , 22);
    display.setTextSize(2);
    display.print(NUM[i]);
    
  }
  for (int i =0; i <7; i++){
      display.fillRect(i * 16 + 15, 37, 2,2, BLACK);
  }
  Serial.print("choice: ");
  Serial.println(choice);
  display.fillRect(choice * 16 + 15, 37, 2,2, WHITE);
  decimal = choice;
  display.display();
}

void displayDig(int prev, int choice, int dig, int NUM[]){
  if (prev == choice) return;
  display.clearDisplay();
  display.fillRect(dig * 16 + 15, 37, 2,2, WHITE);
  for (int i =0; i <8; i++){
    if (i == choice){
      display.fillRect(choice * 16, 20, 14, 18, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(choice*16 +2 , 22);
      display.setTextSize(2);
      if(i == 7 ){
        display.print(">");
      }else{
        display.print(NUM[i]);
      }
    } else {
      display.fillRect(i * 16, 20, 14, 18, BLACK);
      display.setTextColor(WHITE);
      display.setCursor(i*16 +2 , 22);
      display.setTextSize(2);
      if(i == 7 ){
        display.print(">");
      }else{
        display.print(NUM[i]);
      }
    }
  }
  display.display();
}

void displayAll(int prev, int newin, int choice, int dig, int NUM[]){
  if (prev == newin) return;
  display.fillRect(dig * 16 + 15, 37, 2,2, WHITE);
  for (int i =0; i <8; i++){
    if (i == choice){
      display.fillRect(choice * 16, 20, 14, 18, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(choice*16 +2 , 22);
      display.setTextSize(2);
      if(i == 7 ){
        display.print(">");
      }else{
        display.print(NUM[i]);
      }
    } else {
      display.fillRect(i * 16, 20, 14, 18, BLACK);
      display.setTextColor(WHITE);
      display.setCursor(i*16 +2 , 22);
      display.setTextSize(2);
      if(i == 7 ){
        display.print(">");
      }else{
        display.print(NUM[i]);
      }
    }
  }
  display.display();
}



int Home(int QTY, String SEL){
  int response = digitalRead(SW);
  int choice = 0;
  int prev = -1;
  int cur = 0;
  int last = cur;
  while (response != 0){
    response = digitalRead(SW);
    cur = digitalRead(CLK);
    if (cur != last  && cur == 1){
      if (digitalRead(DT) != cur) {
        choice ++;
        if (choice > 3) choice = 3;
      } else {
        choice --;
        if (choice < 0) choice = 0;
      }
    }
    last = cur;
    // Serial.println(choice);
    displayHome(prev, choice, QTY, SEL);
    prev = choice;
    delay(1);
  }
  waitForButton();
  return choice;
}


void displayHome(int prev, int choice, int QTY, String SEL){
  display.clearDisplay();
  if (choice == prev) return;
  double price = 0.0;
  QTY -= 10000000;
  if (type ==true){
    if (decimal > 6) decimal = 6;
    price = QTY / pow(10, (6-decimal));
  }
  
  String homeChoice[4] = {"SEL:", "$:", "BUY", "SELL"};
  if (type == true ) homeChoice[1] = "QTY:";
  for(int i=0; i<4; i++){
    display.setTextSize(2);
    if (i == choice){
      display.fillRect(0, 16*choice+1, 128, 16, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(0, 16*choice+1);
      display.println(homeChoice[choice]);
      display.setTextSize(2);
      if (choice == 0){
        display.setCursor(45, 1);
        display.print(SEL);
      } else if (choice == 1){
        
        display.setCursor(23, 17);
        if (type==true){
          display.setCursor(45, 17);
          display.print(price,5);
        } else{
          display.print(QTY);
        }
        
      }
    }else{
      display.setTextColor(WHITE);
      display.setCursor(0, 16*i+1);
      display.println(homeChoice[i]);
      display.setTextSize(2);
      if (i == 0){
        display.setCursor(45, 1);
        display.print(SEL);
      } else if (i == 1){
        display.setCursor(23, 17);
        if (type==true){
          display.setCursor(45, 17);
          display.print(price,5);
        } else{
          display.print(QTY);
        }
      }
    }
  }
  display.display();
}

bool awaitYN( String Y, String N){
  int response = digitalRead(SW);
  int choice = 0;
  int prev = -1;
  int cur = 0;
  int last = cur;
  while (response != 0){
    response = digitalRead(SW);
    cur = digitalRead(CLK);
    if (cur != last  && cur == 1){
      if (digitalRead(DT) != cur) {
        choice = 1;
      } else {
        choice = 0;
      }
    }
    last = cur;
    displayYN(prev, choice, Y, N);
    prev = choice;
    delay(1);
  }
  waitForButton();
  Serial.print("Selected ");
  Serial.println(choice);
  return choice;
}


void waitForButton(){
  while(true){
    if (digitalRead(SW) == 1) break;
  }
}

void displayYN(int prev, int choice, String Y, String N){
  if (choice == prev) return;
  display.setTextSize(2);
  if (prev == 0){
    display.fillRect(0, 49, 64, 16, BLACK);
  }else if (prev == 1){
    display.fillRect(64, 49, 64, 16, BLACK);
  }else{
    display.fillRect(0, 49, 128, 16, BLACK);
  }
  if (choice == 0){
    display.setTextColor(WHITE);
    display.setCursor(66, 50);
    display.println(Y);
    display.fillRect(0, 49, 64, 16, WHITE);
    display.setTextColor(BLACK);
    display.setCursor(2, 50);
    display.println(N);
  } else if (choice == 1){
    display.setTextColor(WHITE);
    display.setCursor(2, 50);
    display.println(N);
    display.fillRect(64, 49, 64, 16, WHITE);
    display.setTextColor(BLACK);
    display.setCursor(66, 50);
    display.println(Y);
  }
  display.display();
}

void displayText(String Text, int textSize) {
  //Set text size
  display.setTextSize(textSize);
  //Set text color
  display.setTextColor(WHITE);
  //Set text location
  display.setCursor(6, 2);
  display.println(String(Text));
  //"verify" the screen
  display.display();
}
