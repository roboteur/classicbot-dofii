/* ROBOT DOFII (by The Roboteur)
 *   
 * Website: www.roboteur.me
 * Facebook: facebook.com/TheRoboteur
 * Instagram: instagram.com/the_roboteur
 * YouTube: bitly.com/RoboteurTV  
 * GitHub: github.com/roboteur
 *  
 */

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

bool ota_time_trigger = true;
uint16_t ota_time_consumed = 0;


/* Assign Pins To Ultrasonic Sensor */
const int trigPin = D6;     
const int echoPin = D5; 
            
long duration;                          
int distance;

/* Define Servo Variables */
int PIN_BODY = D8; 
int PIN_HEAD = D7; 

/* Create Servo Object */
Servo SERVO_BODY;
Servo SERVO_HEAD; 

int state_current = 0;

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

  server.on("/ota", [](){
    server.send(200, "text/plain", "Upload the firmware.");
    delay(1000);
    ESP.restart();    
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

  /* Mobile Control */
  server.on("/", handle_OnConnect);
  server.on("/up", handle_Up);
  server.on("/down", handle_Down);
  server.on("/right", handle_Right);
  server.on("/left", handle_Left);
  server.on("/mid", handle_Mid);  
  
  server.begin();

}

void loop() {
 
  /* For Route IP OTA */
  if(ota_time_trigger)
  {
    while(ota_time_consumed < 25000)
    {
      ArduinoOTA.handle();
      ota_time_consumed = millis();
      delay(10);
    }
    ota_time_trigger = false;
  }

  server.handleClient();
  
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

           /* Mobile Control */   
       case 7: // UP
           SERVO_HEAD.write(0);
           
           break;

       case 8: // DOWN
           SERVO_HEAD.write(180);
           
           break;

       case 9: // LEFT
           SERVO_BODY.write(0);
           
           break;
        
       case 10: // RIGHT
           SERVO_BODY.write(180);
           
           break;
          
  }     
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
    state_current = 6;
  
  }

void handle_Up()  {
  state_current = 7;
  server.send(200, "text/html", UpHTML());
  
  }

void handle_Down()  {
  state_current = 8;
  server.send(200, "text/html", DownHTML());
  
  }

void handle_Right()  {
  state_current = 10;
  server.send(200, "text/html", RightHTML());
  
  }

void handle_Left()  {
  state_current = 9;
  server.send(200, "text/html", LeftHTML());
  
  }
  
void handle_Mid()  {
  state_current = 4;
  server.send(200, "text/html", MidHTML());
  
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
  ptr +="<p><a href=\"/up\"><button>U</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>L</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/mid\"><button>-</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/right\"><button>R</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>D</button></a></p>";
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
  ptr +="<p><a href=\"/up\"><button>U</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>L</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/mid\"><button>-</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/right\"><button>R</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>D</button></a></p>";
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
  ptr +="<p><a href=\"/up\"><button>U</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>L</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/mid\"><button>-</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/right\"><button>R</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>D</button></a></p>";
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
  ptr +="<p><a href=\"/up\"><button>U</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>L</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/mid\"><button>-</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/right\"><button>R</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>D</button></a></p>";
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
  ptr +="<p><a href=\"/up\"><button>U</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>L</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/mid\"><button>-</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/right\"><button>R</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>D</button></a></p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  }

String MidHTML(){
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
  ptr +="<p><a href=\"/up\"><button>U</button></a></p>";
  ptr +="<p><a href=\"/left\"><button>L</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/mid\"><button>-</button></a>&nbsp;&nbsp;&nbsp;<a href=\"/right\"><button>R</button></a></p>";
  ptr +="<p><a href=\"/down\"><button>D</button></a></p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  }