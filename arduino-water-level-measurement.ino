#include <ArduinoJson.h>

#include <ESPAsyncTCP.h>
#include <tcp_axtls.h>
#include <async_config.h>
#include <AsyncPrinter.h>
#include <ESPAsyncTCPbuffer.h>
#include <SyncClient.h>
#include <DebugPrintMacros.h>

#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <SPIFFSEditor.h>
#include <WebHandlerImpl.h>
#include <ESPAsyncWebServer.h>
#include <WebAuthentication.h>
#include <AsyncWebSynchronization.h>
#include <AsyncWebSocket.h>
#include <WebResponseImpl.h>
#include <StringArray.h>

#include <ESP8266WiFi.h>
#include <NewPing.h>

const boolean DEBUG = true;

//#define echo D7 // Echo Pin
//#define trigger D6 // Trigger Pin
const int trigger = 12;
const int echo = 13;
#define MAX_DISTANCE 100
// Measurement
const int num_measurements = 2;
// WLAN
const char* ssid = "WLAN";
const char* password = "000000000";
long liter = 0;
char* picture = "";

NewPing sonar(trigger, echo, MAX_DISTANCE);

AsyncWebServer server(80);

/*
   Some functions:
   getLiter() -> Get liter in tank
   getPicture() -> Get corresponding picture
*/

//Compute volume displayed on website
String getLiter() {
  int distance = 0;

  // Measure multiple Times in order to
  // achieve a more exact result
  if (DEBUG) {
    Serial.println("Start Measurement!");
  }


  //********************************************
  // Check if measurement is in eligible region
  //********************************************

  while (distance == 0) {
    delay(50);
    distance = sonar.ping_cm();
  }

  // Check that distance is not greater then cistern
  // height or distance is negative
  liter = (MAX_DISTANCE - distance) * 23.36;


  if (liter < 0.)
  {
    liter = 0.;
  } else if (liter > 5000) {
    liter = 5000.;
  }

  if (DEBUG) {
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.println("Liter:");
    Serial.println(liter);
  }

  return String(liter);
}

// Choose which image to show
String getImage() {
  if (liter >= 0 && liter < 500)
  {
    // 0%
    picture = " <path fill='currentColor' d='M16,20H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 500  && liter < 1000)
  {
    // 10%
    picture = " <path fill='currentColor' d='M16,18H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 1000 && liter < 1500)
  {
    // 20%
    picture = "<path fill='currentColor' d='M16,17H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 1500 && liter < 2000)
  {
    // 30%
    picture = "   <path fill='currentColor' d='M16,15H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 2000 && liter < 2500)
  {
    // 40%
    picture = "<path fill='currentColor' d='M16,14H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 2500 && liter < 3000)
  {
    // 50%
    picture = "<path fill='currentColor' d='M16,13H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 3000 && liter < 3500)
  {
    // 60%
    picture = "<path fill='currentColor' d='M16,12H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 3500 && liter < 4000)
  {
    // 70%
    picture = "<path fill='currentColor' d='M16,10H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 4000 && liter < 4500)
  {
    // 80%
    picture = "<path fill='currentColor' d='M16,9H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 4500 && liter <= 4800)
  {
    // 90%
    picture = "<path fill='currentColor' d='M16,8H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else // Maximum volume is 5000 liter
  {
    // 100%
    picture = "<path fill='currentColor' d='M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }

  if (DEBUG) {
    Serial.println(picture);
  }
  return String(picture);
}

// Placeholder in HTML Code
String colldata(const String& var) {
  if (DEBUG) {
    Serial.println(var);
  }
  if (var == "LITER") {
    return getLiter();
  }
  if (var == "BAT") {
    return getImage();
  }
  return "";
}

void setup() {
  Serial.begin(115200);
  delay(10);

  // Starte SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  // Define fixed IP
  //IPAddress ip(192, 168, 178, 111);
  //IPAddress gateway(192, 168, 178, 1);
  IPAddress ip(192, 168, 2, 123);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);

  // Connect to WiFi network
  WiFi.config(ip, gateway, subnet);

WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  Serial.println("Server started");

  // Print the IP address
  Serial.println("=============================");
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  Serial.print("Local IP:");
  Serial.println(WiFi.localIP());
  Serial.print("Local Gateway:");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Local Subnet:");
  Serial.println(WiFi.subnetMask());
  Serial.println("=============================");

  // Load Website
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, colldata);
  });

  // Load CSS Style
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Send volume to website
  server.on("/LITER", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getLiter().c_str());
  });

  // Send SVG code of image to website
  server.on("/BAT", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getImage().c_str());
  });

  if (DEBUG) {
    Serial.println("Start Server!");
  }
  // Start server
  server.begin();
}

void loop() {
  // Debugger
  //delay(2000);
  //getLiter();
  /*
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  */
}
