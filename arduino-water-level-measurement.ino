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

//#define echo D7 // Echo Pin
//#define trigger D6 // Trigger Pin
const char* ssid = "MyWlan";
const char* password = "000000000000";

int trigger = 12;
int echo = 13;

long duration = 0;
long distance = 0;
long liter = 0;

char* picture = "";

AsyncWebServer server(80);
//WiFiServer server(80);

/*
 * Some functions:
 * getLiter() -> Get liter in tank
 * getPicture() -> Get corresponding picture
 */

//Berechnung der Liter für die Webseite
String getLiter() {
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  // Prüfen ob Messwert zwischen 200cm und 0cm liegt
  distance = (duration / 2) / 29.1;
    if (distance >= 200 || distance <= 0)
  {
    distance = 0;
  }
  else
  {
    liter = 199 - distance;
    liter = liter * 84.7457;
  }
  Serial.println(String(liter));
  return String(liter);
}

// Choose which image to show
String getImage() {
  if (liter >= 0 && liter <= 1600)
  {
    // 0%
    picture = " <path fill='currentColor' d='M16,20H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 1600 && liter <= 3200)
  {
    // 10%
    picture = " <path fill='currentColor' d='M16,18H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 3200 && liter <= 4800)
  {
    // 20%
    picture = "<path fill='currentColor' d='M16,17H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 4800 && liter <= 6400)
  {
    // 30%
    picture = "   <path fill='currentColor' d='M16,15H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 6400 && liter <= 8000)
  {
    // 40%
    picture = "<path fill='currentColor' d='M16,14H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 8000 && liter <= 9600)
  {
    // 50%
    picture = "<path fill='currentColor' d='M16,13H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 9600 && liter <= 12000)
  {
    // 60%
    picture = "<path fill='currentColor' d='M16,12H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 112000 && liter <= 128000)
  {
    // 70%
    picture = "<path fill='currentColor' d='M16,10H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  else if (liter >= 12800 && liter <= 144000)
  {
    // 80%
    picture = "<path fill='currentColor' d='M16,9H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  /*
  else if (liter >= 14400 && liter <= 16000)
  {
    // 90%
    picture = "<path fill='currentColor' d='M16,8H8V6H16M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  */
  else if (liter >= 14400 && liter <= 16000)
  {
    // 100%
    picture = "<path fill='currentColor' d='M16.67,4H15V2H9V4H7.33A1.33,1.33 0 0,0 6,5.33V20.67C6,21.4 6.6,22 7.33,22H16.67A1.33,1.33 0 0,0 18,20.67V5.33C18,4.6 17.4,4 16.67,4Z' />";
  }
  Serial.println(picture);
  return String(picture);
}

// Placeholder in HTML Code
String colldata(const String& var) {
  Serial.println(var);
  // TODO: Fix bug in String cast!
  /*
  if (var == "LITER") {
    return getLiter();
  }
  */
  if (var == "BAT") {
    return getImage();
  }
  return "";
}

// Printing whole webpage
// For debugging
/*
void webPageprint() {
  WiFiClient client = server.available();
  client.println("<!DOCTYPE html>");
  client.println("<head>\n<meta charset='UTF-8'>");
    // Compatibility for mobile devices

    client.println("<meta name='viewport' content='width=device-width, initial-scale=1' />");
    client.println("<title>Water Level</title>");
    client.println("<style>");
      client.println(" .container { display: flex; align-items: center; justify-content: center } img { max-width: 100% } .image { flex-basis: 40% } .text { font-size: 20px; padding-left: 20px; } ");
    client.println("</style>");
  client.println("</head>\n<body>");
  client.println("<H2>Water Level</H2>");
  client.println("<div class='container'>");
    client.println("<div class='image'>");
      client.println("<svg style='width:20%;height:20%' viewBox='0 0 24 24'> <path fill='currentColor' d='M16 20H8V6H16M16.67 4H15V2H9V4H7.33C6.6 4 6 4.6 6 5.33V20.67C6 21.4 6.6 22 7.33 22H16.67C17.41 22 18 21.41 18 20.67V5.33C18 4.6 17.4 4 16.67 4M15 16H9V19H15V16M15 7H9V10H15V7M15 11.5H9V14.5H15V11.5Z' /> </svg>");
    client.println("</div>");
    client.println("<div class='text'>");
      client.println("<h1>");
        client.print(duration);
      client.println("</h1>");
    client.println("</div>");
  client.println("</div>");
  client.print("</body>\n</html>");
}
*/

void setup() {
  Serial.begin(115200);
  delay(10);

  // Starte SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //-----------
  // Debugging
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //-----------
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  // Define fixed IP
  IPAddress ip(192,168,2,123);
  IPAddress gateway(192,168,2,1);
  IPAddress subnet(255,255,255,0);

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
  //server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println("=============================");
  Serial.println("Local IP:");
  Serial.println(WiFi.localIP());
  Serial.println("Local Gateway:");
  Serial.println(WiFi.gatewayIP());
  Serial.println("Local Subnet:");
  Serial.println(WiFi.subnetMask());
  Serial.println("=============================");

  // Webseite Laden
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, colldata);
  });

  // CSS Datei Laden
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  // Liter an Webseite übergeben
  server.on("/LITER", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getLiter().c_str());
  });

  // Bild an Webseite übergeben
  server.on("/BAT", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", getImage().c_str());
  });
  // Start server
  server.begin();
}


void loop() {
/*
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2.) / 100.;

  Serial.println("=========== Loop ============");
  Serial.println(duration);
  Serial.println("=============================");

  String cmd;
  cmd += "\"";


  // webPagePrint();

  delay(500);
*/
  // Debugger
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
