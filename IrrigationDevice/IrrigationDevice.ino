#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

const char *ssid = "";
const char *password = "";
const char *host = "https://irrigation-monitor-app.herokuapp.com/";
const String id = "IrrDev1";

#define DHTPIN 2
DHT dht(DHTPIN, DHT11);
const int moisturePin = A0; 
const int motorPin = 14; 
WiFiClient wifiClient;

void setup() {
  // put your setup code here, to run once:
  pinMode(DHTPIN, INPUT);
  pinMode(moisturePin, INPUT);
  pinMode(motorPin, OUTPUT);
  
   delay(1000);
  Serial.begin(115200);
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
}

void loop() {
  HTTPClient http;    //Declare object of class HTTPClient
  Serial.print("In loop");
  String ADCData, station, postData;
  int moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) );
  int h = dht.readHumidity();     // read humiduty
  int t = dht.readTemperature();     // read temperature
  if (isnan(h) || isnan(t))
  {
    Serial.print("Failed to read from DHT sensor!");
    h=0;
    t=0;
  }
  if (moisturePercentage < 50) {
    digitalWrite(motorPin, LOW);         // tun on motor
    Serial.print("MOTOR ON");
  }
  if (moisturePercentage > 50 && moisturePercentage < 55) {
    digitalWrite(motorPin, LOW);        //turn on motor pump
     Serial.print("MOTOR ON");
  }
  if (moisturePercentage > 56) {
    digitalWrite(motorPin, HIGH);          // turn off mottor
    Serial.print("MOTOR OFF");
  }
  //Post Data
  String Data = "moisture_level=" + String(moisturePercentage) + "&temperature=" + String(t) + "&humidity=" + String(h) ;
  Serial.print("\nhttp://irrigation-monitor-app.herokuapp.com/data/"+id+"/"+String(moisturePercentage)+"/"+String(t)+"/"+String(h)+"\n");
  http.begin(wifiClient,"http://irrigation-monitor-app.herokuapp.com/data/"+id+"/"+String(moisturePercentage)+"/"+String(t)+"/"+String(h)); 

  int httpCode = http.GET();   //Send the request
  if(httpCode == 300){
    Serial.print("Manual MOTOR ON");
    digitalWrite(motorPin, LOW); 
  }
  Serial.println(httpCode);   //Print HTTP return code

  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}
