#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "test";
const char *password = "test1234";
const String host = "https://emg-monitor.onrender.com/";
const String id = "EMGDev1";

const int emgPin = A0; 
const int httpsPort = 8000;
const int size = 1000;
int cnt;

static String data = "{";
HTTPClient http;    //Declare object of class HTTPClient

WiFiClientSecure client;
String url=host+"data/"+id;

void setup() {
  // put your setup code here, to run once:
  pinMode(emgPin, INPUT);
  
  delay(1000);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  cnt = 0;
  client.setInsecure(); //the magic line, use with caution
  // client.connect(host, httpsPort);
}

void loop() {
  // Serial.print("In loop\n");
  int ang_val = analogRead(emgPin);
  // Serial.print("EMG_Val:");
  // Serial.println(ang_val);
  if(cnt == size-1) {    
    data += "\"" + String(size-1) + "\"" + ":";
    data += "\"" + String(ang_val) + "\"";
    data += "}";
    Serial.println("Done");
    http.begin(client, url); 
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(data);
    Serial.println(httpResponseCode);
    data = "{";
  }
  else {
    data += "\"" + String(cnt) + "\"" + ":";
    data += "\"" + String(ang_val) + "\",";
  }
  cnt++;
  cnt %= size;
  delayMicroseconds(1000);  
}
