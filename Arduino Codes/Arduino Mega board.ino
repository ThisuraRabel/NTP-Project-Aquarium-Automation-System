
#include <Wire.h>                  //For OLED
#include <Adafruit_GFX.h>          //For OLED
#include <Adafruit_SSD1306.h>      //For OLED
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3232RTC.h>
#include <SoftwareSerial.h>
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define ONE_WIRE_BUS 22


const long utcOffsetInSeconds = 19800; 
int Wlvl=0;
int WLvl1 = 0;
int WLvl2 = 0;
int WLvl3 = 0;
int Hh, Hm, Hs, Lh, Lm, Ls;
float PH;
int LI;
boolean highlevelZero = false;
boolean lowlevelZero = false; 
boolean WarningRst = false;
boolean PHPf = false;
boolean Cf = false;
boolean Hf = false;
boolean tmpSf = false;
boolean PHSf = false;
int PHPfH, CfH, HfH, tmpSfH, PHSfH;
int GSMtimeout;
String GSMbuffer;
String number; // assign PNumber from DB
float calibration_value = 21.34 + 1.57;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

float ph_act;


SoftwareSerial sim(2, 3);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  
    
    pinMode(A0, INPUT);    // PH
    pinMode(A1, INPUT);    // WL1 
    pinMode(A2, INPUT);    // WL2 
    pinMode(A3, INPUT);    // WL3 
    pinMode(A7, INPUT);    // LDR1
	pinMode(A8, INPUT);    // LDR2
	pinMode(A9, INPUT);    // LDR3
    
    pinMode(22, INPUT);    // TempSensor
    pinMode(10, INPUT);    // PH controller
    pinMode(11, INPUT);    // Heater
    pinMode(12, INPUT);    // Cooler
  
    pinMode(8, OUTPUT);    // Speaker
    digitalWrite(8, LOW);  // Speaker


    Serial.begin(9600);   // initialize serial for debugging
    

    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet)
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");



    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
  
}

void loop() {
 
   WLeakageCheck();
   WLCheck();
   GetPH();
   GetLI();
   GetTmp();
   OLEDdispay();

}

void OLEDdispay(){
  
    display.clearDisplay();
    
    display.setCursor(10, 0);
    display.print("TE: ");
    display.print(temp);
    display.println("`C");
    
    display.setCursor(10, 20);
    display.print("PH: ");
    display.println(ph_act);
    
    display.setCursor(10, 40);
    display.print("LI: ");
    display.print(LI);
    display.println("%");
    
    display.display();
    
}

void GetPH(){
  
    
    if (PHPf == false){
         if (digitalRead(10) == 0) SendMessage(3);
         PHPfH = hour();
         PHPf = true;
    }
    if (PHPfH + 5 == hour()){
         PHPf = false;
    }
     

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
    ph_act = -5.70 * volt + calibration_value;

   
     if (PHSf == false){
         if (ph_act > 14 || ph_act < 0) SendMessage(7);
         PHSfH = hour();
         PHSf = true;
    }
    if (PHSfH + 5 == hour()){
         PHSf = false;
    }
    
     
}

void GetLI(){

    LI = (((analogRead(A7)+analogRead(A8)+analogRead(A9))/3)/75)*10;
	
	String str = String((analogRead(A7)/75)*10) + String((analogRead(A8)/75)*10) + String((analogRead(A9)/75)*10); 
	
	espSerial.println(str);
		delay(100);
    
}

void GetTmp(){
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    Serial.println(temp);

    boolean Cf = false;
    boolean Hf = false;
 
  
    if (tmpSf == false){
         if (temp > 70 || temp < -25) SendMessage(6);
         tmpSfH = hour();
         tmpSf = true;
    }
    if (tmpSfH + 5 == hour()){
         tmpSf = false;
    }
    
    if (Hf == false){
         if (digitalRead(11) == 0) SendMessage(5);
         HfH = hour();
         Hf = true;
    }
    if (HfH + 5 == hour()){
         Hf = false;
    }

    if (Cf == false){
         if (digitalRead(12) == 0) SendMessage(4); 
         CfH = hour();
         Cf = true;
    }
    if (CfH + 5 == hour()){
         Cf = false;
    }
    
}

void WLCheck(){

    WLvl1 = analogRead(A3);
    WLvl2 = analogRead(A2);
    WLvl3 = analogRead(A1);   
	
      
    if (WLvl3 >= 270 && WLvl2 >= 270 && WLvl1 >= 270){
      Wlvl = 3;
      WarningRst = true;
    }
    
    if (WLvl3 <= 270 && WLvl2 >= 270 && WLvl1 >= 270) Wlvl = 2;
    if (WLvl3 <= 270 && WLvl2 <= 270 && WLvl1 >= 270) Wlvl = 1;
    
    if(WLvl3 <= 270 && WLvl2 <= 270 && WLvl1 <= 270){
      Wlvl = 0;
      SendMessage(2);
      WarningRst = false;
      alertSpeaker();
    }
     
}

void WLeakageCheck(){

    int highlevel, lowlevel;
    if(WarningRst == true){
        if (highlevelZero == false){
            if (Wlvl == 2){
                 
                highlevelZero = true;
                Hh = hour();
                Hm = minute();
                Hs = second(); 
            }     
         }  
         if (highlevelZero == true){
            if (Wlvl == 1){  
                Lh = hour();
                Lm = minute();
                Ls = second();
                long int HTime = Hh * Hm * Hs;
                long int LTime = Lh * Lm * Ls;
                if ((LTime - HTime) <= 3600){
                   WarningRst = false;
                   SendMessage(1);
                   alertSpeaker();
                   Serial.println(LTime - HTime);
                }  
                highlevelZero = false;      
            }    
            
         }
    }
}

void alertSpeaker(){
    while(WarningRst == false){
       tone(8, 1000, 500);
       delay (800);
       tone(8, 1000, 500);
       delay (800);
    }     
}

void SendMessage(int p){

  if (Serial.available()) {
	  number = Serial.read();
  }
  
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS;
  if (p == 1)  SMS = "Water Leakage Detected !!!";
  if (p == 2)  SMS = "Water Level is Dangerously low !!!";
  if (p == 3)  SMS = "PH controlling pump faliure !!!";
  if (p == 4)  SMS = "Cooler faliure !!!";
  if (p == 5)  SMS = "Heater faliure !!!";
  if (p == 6)  SMS = "Temperature sensor is not working properly !!!";
  if (p == 7)  SMS = "PH sensor is not working properly !!!";
  
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  GSMbuffer = readSerial();
}

String readSerial() {
  GSMtimeout = 0;
  while  (!sim.available() && GSMtimeout < 12000  )
  {
    delay(13);
    GSMtimeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
