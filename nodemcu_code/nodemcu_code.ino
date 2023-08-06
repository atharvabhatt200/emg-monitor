#include <ESP8266WiFi.h>
// #include <WiFiClientSecure.h>
// #include <WiFiClient.h>
// #include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "test";
const char *password = "test1234";
const String host = "https://emg-monitor.onrender.com/";
const String id = "EMGDev1";

const int emgPin = A0; 
const int httpsPort = 8000;
const int size = 24000;
uint16_t arr[size];
// WiFiClient wifiClient;
int cnt;

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
}

void loop() {
  // Serial.print("In loop\n");
  arr[cnt]= analogRead(emgPin);
  // Serial.println(String(ESP.getFreeHeap()));
  // Serial.print(String(analogInput)+"\n");
  // = analogInput;

  if(cnt == size-1) {
    int sized = 1000;
    
    Serial.println("Done");
    for(int itr = 0; itr < 24; itr++) {
      //Post Data
      HTTPClient http;    //Declare object of class HTTPClient

      WiFiClientSecure client;
      // WiFiClient client;
      String url=host+"data/"+id;
      // Serial.print(url+"\n");

      client.setInsecure(); //the magic line, use with caution
      client.connect(host, httpsPort);

      http.begin(client, url); 

      http.addHeader("Content-Type", "application/json");
      String data = "{";
      for(int i = 0; i <= sized-2; i++) {
        data += "\"" + String(i) + "\"" + ":";
        data += "\"" + String(arr[itr*1000 + i]) + "\",";
      }
      data += "\"" + String(sized-1) + "\"" + ":";
      data += "\"" + String(arr[itr*1000 + size-1]) + "\"";
      // data[len(data)-1] = '}';
      data += "}";
      // Serial.println(data);

      int httpResponseCode = http.POST(data);

      // int httpCode = http.GET();   //Send the request
      Serial.println(String(httpResponseCode));   //Print HTTP return code

      http.end();  //Close connection
      // delay(5000);
    }
  }
  cnt++;
  cnt %= size;
  delayMicroseconds(42);  //Post Data at every 0.1 seconds
}
