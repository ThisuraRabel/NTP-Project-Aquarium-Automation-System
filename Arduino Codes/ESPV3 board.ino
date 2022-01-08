#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>                 //For Servo Motor
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ThingSpeak.h"
                          
#define FIREBASE_HOST "samms-ab9b9-default-rtdb.asia-southeast1.firebasedatabase.app"   
#define FIREBASE_AUTH "72gIIYjwmmn3CnFuovXLG6ICz6iIyUCy3cebPKm8"

#define WIFI_SSID "THK"                 //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "2020thxx"       //Password of your wifi network 

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

const long utcOffsetInSeconds = 19800;  //time setting for SriLanka

unsigned long myChannelNumber = 1526630;                //thingspeak channel ID
const char * myWriteAPIKey = "LPN1132KXS3MI7X7";        //thingspeak write API

// Declare global variable to store value
float PHStat;
String  i;
float PHhigh = 24.0;
float PHlow = 1.0;
float calibration_value = 21.34 + 10.7;
unsigned long int avgval; 
int buffer_arr[10],temp;
int feedAmt;
int fTimes;
boolean doFeed = false;

Servo servo;                     
WiFiServer server(80);
WiFiUDP ntpUDP;             // for NTP
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {

  i = "";
 
  pinMode(A0, INPUT);  //PH sensor
  
  // initialize digital 10,11,22 as an outputs
 
  pinMode(5, OUTPUT);    // PH inc D1
  pinMode(4, OUTPUT);    // PH dec D2
  pinMode(0, OUTPUT);    // Servo  D3

  analogWrite(A0, LOW); 
  digitalWrite(5, LOW); 
  digitalWrite(4, LOW); 
  digitalWrite(0, LOW); 
  
  servo.attach(0);   // Set digital pin 3 to generate servo motor signal

  Serial.begin(9600); // Select the same baud rate if you want to see the datas on Serial Monitor
  WiFi.disconnect();
  delay(3000);        
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);     
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
  Serial.println(WiFi.localIP());     //print local IP address
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
  Firebase.reconnectWiFi(true);
  server.begin();
  
}

void loop() { 

    SndSensDtoRTD();
    PHControl();
    Feed();
    WifiCommunications();
    //ThnigspeakWrite();
}

void WifiCommunications(){
    WiFiClient client = server.available();
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    i = (client.readStringUntil('\r'));
    client.flush();
    i.remove(0, 5);
    i.remove(i.length()-9,9);
    if (i == "CN") {
     
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("ConnS");
      client.println("</html>");
      delay(1);
    }
  
  if (i == "fd") {
      doFeed = true;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("Feeding... ");
      client.println("</html>");
      delay(1);
      
    }
  
  if (i == "chF") {
      fTimes = 3;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("Feed times are Set");
      client.println("</html>");
      delay(1);
    }
  
  if (i == "PHH") {
      PHhigh = 6.0;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("PH high level is Set");
      client.println("</html>");
      delay(1);
    }
  if (i == "PHL") {
      PHlow = 5.0;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("PH low level is Set");
      client.println("</html>");
      delay(1);
    }
  
}


void PHControl(){

    for(int i=0;i<10;i++) { 
       buffer_arr[i]=analogRead(A0);
       delay(30);
    }
    for(int i=0;i<9;i++){
       for(int j=i+1;j<10;j++){
       if(buffer_arr[i]>buffer_arr[j]){
           temp=buffer_arr[i];
           buffer_arr[i]=buffer_arr[j];
           buffer_arr[j]=temp;
       }
     }
    }
    avgval=0;
    for(int i=2;i<8;i++) avgval+=buffer_arr[i];
    float volt=(float)avgval*5.0/1024/6; 
    PHStat = -5.70 * volt + calibration_value;


    if (PHStat > PHhigh){
        //ph low pump
        digitalWrite(4, HIGH);
        delay(5000); 
        digitalWrite(4, LOW);
        delay(5000); 
    }

    if (PHStat < PHlow){
       //ph high pump
        digitalWrite(5, HIGH);
        delay(5000); 
        digitalWrite(5, LOW);
        delay(5000); 
    }
   
}

void Feed(){
     if (doFeed == true){
        servo.write(0);    
        delay(1000);
        servo.write(180);    
        delay(1000);
     }   
}


void SndSensDtoRTD(){
  
    if (Firebase.setInt(firebaseData, "/PH", PHStat)) {    // On successful Write operation, function returns 1  
          Serial.print("PH = ");
          Serial.println(PHStat);
          
          delay(100);
    } else Serial.println(firebaseData.errorReason());
  
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
       ThingSpeak.setField(2, PHStat);
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
