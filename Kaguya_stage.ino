#include <Adafruit_NeoPixel.h>

#define PIN 6
#define STRIPSIZE 12
#define WHATSAPPNOTIFICATION "new WANOT"
#define MAX_MESSAGE_LENGTH 12
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600); //This pipes to the serial monitor
  strip.begin();
  strip.setBrightness(100);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(255, 0, 0), 100); // Red
}

void loop() {
  delay(1000);
  IncomingNotification();
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
void TwoBlinkNotification(uint32_t color, int speed=100)
{
  colorWipe(strip.Color(255, 0, 0), 0);
  delay(speed);
  colorWipe(strip.Color(255, 0, 0), 0);
  delay(speed);
}



// Handles Serial input
void IncomingNotification() {
  
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    } else 
    {
      //Add null character to string
      message[message_pos] = '\0';

      //Print the message (or do other things)
      Serial.println(message); // put LED function here
      if(message == WHATSAPPNOTIFICATION)
      {
        // blink green twice
        TwoBlinkNotification(strip.Color(0, 255, 0), 50); 
      } else 
      {
        //Do Nothing        
      }
      //Reset for the next message
      message_pos = 0;
    }
  } // end while
}