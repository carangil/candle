#include <Adafruit_NeoPixel.h>

#define PIN 0

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.




void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  

}



//0 1
//2 3

#define MINUNCALM (5*256)
#define MAXUNCALM (60*256)


#define UNCALMINC 10

#define MAXDEV    100

int centerx=MAXDEV;
int centery=MAXDEV/2;



int xvel=0;
int yvel=0;

int uncalm=MINUNCALM;
int uncalmdir= UNCALMINC;

char cnt=0;




uint32_t powcolor(int val) {


  
  if (val > 255)
    val=255;
    
  if(val <0)
    val=0;
  
  
  return strip.Color( val/4, val, val/16);
  
}


void loop() {
  
  
  int movx=0;
  int movy=0;
    
  
   //random trigger brightness oscillation, if at least half uncalm
   if (uncalm > (MAXUNCALM/2)) {
     

      if (random(2000)<5) {
          uncalm = MAXUNCALM*2;  //occasional 'bonus' wind
      }
          
   }
   
 
  
  
  //random poke, intensity determined by uncalm value (0 is perfectly calm)
  
  movx = random(uncalm>>8) -(uncalm>>9);
  movy = random(uncalm>>8) -(uncalm>>9);

  

  //if reach most calm value, start moving towards uncalm
  if (uncalm < MINUNCALM)
  {
    
    uncalmdir=UNCALMINC;
  }  
  
  //if reach most uncalm value, start going towards calm
  if (uncalm > MAXUNCALM)
  {
    uncalmdir=-UNCALMINC;
  }
  
  uncalm += uncalmdir;


  //move center of flame by the current velocity
  
  centerx+=movx +(xvel>>2);
  centery+=movy +(yvel>>2);
  
  
  //range limits
  if (centerx < -MAXDEV)
     centerx = -MAXDEV;
    
  if (centerx > MAXDEV)
     centerx = MAXDEV;


  if (centery < -MAXDEV)
     centery = -MAXDEV;
    
  if (centery > MAXDEV)
     centery = MAXDEV;




  //counter
  cnt++;
  
   if (! (cnt &3)) {
    
    //attenuate velocity 1/4 clicks
    
    
    xvel = (xvel *999)/1000;
    yvel = (yvel *999)/1000;
 }
  

  //apply acceleration towards center, proportional to distance from center (spring motion; hooke's law)

  xvel -= centerx;
  yvel -= centery;

    
  //g r b

  
  strip.setPixelColor( 0, powcolor(128 - centerx - centery ));
  strip.setPixelColor( 1, powcolor(128 + centerx - centery   ));
  strip.setPixelColor( 2, powcolor(128 + centerx + centery  ));
  strip.setPixelColor( 3, powcolor(128 - centerx + centery ));
  
  strip.show();
  
  delay(20);
  
}
