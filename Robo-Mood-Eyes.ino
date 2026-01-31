#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <FluxGarage_RoboEyes.h>

// --- SETTINGS ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHTPIN 2        
#define DHTTYPE DHT11   

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

unsigned long sensorTimer = 0;
const long sensorInterval = 2000; 
int lastMoodState = -1; 
bool isSweating = false; // Track sweating state

void setup() {
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED Error"));
    for(;;); 
  }

  dht.begin();
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);
  
  // v1.1.2 Alive behaviors
  roboEyes.setAutoblinker(true, 3, 2); 
  roboEyes.setIdleMode(true, 2, 2);    
  
  roboEyes.open();
  Serial.println(F("RoboEyes Humidity & Temp System Online"));
}

void loop() {
  roboEyes.update(); 

  if(millis() - sensorTimer >= sensorInterval) {
    sensorTimer = millis();
    
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
      Serial.println(F("DHT Error: Check Pins"));
      return; 
    }

    // --- HUMIDITY "SWEAT" LOGIC ---
    // If humidity is high (> 70%), make eyes sweat
    if (h > 70.0) {
      if (!isSweating) {
        roboEyes.setSweat(true);
        isSweating = true;
        Serial.println(F("It's too humid! Sweating..."));
      }
    } else {
      if (isSweating) {
        roboEyes.setSweat(false);
        isSweating = false;
        Serial.println(F("Humidity dropped. Stopped sweating."));
      }
    }

    // --- TEMPERATURE MOOD LOGIC ---
    if (t < 25.0) {
      if (lastMoodState != 0) {
        roboEyes.setMood(TIRED); // Tired acts as Sad in v1.1.2
        lastMoodState = 0;
        sendSerialUpdate(t, h, "SAD/TIRED");
      }
    } 
    else if (t >= 25.0 && t <= 30.0) {
      if (lastMoodState != 1) {
        roboEyes.setMood(HAPPY);
        roboEyes.anim_laugh(); 
        lastMoodState = 1;
        sendSerialUpdate(t, h, "HAPPY & LAUGHING");
      }
    } 
    else if (t > 30.0) {
      if (lastMoodState != 2) {
        roboEyes.setMood(ANGRY);
        lastMoodState = 2;
        sendSerialUpdate(t, h, "ANGRY");
      }
    }
  }
}

void sendSerialUpdate(float temp, float hum, String mood) {
  Serial.print(F("T: "));
  Serial.print(temp);
  Serial.print(F("C | H: "));
  Serial.print(hum);
  Serial.print(F("% | Mood: "));
  Serial.println(mood);
}
