#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define trig1 12
#define echo1 14
#define trig2 26
#define echo2 25

// variable for calcurate speed
unsigned long startTime = 0;
unsigned long endTime = 0;
int distance1, distance2;
unsigned long previousMillis = 0;
const long interval = 100;
int status = 0;
bool unpreson = false;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// define for send data 2 db
const char* ssid     = "LENOVO3";
const char* password = "netnut00";
String apiKeyValue = "";
float value1=0;

void send2db(float speed){
  const char* serverName = "http://prepro3.informatics.buu.ac.th:8058/speedDbwrite.php";
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(serverName);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Prepare your HTTP POST request data
  String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + speed;
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);
 
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Hello, world!");
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
}

float calculateSpeed(unsigned long elapsedTime, int distance) {
  // Calculate speed using the formula v = s/t
  // Convert distance from cm to m for consistency with time (which is in milliseconds)
  float speed = (float) distance / ((float) elapsedTime / 1000); // Convert time from milliseconds to seconds
  return speed;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    digitalWrite(trig1, LOW);
    delayMicroseconds(2);
    digitalWrite(trig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig1, LOW);

    int duration1 = pulseIn(echo1, HIGH);
    distance1 = duration1 * 0.034 / 2;

    digitalWrite(trig2, LOW);
    delayMicroseconds(2);
    digitalWrite(trig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig2, LOW);

    int duration2 = pulseIn(echo2, HIGH);
    distance2 = duration2 * 0.034 / 2;

    // Serial.print("Distance 1: ");
    // Serial.print(distance1);
    // Serial.println(" cm");

    // Serial.print("Distance 2: ");
    // Serial.print(distance2);
    // Serial.println(" cm");

    if (distance1 < 20 && status == 0) {
      status = 1;
      startTime = millis();
    }

    if (distance2 < 20 && status == 1) {
      status = 0;
      endTime = millis();
      unsigned long elapsedTime = endTime - startTime;
      float speed = calculateSpeed(elapsedTime, distance2);
      Serial.print("Speed: ");
      Serial.print(speed);
      Serial.println(" m/s");

      if(WiFi.status()== WL_CONNECTED){
        send2db(speed);
      }
      else {
      Serial.println("WiFi Disconnected");
      }

      // Check if speed exceeds 30 m/s
      if (speed >= 10) {
        // Serial.println("Speed limit exceeded!");
        // lcd.setCursor(0, 1);
        // lcd.print("Speed Exceeded!");
        unpreson = true;
      } 

      else {
        lcd.setCursor(0, 1);
        lcd.print("Speed: ");
        lcd.print(speed);
        lcd.print(" m/s");
      }
    }
  }

  if (unpreson){
    unpreson = false;
    while (!(Serial.available() > 0)){
      ;
      }
    String data = Serial.readString(); // Read data from Serial monitor as a String
    Serial.println("Received data: " + data); // Print the received data to the Serial monitor
    char c = Serial.read();
    }
}
