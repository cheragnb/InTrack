// This #include statement was automatically added by the Spark IDE.
//dledfasfafagagdgdgdgdsgsdfg
#include "Adafruit_TCS34725/Adafruit_TCS34725.h"
#include <math.h>
#include "application.h"
#define TCAADDR 0x70

boolean commonAnode = false;
char szInfo[128];
char szInfo1[128];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
    Serial.begin(9600);
    Serial.println("Color View Test!");
    
    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
    }
}

 
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void loop() {
    Serial.println("Now 1"); //
    tcaselect(0);
    tcs.begin();
    uint16_t clear, red, green, blue;

    tcs.setInterrupt(false);      // turn on LED
    
    delay(60);  // takes 50ms to read 
      
    tcs.getRawData(&red, &green, &blue, &clear);
    tcs.setInterrupt(true);  // turn off LED
    
    // Figure out some basic hex code for visualization
    uint32_t sum = clear;
    float r, g, b;
    
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;
    r *= 256; g *= 256; b *= 256;
    
    sprintf(szInfo, "%d,%d,%d", (int)r, (int)g, (int)b);
    
    Spark.publish("colorinfo", szInfo);
    
    Serial.println(szInfo);
    
    delay(1000);
    
    Serial.println("Now 2"); //
    tcaselect(1);
    tcs.begin();

    tcs.setInterrupt(false);      // turn on LED
    
    delay(60);  // takes 50ms to read 
      
    tcs.getRawData(&red, &green, &blue, &clear);
    tcs.setInterrupt(true);  // turn off LED
    
    // Figure out some basic hex code for visualization
     sum = clear;
     
    
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;
    r *= 256; g *= 256; b *= 256;
    
    sprintf(szInfo1, "%d,%d,%d", (int)r, (int)g, (int)b);
    
    Spark.publish("colorinfo", szInfo);
    
    Serial.println(szInfo1);
    
    delay(1000);
}

