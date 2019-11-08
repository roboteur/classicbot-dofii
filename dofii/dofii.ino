/* ROBOT DOFII by The Roboteur */

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

  server.on("/", handle_OnConnect);
  server.on("/up", handle_Up);
  server.on("/down", handle_Down);
  server.on("/right", handle_Right);
  server.on("/left", handle_Left);
  
  server.begin();

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

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
  
  }

void handle_Up()  {
  server.send(200, "text/html", UpHTML());
  
  }

void handle_Down()  {
  server.send(200, "text/html", DownHTML());
  
  }

void handle_Right()  {
  server.send(200, "text/html", RightHTML());
  
  }

void handle_Left()  {
  server.send(200, "text/html", LeftHTML());
  
  }
  
String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>DOFII</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 12px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>DOFII</h1>\n";
  ptr +="<p><a href=\"/up\"><button>UP</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>DOWN</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>LEFT</button></a></p>";
  ptr +="<p><a href=\"/right\"><button>RIGHT</button></a></p>";  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  }

String LeftHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>DOFII</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 12px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>DOFII</h1>\n";
  ptr +="<p><a href=\"/up\"><button>UP</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>DOWN</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>LEFT</button></a></p>";
  ptr +="<p><a href=\"/right\"><button>RIGHT</button></a></p>";  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  }

String RightHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>DOFII</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 12px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>DOFII</h1>\n";
  ptr +="<p><a href=\"/up\"><button>UP</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>DOWN</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>LEFT</button></a></p>";
  ptr +="<p><a href=\"/right\"><button>RIGHT</button></a></p>";  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  }

String UpHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>DOFII</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 12px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>DOFII</h1>\n";
  ptr +="<p><a href=\"/up\"><button>UP</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>DOWN</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>LEFT</button></a></p>";
  ptr +="<p><a href=\"/right\"><button>RIGHT</button></a></p>";  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  }

String DownHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>DOFII</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 12px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>DOFII</h1>\n";
  ptr +="<p><a href=\"/up\"><button>UP</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>DOWN</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>LEFT</button></a></p>";
  ptr +="<p><a href=\"/right\"><button>RIGHT</button></a></p>";  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  }
