////////////////////////////////////////////////////////////////
// define constants and include libraries
////////////////////////////////////////////////////////////////


// define PINs
#define echoPin D7 // Echo Pin
#define trigPin D1 // Trigger Pin

// setup blynk serial console
#define BLYNK_PRINT Serial

// include library for WiFi and Blynk
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


////////////////////////////////////////////////////////////////
// define variables
////////////////////////////////////////////////////////////////

// Auth Token from the Blynk App
char auth[] = "<here goes your auth token>";

// Blynk Server IP
IPAddress blynkServer(192,168,0,200);

// Blynk Server Port
int port = 8080;

// WiFi credentials.
char ssid[] = "<your ssid>";
char pass[] = "<your WLAN password>";

// ultrasonic sound wave travel time in microseconds
long duration;

// distance between sensor and reflecting object in cm
int distance;

// warn limit for distance in cm
int maxValue = 200;

// variable for enabeling or disabeling notifications
boolean sendNotification = false;

// timer for running our function every so often
SimpleTimer timer;


////////////////////////////////////////////////////////////////
// functions
////////////////////////////////////////////////////////////////

// function to get value from sensor and send it to the app
void readSensorAndSendValue() {
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;

  // send distance to app
  Blynk.virtualWrite(V1, distance);

  // send distance to serial console for debugging
  Serial.print("Distance: ");
  Serial.println(distance);
  
  // send notifications if they are enabled and if the distance is below the limit set by maxValue
  if(sendNotification && distance > maxValue)
  {
    Serial.println("Sending notification: Wasserstand über der Warnschwelle (" + String(maxValue) + "cm): " + String(distance) + "cm.");
    Blynk.notify("Wasserstand über der Warnschwelle (" + String(maxValue) + "cm): " + String(distance) + "cm.");

    // deactivating notifications to avoid flooding the app
    sendNotification = false;
    Blynk.virtualWrite(V0, sendNotification);
  }
}

// gets notification setting from app
BLYNK_WRITE(V0)
{
  sendNotification = (param.asInt() == 1);

  // send Button value to serial console
  Serial.print("V0 Button value is: ");
  Serial.println(sendNotification);
}

// gets limit setting from the app
BLYNK_WRITE(V2)
{
  maxValue = param.asInt();

  // send value to serial console
  Serial.print("V2 Slider value is: ");
  Serial.println(maxValue);
}

// inital setup of the program, is executed first
void setup(){
  // setup PINs
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // setup serial console
  Serial.begin(9600);

  // setup blynk with WLAN credentials and Blynk-Server IP and Port
  Blynk.begin(auth, ssid, pass, blynkServer, port);

  // set timer that executes the function readSensorAndSendValue every 20 seconds
  timer.setInterval(20000L, readSensorAndSendValue);

  // set notification button value in the app to the value of sendNotification variable
  Blynk.virtualWrite(V0, sendNotification);
  Blynk.virtualWrite(V2, maxValue);
}

// main function of the program, is executed after setup() function
void loop(){
  // start blynk (blynk is configured in setup)
  Blynk.run();

  // start timer (timer is configured in setup)
  timer.run();
}
