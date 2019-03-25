#include <global.h>
#include <Canbus.h>
#include <mcp2515_defs.h>
#include <mcp2515.h>
#include <defaults.h>

#define BAUD_RATE 9600
#define REFRESH_RATE 10

#define MESSAGE_PRESSURE 0x631

union Measure {
  float f;
  byte b[4];
};

Measure pressure_kPa;

void setup() {
  Serial.begin(BAUD_RATE);
  delay(1000);

  if (Canbus.init(CANSPEED_500))
    Serial.println("CAN bus initialized. Reading CAN messages...");
  else
    Serial.println("CAN controller failed initialization!");
}

void loop() {
  tCAN message;
  if (mcp2515_check_message()) {
    if (mcp2515_get_message(&message)) {
      
      switch (message.id) {
        case MESSAGE_PRESSURE:
          for (int i = 0; i < 4; i++)
            pressure_kPa.b[i] = message.data[i];
          Serial.print(pressure_kPa.f);
          Serial.println(" kPa");
          break;
      }
    }
  }

  delay(1000 / REFRESH_RATE);
}