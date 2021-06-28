#include <ESP8266WiFi.h>

const char* ssid = "MyWlan";
const char* password = "000000000000";

WiFiServer server(80);

int trigger = 0;
int echo = 2;
long duration = 0;

long distance = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

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
  server.begin();
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


}

void loop() {

  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 100;

  Serial.println("=========== Loop ============");
  Serial.println(duration);
  Serial.println("=============================");

  String cmd;
  cmd += "\"";

  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
  client.println("<head>\n<meta charset='UTF-8'>");
  client.println("<title>Water Level</title>");
  client.println("</head>\n<body>");
  client.println("<H2>Water Level</H2>");
  client.println("<div>");

  client.println("<svg style='width:20%;height:20%' viewBox='0 0 24 24'> <path fill='currentColor' d='M16 20H8V6H16M16.67 4H15V2H9V4H7.33C6.6 4 6 4.6 6 5.33V20.67C6 21.4 6.6 22 7.33 22H16.67C17.41 22 18 21.41 18 20.67V5.33C18 4.6 17.4 4 16.67 4M15 16H9V19H15V16M15 7H9V10H15V7M15 11.5H9V14.5H15V11.5Z' /> </svg>");
  client.println("<div style='font-size:300%'>");
  client.println(duration);
  client.println("</div>");

  client.println("</div>");
  client.print("</body>\n</html>");
  delay(500);
}
