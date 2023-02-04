#include <Adafruit_NeoPixel.h>

#define PIN 6
#define STRIPSIZE 12
#define MAX_MESSAGE_LENGTH 12
#define UPDATE_CYCLE 5000

// constants
static const char WHATSAPPNOTIFICATION[MAX_MESSAGE_LENGTH] = "new WANOT";

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);
static const uint32_t standard_color = (255, 0, 0);
static const uint32_t black_color = (0, 0, 0);

void setup() {
  Serial.begin(9600); //This pipes to the serial monitor
  strip.begin();
  strip.setBrightness(127);
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(255, 0, 0), 1000); // Red
}

void loop() {
  delay(UPDATE_CYCLE);
  if (Serial.available() > 0)
  {
    IncomingNotification();
    // return to red
    colorWipe(strip.Color(255, 0, 0), 100);
  } 
  else 
  {
    //Do nothing
  }  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// makes LED strip blink twice when called
void TwoBlinkNotification(uint32_t color, int speed=50)
{
  //change to color
  colorWipe(color, speed);
  //blink to black
  colorWipe(black_color, speed);
  //change to color
  colorWipe(color, speed);
  //blink to black
  colorWipe(black_color, speed);
}



// Handles Serial input
void IncomingNotification() {
  
  while (Serial.available() > 0)
  {

    //Create a place to hold the incoming message       
    static char readInput[MAX_MESSAGE_LENGTH];
    static unsigned int read_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (read_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      readInput[read_pos] = inByte;
      read_pos++;
    } else 
    {
      
      //Print the message (or do other things) 
      if(strcmp(readInput, WHATSAPPNOTIFICATION) == 0)
      {
        // blink green twice
        TwoBlinkNotification(strip.Color(0, 255, 0), 50); 
        delay(500);
        TwoBlinkNotification(strip.Color(0, 255, 0), 50); 
        delay(500);
      } else 
      {
        //Do Nothing        
      }
      //Reset for the next message
      read_pos = 0;
    }
  } // end while
}