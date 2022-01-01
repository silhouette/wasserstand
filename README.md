# water level monitoring with blynk, D1 mini and ultrasonic sensor
Hardware used: Wemos D1 mini with HC SR-04 ultrasonic sensor

Adopt the following values to your needs:

```c
// Auth Token from the Blynk App
char auth[] = "<here goes your auth token>";

// Blynk Server IP
IPAddress blynkServer(192,168,0,200);

// Blynk Server Port
int port = 8080;

// WiFi credentials.
char ssid[] = "<your ssid>";
char pass[] = "<your WLAN password>";
```
