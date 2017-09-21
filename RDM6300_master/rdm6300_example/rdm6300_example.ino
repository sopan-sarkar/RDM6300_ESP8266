#include <SoftwareSerial.h>
#include <RDM6300.h>

#define RFID_RX_PIN 2
#define RFID_TX_PIN 3

RDM6300 RFID(RFID_RX_PIN, RFID_TX_PIN);

int tag;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial Ready");
}

void loop() {
  if(RFID.isIdAvailable()) {
    tag = RFID.readId();
    Serial.print("ID:       ");
    Serial.println(tag,HEX);
  }

}
