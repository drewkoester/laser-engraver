#include <Adafruit_NeoPixel.h>

#define SWITCH 0

#define PIN       0
#define NUM_LEDS 12

int maxBrightness     = 150;
int defaultBrightness = 75;
int minBrightness     = 10;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


uint8_t  mode   = 0,             // Current animation effect
         offset = 0;             // Position of spinner animation
uint32_t greenColor  = 0x20c2c7; 
uint32_t redColor    = 0xc72b20; 


uint32_t prevTime;               // Time of last animation mode switch

void setup() {
  pixels.begin();
  pixels.setBrightness(defaultBrightness); 
  prevTime = millis();      // Starting time
}

// only turn on certain lights (leaves a gap)
void bowtieColor(int i, uint32_t color) {
  uint32_t colorChange = greenColor;
//  todo: Random party mode
  if(random(0,200) == 1)
    partyMode();
  else{
    if((i == 11) || (i == 0)  || (i == 1) )
      pixels.setPixelColor(i, 0);
    else if((i == 5) || (i == 6)  || (i == 7) )
      pixels.setPixelColor(i, 0);
    else {
      blinkLed(i, colorChange);
    }
  }
}

void partyMode(){
  uint8_t  i;
  for(i = 0; i < 10; i++){
    partyAction();
  }
}

void partyAction(){
  uint8_t  j;
  for(j = 0; j< NUM_LEDS; j++) {
    pixels.setPixelColor(j, greenColor); // Set it to current color
    pixels.show();                  // Refresh LED states
    pixels.setPixelColor(j, 0);
    delay(10);                      // 10 millisecond delay
  }
}

// randomly determines if a LED is lit or not
void blinkLed(int i, uint32_t color) {
  if(random(0,2) == 0)
    pixels.setPixelColor(i, color);
  else
    pixels.setPixelColor(i, 0);
}

// creates a 'basic' fade look
void fadeLed(uint8_t currentLevel){
  boolean run = true;
  int adjust = random(10, 50);
  int polarity = random(0,2);
  if((currentLevel > maxBrightness) || (currentLevel < minBrightness)){
    pixels.setBrightness(defaultBrightness);
    run = false;
  }
        
  if(run == true){
    uint8_t  i;
    for(i = 0; i< adjust; i++) {
      if(polarity == 1)
        pixels.setBrightness(currentLevel + i);
      else
        pixels.setBrightness(currentLevel - i);    
      delay(5);
    }    
  }
}

// Simple function for testing everything by setting a single color
void testLED() {
  uint8_t  i;
  for( i = 0; i < NUM_LEDS; i++ ) {
    pixels.setPixelColor( i, 0x00FF00 );
  }
}

void basicLED(){
  uint8_t  i;
  for(i=0; i<NUM_LEDS; i++) {    // For each LED...
    bowtieColor(i, greenColor);
  }
}

void loop() {
  uint8_t  t;
//  testLED();
  basicLED();

  pixels.show();                 // Refresh LED states
  delay(random(750,5000));       // how fast the spinner goes

  offset++;                      // Shift animation by 1 pixel on next frame

  t = millis();                    // Current time in milliseconds
  if((t - prevTime) > 5000) {      // Every 8 seconds...
    fadeLed(pixels.getBrightness());
    pixels.clear();                // Set all pixels to 'off' state
    prevTime = t;                  // Record the time of the last mode change
  }
}
