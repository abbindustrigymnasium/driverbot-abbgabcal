#include "EspMQTTClient.h" 
#include "ArduinoJson.h" 
#include <Servo.h>
Servo servo;

#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1

void onConnectionEstablished();

EspMQTTClient client(
  "wifi SSID",                           // Wifi ssid                                }
  "wifi password",                                  // Wifi password                          }
  "maqiatto.com",                                   // MQTT broker ip                       } 
  1883,                                             // MQTT broker port                     } 
  "gabriel.calota@abbindustrigymnasium.se",            // MQTT username                        } -- Information anges för att IOT-enheten ska anslutas till rätt MQTT-broker och nätverk.
  "Kanin21",                                         // MQTT password                        }
  "Motordator",                                 // Client name                          }
  onConnectionEstablished,                          // Connection established callback      }
  true,                                             // Enable web updater                   }
  true                                              // Enable debug messages                }
);

unsigned long lastVelInst = 0; //Deklarerar variabel som håller koll på när senaste hastighetsinstruktionen var
unsigned long lastTurnInst = 0; //Deklarerar variabel som håller koll på när senaste svänginstruktionen var
int HalfSec = 500; //En half sekund 
int Turn = 90; //Standardsväng, 90 grader, rakt på
bool Driving = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);
  servo.attach(14);
  Serial.begin(115200);
  servo.write(90);
}

void onConnectionEstablished() {

   client.subscribe("gabriel.calota@abbindustrigymnasium.se/motor", [] (const String & payload)
  {
    
    Serial.println(payload);
    if (payload == "fram") { //Kör framåt
      digitalWrite(motorPinRightDir, 1);
      digitalWrite(motorPinRightSpeed, 1023);
      Driving = true;
      lastVelInst = millis(); //Sparar senaste körinstruktionens tid
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Fram"); //Skickar senaste instruktionen till logger
      }
    else if (payload == "bak") {
      digitalWrite(motorPinRightDir, 0);
      digitalWrite(motorPinRightSpeed, 1023);
      Driving = true;
      lastVelInst = millis(); //Sparar senaste körinstruktionens tid
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Bak"); //Skickar senaste instruktionen till logger
      }
    else if (payload == "left") {
      if (Turn < 170) {
        Turn = Turn+5; //Svänger servot  grader per svänginstruktion
        servo.write(Turn);
        Serial.println(Turn);
        }
      lastTurnInst = millis();  //Sparar senaste svängsinstruktionens tid
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Vänster"); //Skickar senaste instruktionen till logger
      }
     else if (payload == "right") {
      if (Turn > 10) {
        Turn = Turn-5; //Svänger servot  grader per svänginstruktion
        servo.write(Turn);
        Serial.println(Turn);
        }
      lastTurnInst = millis();  //Sparar senaste svängsinstruktionens tid
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Höger"); //Skickar senaste instruktionen till logger
      }
      
  });
  
  };

void loop() {
  client.loop();

  if ((lastVelInst < millis() - HalfSec) && (Driving == true)) { //Kollar om den kör och om det har gått en halv sekund sedan senaste körinstruktionen. 
    digitalWrite(motorPinRightSpeed, 0); //Slutar köra
    Driving = false;
    Serial.println("Stop");
    } 

  if ((lastTurnInst < millis() - HalfSec) && (Turn != 90)) { //Kollar om den kör och om det har gått en halv sekund sedan senaste svänginstruktionen. 
    Turn = 90;
    servo.write(Turn); //Sätter tillbaka kursen på rakt på
    Serial.println("Rakt");
    }
  

}
