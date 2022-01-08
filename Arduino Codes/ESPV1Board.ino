#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ThingSpeak.h"
SoftwareSerial espSerial(3, 1);

#define ONE_WIRE_BUS 0

#define WIFI_SSID "THK"                 //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "2020thxx"       //Password of your wifi network    
                     
#define FIREBASE_HOST "samms-ab9b9-default-rtdb.asia-southeast1.firebasedatabase.app"   
#define FIREBASE_AUTH "72gIIYjwmmn3CnFuovXLG6ICz6iIyUCy3cebPKm8"

FirebaseData firebaseData;  // Declare the Firebase Data object in the global scope

const long utcOffsetInSeconds = 19800;  //time setting for SriLanka

unsigned long myChannelNumber = 1526630;                //thingspeak channel ID
const char * myWriteAPIKey = "LPN1132KXS3MI7X7";        //thingspeak write API
// Declare global variable to store values
int ldrStat=0;
float temp = 0;
float TH = 32.0;
float TL = 25.0;
int ldrThreshold = 30;          //initial light intensity
int ldrStat1 = LOW;              //Light Intensity Detected by LDR 1
int ldrStat2 = LOW;              //Light Intensity Detected by LDR 2
int ldrStat3 = LOW;              //Light Intensity Detected by LDR 3   
boolean manMode = false; 
boolean OneSensor = false;               
int snd = 0;
String  i;


WiFiServer server(80);      // http server
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiUDP ntpUDP;             // for NTP
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup() {

  i = "";
  pinMode(A0, INPUT);     //LDR
   
  // initialize digital 10,11,22 as an outputs
  pinMode(16, OUTPUT);    //Lamp      D0
  pinMode(5, OUTPUT);    //Heater     D1
  pinMode(4, OUTPUT);    //Cooler     D2
  pinMode(0, INPUT);    //TempSensor  D3
 
  digitalWrite(16, LOW); 
  digitalWrite(5, LOW); 
  digitalWrite(4, LOW); 
  
  Serial.begin(9600);                               
  WiFi.disconnect();
  delay(3000);        
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);               //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                 //print local IP address
  
  //delay(1000);
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
  Firebase.reconnectWiFi(true);
  server.begin();
  
  
}

void loop() { 
 
    WifiCommunications();
    ThnigspeakWrite();
    SndSensDtoRTD();
    LightControl();
    TempControl();
	RecvDatafFirebase();
    
}

void RecvDatafFirebase(){
	String path = "/person";
	FirebaseObject obj = Firebase.get(path);
	String phNum = obj.getString("pNum");
	
	espSerial.println(phNum);
    delay(100); 
}


void LightControl(){

  String str ;
  if (Serial.available()) {
		str = Serial.read();
		ldrStat1 = String(str.at(0))+String(str.at(1));
		ldrStat2 = String(str.at(2))+String(str.at(3));
		ldrStat3 = String(str.at(4))+String(str.at(5));
		
  }
  ldrStat = (ldrStat1 + ldrStat2 + ldrStat3)/3;
    
  if (manMode == false){
       if (ldrStat1 <= ldrThreshold && ldrStat2 <= ldrThreshold && ldrStat3 <= ldrThreshold) {
            digitalWrite(16, HIGH);            
       } 
       else {
            digitalWrite(16, LOW);
       }
  }
        
}


void WifiCommunications(){
    WiFiClient client = server.available();
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    i = (client.readStringUntil('\r'));
    client.flush();
    i.remove(0, 5);
    i.remove(i.length()-9,9);
    if (i == "LOn") {
      digitalWrite(16,HIGH);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Lamp ON");
      client.println("</html>");
      delay(1);
      Serial.println("client1");
    }
    if (i == "LOff") {
      digitalWrite(16,LOW);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Lamp OFF");
      client.println("</html>");
      delay(1);
      Serial.println("client2");
    }
     
    if (i == "P") {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("ConnS");
      client.println("</html>");
      delay(1);
    }
    
   if (i == "Q") {
      TH = 33.0;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Temp High limit is Set");
      client.println("</html>");
      delay(1);
    }
  
  if (i == "R") {
      TL = 21.0;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Temp Low limit is Set");
      client.println("</html>");
      delay(1);
    }
  
  if (i == "S") {
      ldrThreshold = 70; 
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Light Intensity level is Set");
      client.println("</html>");
      delay(1);
    }

    if (i == "Xon") {
      manMode = true;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Manual Mode is On");
      client.println("</html>");
      delay(1);
    }
    if (i == "Xoff") {
      manMode = false;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Manual Mode is Off");
      client.println("</html>");
      delay(1);
    }

  
}

void TempControl(){

    if (temp > TH){
        digitalWrite(4, HIGH);  //Cooler
    }else digitalWrite(4, LOW);
    
    if (TL > temp){
        digitalWrite(5, HIGH);   //Heater
    }else digitalWrite(5, LOW);
  
}


void ThnigspeakWrite(){

       WiFiClient  client;         // for thingspeak
       ThingSpeak.begin(client);


  timeClient.begin();
            timeClient.update();
            int SndData = timeClient.getSeconds(); 
               
  timeClient.end();
  
  // write to the ThingSpeak channel
  if(SndData == 0){
       Serial.println(SndData); 
       ThingSpeak.setField(1, analogRead(A0));
       ThingSpeak.setField(2, analogRead(A0));
       int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
       if(x == 200){
          Serial.println("Channel update successful.");
       }
       else{
          Serial.println("Problem updating channel. HTTP error code " + String(x));
       }
       client.flush();
  }
  
}
 

void SndSensDtoRTD(){
  
    if (Firebase.setInt(firebaseData, "/LDR", ldrStat)) {    // On successful Write operation, function returns 1  
          
          delay(100);
    } else Serial.println(firebaseData.errorReason());

    if (Firebase.setInt(firebaseData, "/TEMP", temp)) {    // On successful Write operation, function returns 1  
         
          sensors.requestTemperatures();
          temp = sensors.getTempCByIndex(0);
         
          delay(10);
    } else Serial.println(firebaseData.errorReason());

   
    
}
