// Add these libraries in your Arduino libraries folder
#include <U8glib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include<Wire.h>
//#include <GPRS_Shield_Arduino.h>
//#include <sim900.h>

U8GLIB_SH1106_128X64 u8g(12, 11, 8, 9, 10); // (D0, D1, CS, DC, RES)

SoftwareSerial Genotronex(3, 4); // RX, TX - For Bluetooth
SoftwareSerial gsm(5,6); // RX, TX - For GSM Module

//If GSM library is used (Otherwise use AT commands)
/*int rx = 5;
int tx = 6;
char PhoneNum[] = "7092345559";
char message1[] = "Woah chill out";
char message2[] = "Woah peace out";
int br = 9600;
GPRS gsm(rx,tx,br);*/

//Variables for pulse sensing
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

//For temperature sensor
OneWire oneWire(7); 
DallasTemperature sensors(&oneWire);
float t = 0;

int flag = 0;

void setup()
{
  Serial.begin(115200);// we agree to talk fast!
  gsm.begin(9600);
  //If gsm library is used
  /*while (!gsm.init())
  {
    delay(500);
    Serial.println("GPS not connected");
  }*/
  Serial.println("GPS connected");
  u8g.setFont(u8g_font_unifont); //Sets font for the OLED.
  u8g.setColorIndex(1); //Instructs the display to draw with a pixel on. 
  sensors.begin(); // Start up the temperature sensor library.
  interruptSetup();// Sets up to read Pulse Sensor signal every 2mS  
  Genotronex.begin(38400);
}


//  Where the Magic Happens
void loop()
{ 
  Serial.println(flag);
  if (QS == true)
  {  
    //  A Heartbeat Was Found.
    sensors.requestTemperatures(); // Send the command to get temperature.
    t = sensors.getTempCByIndex(0); // Gets temperature in deg C.
    t = (t*1.8)+32; // Converts temperature fron C to F.
    serialOutputWhenBeatHappens(); // A Beat Happened, Output that to serial.     
    QS = false;                      // Reset the Quantified Self flag for next time.
  }
  
  if (t > 89) //Set any temperature for demonstration (but above 98 F is considered fever).
  {
    u8g.firstPage(); 
    do 
    {  
      draw2(); // Prints warning on OLED screen.
    } 
    while( u8g.nextPage() );
    if (flag == 0)
    {
      //Send emergency messagase to choosen number/numbers.
      gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode.
      delay(1000);  // Delay of 1 second.
      gsm.println("AT+CMGS=\"+917092345559\"\r"); // Replace with mobile number.
      delay(1000);
      gsm.println("Chill out");// The SMS text you want to send.
      delay(100);
      gsm.println((char)26);// ASCII code of CTRL+Z.
      delay(1000);
      //If using GSM library.
      /*gsm.sendSMS(PhoneNum, message1);*/
      Serial.println("Sending message");
    }
    flag = 1;
  }
  
  else if (BPM > 300) //Set any BPM for demonstration (but above 100BPM is considered too fast).
  {
    
    u8g.firstPage();
    do 
    {  
      draw3(); //Prints warning on OLED.
    } 
    while( u8g.nextPage() );
    if (flag == 0);
    {
      //Send emergency messagase to choosen number/numbers.
      gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode.
      delay(1000);  // Delay of 1 second.
      gsm.println("AT+CMGS=\"+917092345559\"\r"); // Replace with mobile number.
      delay(1000);
      gsm.println("Peace out");// The SMS text you want to send.
      delay(100);
      gsm.println((char)26);// ASCII code of CTRL+Z.
      delay(1000);
      //If using GSM library
      /*gsm.sendSMS(PhoneNum, message2);*/
      Serial.println("Sending message");
    }
    flag = 1;
  }
  else
  {
    flag = 0;
    u8g.firstPage();
    do 
    {  
      draw(); //Prints BPM and Temperature on screen.
    } 
    while( u8g.nextPage() );
   }
  delay(800); //Take a break.
}










