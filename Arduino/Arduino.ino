#include "EspMQTTClient.h" 
#include "ArduinoJson.h" 
#include <Servo.h>
Servo servo;

#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1

void onConnectionEstablished();

EspMQTTClient client(
  "Gabriels iPhone",                           // Wifi ssid                                }
  "Password",                                  // Wifi password                          }
  "maqiatto.com",                                   // MQTT broker ip                       } 
  1883,                                             // MQTT broker port                     } 
  "gabriel.calota@abbindustrigymnasium.se",            // MQTT username                        } -- Information anges för att IOT-enheten ska anslutas till rätt MQTT-broker och nätverk.
  "Password",                                         // MQTT password                        }
  "Motordator",                                 // Client name                          }
  onConnectionEstablished,                          // Connection established callback      }
  true,                                             // Enable web updater                   }
  true                                              // Enable debug messages                }
);

unsigned long lastVelInst = 0;
unsigned long lastTurnInst = 0;
int TwoSec = 2000;
int HalfSec = 500; 
int Turn = 90;
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
    if (payload == "fram") {
      digitalWrite(motorPinRightDir, 1);
      digitalWrite(motorPinRightSpeed, 1023);
      Driving = true;
      lastVelInst = millis();
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Fram");
      }
    else if (payload == "bak") {
      digitalWrite(motorPinRightDir, 0);
      digitalWrite(motorPinRightSpeed, 1023);
      Driving = true;
      lastVelInst = millis();
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Bak");
      }
    else if (payload == "right") {
      if (Turn > 30) {
        Turn = Turn-5;
        servo.write(Turn);
        Serial.println(Turn);
        }
      lastTurnInst = millis();
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Höger");
      }
     else if (payload == "left") {
      if (Turn < 150) {
        Turn = Turn+5;
        servo.write(Turn);
        Serial.println(Turn);
        }
      lastTurnInst = millis();
      client.publish("gabriel.calota@abbindustrigymnasium.se/motor/logger", "Motordator: Vänster");
      }
      
  });
  
  };

void loop() {
//  prevVelocity = velocity
  client.loop();

  if ((lastVelInst < millis() - TwoSec) && (Driving == true)) {
    digitalWrite(motorPinRightSpeed, 0);
    Driving = false;
    Serial.println("Stop");
    } 

  if ((lastTurnInst < millis() - HalfSec) && (Turn != 90)) {
    Turn = 90;
    servo.write(Turn);
    Serial.println("Rakt");
    }
  

}
