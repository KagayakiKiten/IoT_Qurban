#include <Wire.h>
#include <Adafruit_INA219.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


Adafruit_INA219 ina219;
int H = 15;
int N = 2;
int L = 32;
int LL = 33;
int button = 4;

const unsigned long event1 = 1000;
const unsigned long event2 = 3000;

unsigned long previoustime1 = 0;
unsigned long previoustime2 = 0;

unsigned long turnon = 0;
