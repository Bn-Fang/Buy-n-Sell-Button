// #include "FastLED.h"
// #define LED_PIN 14
// #define NUM_LEDS 3
// CRGB leds[NUM_LEDS];

// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #include <Fonts/FreeMonoBoldOblique12pt7b.h>
// #include <Fonts/FreeSerif9pt7b.h>

// #define SCREEN_WIDTH 128  // OLED display width, in pixels
// #define SCREEN_HEIGHT 64  // OLED display height, in pixels

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// #define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// #define CLK 2
// #define DT 13
// #define SW 16
// int counter = 0;
// int currentStateCLK;
// int lastStateCLK;
// String currentDir ="";
// unsigned long lastButtonPress = 0;


// void setup() {
//   FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
//   // Set LEDs and rotary encoder pins as inputs
        
// 	// Set encoder pins as inputs
// 	pinMode(CLK,INPUT);
// 	pinMode(DT,INPUT);
// 	pinMode(SW, INPUT_PULLUP);

// 	// Setup Serial Monitor
// 	Serial.begin(9600);

// 	// Read the initial state of CLK
// 	lastStateCLK = digitalRead(CLK);

//   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//   display.clearDisplay();
//   display.display();
// }

// void loop() {
//   // Read the state of the encoder's pushbutton value
//   int SWstate = digitalRead(SW);

//   // Lights the yellow LED to indicate increasing rotation speed
//   if (SWstate == 0) {
//     fill_solid(leds, NUM_LEDS, CRGB(100, 100, 100));
//     FastLED.show();
//   } else {
//     fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
//     FastLED.show();
//   }

//   currentStateCLK = digitalRead(CLK);
// 	// If last and current state of CLK are different, then pulse occurred
// 	// React to only 1 state change to avoid double count
// 	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

// 		// If the DT state is different than the CLK state then
// 		// the encoder is rotating CCW so decrement
// 		if (digitalRead(DT) != currentStateCLK) {
// 			counter ++;
// 			currentDir ="CW";
// 		} else {
// 			// Encoder is rotating CW so increment
// 			counter --;
// 			currentDir ="CCW";
// 		}

// 		Serial.print("Direction: ");
// 		Serial.print(currentDir);
// 		Serial.print(" | Counter: ");
// 		Serial.println(counter);
    
//     // delay(1);
//     displayText(String(counter), 2);
// 	}
// 	// Remember last CLK state
// 	lastStateCLK = currentStateCLK;
//   delay(1);
// }

// void displayText(String Text, int textSize) {
//   //Set text size
//   display.setTextSize(textSize);
//   //Set text color
//   display.setTextColor(WHITE);
//   //Set text location
//   display.setCursor(6, 2);
//   display.println(String(Text));
//   //"verify" the screen
//   display.display();
// }
