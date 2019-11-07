/* ROBOT DOFII by The Roboteur */
/* Description: Human interactive homebrewed robot */


/* OTA Dependencies */
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

/* Servo Dependencies */
#include <Servo.h> 

/* Network User and Password */
const char* ssid = "ZTEH108N_3C01E";          // Wifi User Name
const char* password = "X9425TE9";            // Wifi Password

/* Create Server */
ESP8266WebServer server;

bool ota_flag = true;
uint16_t time_elapsed = 0;
int blinkSpeed = 1000;

/* For ultrasonic sensor */
const int trigPin = D6;                  // DEFINE PIN NUMBERS           
const int echoPin = D5;
           
long duration;                          // DEFINE VARIABLES
int distance;

/* Define Servo Variables */
int PIN_BODY = D8; // Body 
int PIN_HEAD = D7; // Neck (temporary)

/* Create Servo Object */
Servo SERVO_BODY;
Servo SERVO_HEAD; 

int state_current = 0;


/*******************/ 

/**** SETUP ********/
void setup() {
  // pinMode(2, OUTPUT);   // DEFAULT PIN/LED                                                       
  Serial.begin(115200); 
  Serial.println("Starting ...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Restarting...");
    delay(5000);
    ESP.restart();
  }

  SERVO_BODY.attach(PIN_BODY);
  SERVO_HEAD.attach(PIN_HEAD); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  // SET THE ROUTE IP/ota thru ESP.restart() function
  // BYPASSES THE LOOP
  server.on("/ota", [](){
    server.send(200, "text/plain", "Upload the firmware.");
    delay(1000);
    ESP.restart();    
  });

  server.on("/blinkFast", [](){
    server.send(200, "text/plain", "Fast blink!" );
    delay(1000);
    blinkSpeed = 100;    
  });
  
  server.on("/blinkSlow", [](){
    server.send(200, "text/plain", "Slow blink!");
    delay(1000);
    blinkSpeed = 2000;    
  });

  server.on("/blinkNormal", [](){
    server.send(200, "text/plain", "Normal blink.");
    delay(1000);
    blinkSpeed = 1000;    
  });

  server.on("/list", [](){
    server.send(200, "text/plain", "1] ip/ota 2] ip/blinkFast 3] ip/blinkSlow 4] ip/blinkNormal 5] ip/stretch 6] ip/scan 7] ip/stop");
    delay(1000);
  });

  server.on("/stretch", [](){
    server.send(200, "text/plain", "Dofii stretches.");
    delay(1000);
    state_current = 1;
  });

  server.on("/scan", [](){
    server.send(200, "text/plain", "Dofii scans.");
    delay(1000);
    state_current = 2;
  });

  server.on("/sleep", [](){
    server.send(200, "text/plain", "Dofii sleeps.");
    delay(1000);
    state_current = 3;
  });
    
  server.on("/ready", [](){
    server.send(200, "text/plain", "Dofii gets ready.");
    delay(1000);
    state_current = 4;
  });

  server.on("/stop", [](){
    server.send(200, "text/plain", "Dofii stops.");
    delay(1000);
    state_current = 4;
  });

   server.on("/hello", [](){
    server.send(200, "text/plain", "Dofii nods.");
    delay(1000);
    state_current = 5;
  });

  server.on("/normal", [](){
    server.send(200, "text/plain", "Dofii's normal.");
    delay(1000);
    state_current = 6;
  });

    server.on("/", [](){
    server.send(200, "text/plain", "1] ip/ota 2] ip/blinkFast 3] ip/blinkSlow 4] ip/blinkNormal 5] ip/stretch 6] ip/scan 7] ip/stop");
    delay(1000);
  });

  
  server.begin();
 

}
/****************** NEW FUNCTIONS **************************************/
void state_machine_serial() {
 
  // state_previous = state_current; delay(1000);
    
  switch (state_current) {       
      
      case 1: // STRETCH
           SERVO_HEAD.write(60);
           SERVO_BODY.write(0);
           delay(2000);
           SERVO_BODY.write(180);
           delay(2000);
           SERVO_BODY.write(90);
           delay(2000);
           break;
      
      case 2: // SCAN     
           SERVO_HEAD.write(60);
           SERVO_BODY.write(0);
           delay(1000);
           SERVO_BODY.write(45);
           delay(1000);
           SERVO_BODY.write(90);
           delay(1000);
           SERVO_BODY.write(135);
           delay(1000);
           SERVO_BODY.write(180);
           delay(1000);
           SERVO_BODY.write(135);
           delay(1000);
           SERVO_BODY.write(90);
           delay(1000);
           break;

       case 3: // SLEEP
           SERVO_BODY.write(90);
           SERVO_HEAD.write(0);
           delay(2000);
           break;

       case 4: // READY
           SERVO_HEAD.write(60);
           SERVO_BODY.write(90);
           delay(1000);
           break;

       case 5: // HELLO
           SERVO_HEAD.write(60);
           delay(1000);
           SERVO_HEAD.write(90);
           delay(1000);
           SERVO_HEAD.write(60);
           break;

       case 6: // NORMAL
           /* Ultrasonic Sensor */
           digitalWrite(trigPin, LOW);             // CLEARS THE trigPin
           delayMicroseconds(2);
        
           digitalWrite(trigPin, HIGH);            // SETS THE trigPIN TO HIGH STATE FOR 10 MICRO SECONDS
           delayMicroseconds(10);
           digitalWrite(trigPin, LOW);
        
           duration = pulseIn(echoPin, HIGH);      // READS THE echoPin, RETURNS THE SOUNDWAVE TRAVEL TIME IN MICROSECONDS
           distance = duration*0.034/2;            // DISTANCE CALCULATIONS  
            
           if (distance < 20)  {
              SERVO_HEAD.write(60);
              delay(500);
              SERVO_HEAD.write(90);
              delay(500);
              SERVO_HEAD.write(60);
              delay(500);
           }
                                
              break;
           
  }   

  
}

void loop() {
 
  /* For Route IP OTA */
  if(ota_flag)
  {
    while(time_elapsed < 25000)
    {
      ArduinoOTA.handle();
      time_elapsed = millis();
      delay(10);
    }
    ota_flag = false;
  }

  server.handleClient();
  
  // digitalWrite(2, !digitalRead(2));
  // delay(blinkSpeed);

  state_machine_serial();

  

}
