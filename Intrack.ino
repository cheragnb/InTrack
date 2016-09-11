// This #include statement was automatically added by the Spark IDE.
//dledfasfafagagdgdgdgdsgsdfg
#include "Adafruit_TCS34725/Adafruit_TCS34725.h"
#include <math.h>
#include "application.h"
#define TCAADDR 0x70
#define timeout 1000*5

char pens[] = {'x','x','x','x'};
boolean commonAnode = false;
char szInfo[128];
char szInfo1[128];
bool penpicked[] = {false,false, false, false};
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);



Timer timer(100, updatePens, false);
Timer pen1(timeout, penMissing1, true);
Timer pen2(timeout, penMissing2, true);
Timer pen3(timeout, penMissing3, true);



void setup() {
    Serial.begin(9600);
    pinMode(D7, OUTPUT);
    pinMode(D5, INPUT);
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

    penPicked();
    delay(1000);
}

char readRGB(int i){

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
   //  sprintf(szInfo, "%d,%d,%d", (int)r, (int)g, (int)b);
//    sprintf(szInfo1, "%d", colorSelect(r,g,b));
 //   Serial.println(szInfo);
//    Serial.println(colorSelect(r,g,b));
//    Spark.publish("colorinfo", (char)szInfo1);
   // Serial.println(tcs.calculateColorTemperature(red,green,blue));
    // if (tcs.calculateColorTemperature(red,green,blue) < 5000) {
    //     return 'x';
    // }
   // Serial.println(tcs.calculateLux(red,green,blue));
  //  delay(100);
    return colorSelect(r,g,b);
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

void updatePens(){
    for (int i = 0 ; i < 4; i ++){
        pens[i] = readRGB(i);
    }
}

void penPicked(){
    int pins[]  = {D5,D4,D3,D2};
    for (int i = 0 ; i < 4; i ++){
        if ((!digitalRead(pins[i])) && (penpicked[i] == false)){

            updatePens();
            //delay(50);
            Serial.print(pens[i]); Serial.print("   Penn picked at  "); Serial.println(i);
            Particle.publish("Out",(String)pens[i]);
            penpicked[i] = true;
            pen1.start();
        }
        if ((digitalRead(pins[i])) && (penpicked[i] == true)){
            updatePens();
            Serial.print(pens[i]); Serial.print("   Penn returned at   "); Serial.println(i);
            Particle.publish("In",(String)pens[i]);
            penpicked[i] = false;
            pen1.stop();
        }
    }
}


void penMissing1(){
    int pins[]  = {D5,D4,D3,D2};
      for (int i = 0 ; i < 4; i ++){
        if(!digitalRead(pins[i])){
        Serial.print ("missing for first period ");Serial.print (pens[i]);Serial.print ("  at  "); Serial.println (i);
        Particle.publish("First Warning",(String)pens[i]);
        pen2.start();
       // vibrate(500);
    }
}
}

void penMissing2(){
    int pins[]  = {D5,D4,D3,D2};
      for (int i = 0 ; i < 4; i ++){
       if(!digitalRead(pins[i])){
       Serial.print ("missing for second period ");Serial.print (pens[i]);Serial.print ("  at  "); Serial.println (i);
       Particle.publish("Second Warning",(String)pens[i]);
       Particle.publish("text1",(String)pens[i]);
        pen3.start();
      //  vibrate(500);
        }
      }
}

void penMissing3(){
    int pins[]  = {D5,D4,D3,D2};
      for (int i = 0 ; i < 4; i ++){
        if(!digitalRead(pins[i])){
        Serial.print ("missing for third period ");Serial.print (pens[i]);Serial.print ("  at  "); Serial.println (i);
        Particle.publish("Third Warning",(String)pens[i]);
               // vibrate(500);
    }
      }
}

// void vibrate(int dur){
//     digitalWrite(D6,HIGH);
//     delay(dur);
//     digitalWrite(D6,LOW);
// }
