 
// include library for sensor-specific functions
#include "SR04.h" // ultrasonic sensor library
#include "ThingSpeak.h"
#include <WiFi.h>

//-------- Enter Your Wi-Fi Details -----//
char ssid[] = "rohit roy’s iPhone";  //SSID change
char pass[] = "123456789"; //Password change
//--------------------------------------//

 
// set pins for ultrasonic sensors
//sensor 1//
#define trig_pin_1 4 // pin for sending sensor #1 trigger signal 
#define echo_pin_1 16 // pin for receiving sensor #1 echo signal

//sensor 2//
#define trig_pin_2 17  // pin for sending sensor #2 trigger signal 
#define echo_pin_2 5  // pin for receiving sensor #2 echo signal

//sensor 3//
#define trig_pin_3 13  // pin for sending sensor #3 trigger signal 
#define echo_pin_3 12  // pin for receiving sensor #3 echo signal

//sensor 4//
#define trig_pin_4 27  // pin for sending sensor #3 trigger signal 
#define echo_pin_4 26  // pin for receiving sensor #3 echo signal

WiFiClient  client;

unsigned long myChannelField1 = 2100807; // Channel 1 ID
const int ChannelField1 = 1; // sensor 1 To Field Write
const int ChannelField2 = 2; // sensor 2 To Field Write
const int ChannelField5 = 3; // sensor 3 To Field Write
const int ChannelField6 = 4; // sensor 4 To Field Write
const int ChannelField7 = 7;  // Newly added for sending average

// channel for alert 1
unsigned long myChannelField2 = 2109161; // alert 1 Channel 2 ID
const int ChannelField3 = 1; // alert 1 To Field Write

// channel for alert 2
unsigned long myChannelField3 = 2109165; // alert 2 Channel 3 ID

const int ChannelField4 = 1; // alert 2 To Field Write

const char * myWriteAPIKey1 = "9M7MQIRCYVEJUG2U"; // Write API Key channel 1

const char * myWriteAPIKey2 = "9AYFL2F7EZ9YAWVW"; // Write API Key channel 2

const char * myWriteAPIKey3 = "QK95UAHCIP398EF7"; // Write API Key channel 3

 
// initialize two sensor objects
SR04 sensor1 = SR04(echo_pin_1, trig_pin_1);  // sensor 1
SR04 sensor2 = SR04(echo_pin_2, trig_pin_2);  // second 2
SR04 sensor3 = SR04(echo_pin_3, trig_pin_3);  // sensor 3
SR04 sensor4 = SR04(echo_pin_4, trig_pin_4);  // second 4
 
// initialize variables
long d1;                  // sensor 1 distance                
long d2;                  // sensor 2 distance
long d3;                  // sensor 3 distance                
long d4;                  // sensor 4 distance
long d_avg;               // for average distance
long t1 = 0;              // sensor 1 timestamp; initialize as 0
long t2 = 0;              // sensor 2 timestamp; initialize as 0
long t3 = 0;              // sensor 3 timestamp; initialize as 0
long t4 = 0;              // sensor 4 timestamp; initialize as 0
unsigned long start_time; // time since program start
long timeIntervalLed1 = 1000; // time interval between two readings
float max_distance = 20;  // movement sensing range (in cm)
 
void setup() {
  Serial.begin(115200);     // initialize serial monitor at 115200 baud
  WiFi.mode(WIFI_STA);

if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  
  ThingSpeak.begin(client);
  delay(1000);            // pause
  start_time = millis();  // get program start time
  
}
 
void loop() {

 
  // get distance values from 4 sensors
  d1 = sensor1.Distance();
  d2 = sensor2.Distance();
  d3 = sensor3.Distance();
  d4 = sensor4.Distance();

  
  // d_avg = (d1+d2+d3+d4)/4;
 
    Serial.print("Distance in cm 1st sensor: ");
    Serial.println(d1);
    ThingSpeak.writeField(myChannelField1, ChannelField1, d1, myWriteAPIKey1); //sending to thingspeak
    delay(1000);
  
    Serial.print("Distance in cm 2nd sensor: ");
    Serial.println(d2);
    ThingSpeak.writeField(myChannelField1, ChannelField2, d2, myWriteAPIKey1); //sending to thingspeak
     delay(1000);
  
    Serial.print("Distance in cm 3rd sensor: ");
    Serial.println(d3);
    ThingSpeak.writeField(myChannelField1, ChannelField5, d3, myWriteAPIKey1); //sending to thingspeak
    delay(1000);
  
    Serial.print("Distance in cm 4th sensor: ");
    Serial.println(d4);
    ThingSpeak.writeField(myChannelField1, ChannelField6, d4, myWriteAPIKey1); //sending to thingspeak
     delay(1000);
  
    //For plotting average distance choose channel 7 in thingspeak
     Serial.print("Average distance in cm: ");
     Serial.println(d5);
     ThingSpeak.writeField(myChannelField1, ChannelField7, d5, myWriteAPIKey1); //sending to thingspeak
     delay(1000);  

    
  // if either sensor distance drops below limit, record timestamp
  if (d1 < max_distance) {
    t1 = millis();
  } 
  else if (d2 < max_distance) {
    t2 = millis();
  }

  else if (d3 < max_distance) {
    t3 = millis();
  }

   else if (d4 < max_distance) {
    t4 = millis();
  }
  
  if (t1 > 0 && t2 > 0 && t3 > 0 && t4 > 0) {       // if all sensors have nonzero timestamps
    
 truecondition();

  }

unsigned long currentTime = millis();

 if ((t1 > 0) && ((currentTime - t1) > timeIntervalLed1) && ((t2==0) || (t3==0) || (t4==0))) 

{t1=0;

int stringOne = 1;  // string to send message to thinkspeak
Serial.println("Alert not reached wash room");    // alert only sensor 1 is triggered
ThingSpeak.writeField(myChannelField2, ChannelField3, stringOne, myWriteAPIKey2);   //sending to thingspeak
delay(15000);

//exit (0);

    } 

     if ((t4 > 0) && ((currentTime - t4) > timeIntervalLed1) && ((t3==0) || (t2==0) || (t1==0)))

{t4=0;
int stringTwo = 1; 
Serial.println("Alert not returned to bed room");    // aler only sensor 2 is triggered
ThingSpeak.writeField(myChannelField3, ChannelField4, stringTwo, myWriteAPIKey3);    //sending to thingspeak
delay(15000);
//exit (0);

    }
}
  


void truecondition()
{
  
  if (t1 < t2 < t3 < t4) {                      // if lst sensor triggered first
      Serial.println("Alert Moved towards Wash Room");    // direction is sensor 1 to sensor 4
   delay (5000); } 
    else if (t4 < t3 < t2 < t1) {                 // if 4th sensor triggered first
      Serial.println("Alert Came Back Safely");    // direction is sensor 4 to sensor 1
     delay (5000); }
    //else {
      //Serial.println("");         // else print nothing (shouldn't happen)
    //}
 
    // reset both timestamps to 0
   t1 = 0;
   t2 = 0;
   t3 = 0;
   t4 = 0;
    //exit (0);
  }
