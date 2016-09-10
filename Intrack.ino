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
    for (int i = 0 ; i < 1 ; i ++){
       readRGB(i);
    }
}

void readRGB(int i){
    Serial.print("Reading  "); Serial.println(i); 

    tcaselect(i);
    tcs.begin();
    uint16_t clear, red, green, blue;
    delay(60);  // takes 50ms to read 
   
    tcs.getRawData(&red, &green, &blue, &clear);

    uint32_t sum = clear;
    float r, g, b;
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;
    r *= 256; g *= 256; b *= 256;
    sprintf(szInfo, "%d,%d,%d", (int)r, (int)g, (int)b);
    //Spark.publish("colorinfo", szInfo);
   // Serial.println(szInfo);
   Serial.println(colorSelect(r,g,b));
    delay(200);
}


char colorSelect(int a, int b  , int c){
    int max = -65530;
    char color = 'x';
    if (a > 100 && b > 100 && c > 100) {
        return 'w'; 
    }
    if (a > max){ 
        max = a;
        color = 'r';    
    }
    if (b > max){ 
        max = b;
        color = 'g';    
    }
    if (c > max){ 
        max = c;
        color = 'b';    
    }
    
    return color;
}
