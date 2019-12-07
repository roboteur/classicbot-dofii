# ROBOT DOFII
* Easy to build desktop companion robot
* Reprogrammable and Teaches Programming
* Build Your Own For Around $20 (excluding battery)
## Features
* Smartphone Controlled
* Over-The-Air Update
* Code Design Pattern Included

## Quick Start
1. Setup The ESP8266
    * Download Arduino IDE: https://www.arduino.cc/en/Main/Software
    * Open Arduino IDE and click: File -> Preferences -> Additional Boards Manager --> Paste this     link: https://arduino.esp8266.com/stable/package_esp8266com_index.json
    * Setup the board: Go to Tools -> Board -> Board Manager -> search ESP8266 Community, then click install
    * Select the ESP8266 board: Tools -> Board -> NodeMCU 1.0 (ESP12E Module)
    * Select the PORT: Tools -> PORT -> USB0 (or anything that works)
    * Upload the Code: Sketch -> Upload (There should be no error. If there is, it is mostly about user permissions. Message me if you can't go through this.)
2. Upload the code dofii.ino
3. Find the IP Address from the Arduino IDE Serial Monitor. 
3. Copy and paste the IP Address in the browser and start from there

## Bill Of Materials
1. Wemos D1 Mini (ESP8266) x 1  = $5.00 approx.
2. Ultrasonic Sensor x 1  = $2.00 approx.
3. Pan and Tilt x 1 set = $2.00 approx.
4. Servo Motors x 2 (included in pan and tilt) = $4.00 approx.
5. 18650 Battery Holder x 2 slots = $1.00 approx.
6. Solid Wires #22 x 3ft. = $1.00 approx.
7. Jumper Wires male/female x 4 = $0.5 approx. 
8. USB Cable x 1 = $2.00
9. Half-sized breadboard x 1 = $2.00 
10. Long-legged Headers x 3 = $2.00
11. 18650 Battery x 2 = Depends on you. 

## Robot Construction
* (Show Photo)
* (Show Video)

## Wiring Connection
   * Ultrasonic Sensor
        * TRIG --> D6
        * ECHO --> D5  
   * Servo Motors 
        * HEAD --> D7
        * BODY --> D8
   * ALL Vcc --> 5V Powersupply
   * ALL Gnd --> GND Powersupply

## Supplementary Video
* (Link YouTube Video)
