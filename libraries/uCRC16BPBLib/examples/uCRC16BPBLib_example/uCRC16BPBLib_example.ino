/**
 * Tiny and cross-device compatible CCITT CRC16 Bit Per Bit or Byte Per Byte calculator library - uCRC16BPBLib - Example
 *
 * @author Naguissa
 * @url https://www.github.com/Naguissa/uTimerLib
 * @url https://www.foroelectro.net
 */

#include "Arduino.h"
#include "uCRC16BPBLib.h"


uCRC16BPBLib crc;

uint16_t crcStringBytes(char * string, uint16_t length) {
	uint16_t i;
	crc.reset();
	for (i = length; i > 0; string++, i--) {
		crc.feedByte(*string);
	}
	return crc.getResult();
}

uint16_t crcStringBits(char * string, uint16_t length) {
	uint16_t i;
	int8_t j;
	crc.reset();
	for (i = length; i > 0; string++, i--) {
		for (j = 7; j >= 0; j--) {
//		for (j = 0; j < 8; j++) {
			crc.feedBit((bool) (((*string) >> j) & 0x01));
		}
	}
	return crc.getResult();
}







void setup() {
    Serial.begin(57600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }

	uint16_t crc;
	char string[40];
	string[0] = 'T';
	string[1] = (unsigned char)0xd9;
	string[2] = (unsigned char)0xe4;
	string[3] = NULL;

	Serial.print("The crc of \"T\" is 0xD9E4. crc16 bytes returned ");
	Serial.print(crcStringBytes(&string[0], 1), HEX);
	Serial.print("; crc16 bits returned ");
	Serial.println(crcStringBits(&string[0], 1), HEX);
	Serial.println();

	Serial.print("The crc of \"T 0xD9 0xE4\" is... Bytes: ");
	Serial.print(crcStringBytes(&string[0], 3), HEX);
	Serial.print("; Bits: ");
	Serial.print(crcStringBits(&string[0], 3), HEX);
	Serial.print(". The value of crc_ok is ");
	Serial.println(uCRC16BPBLib::crc_ok, HEX);
	Serial.println();

	strcpy(string, "THE,QUICK,BROWN,FOX,0123456789");
	Serial.print("The crc of \"");
	Serial.print(string);
	Serial.print("\" is 0x6E20. crc16 returned... Bytes ");
	Serial.print(crcStringBytes(&string[0], strlen(string)), HEX);
	Serial.print("; Bits: ");
	Serial.println(crcStringBits(&string[0], strlen(string)), HEX);
	Serial.println();

	string[0] = (unsigned char)0x03;
	string[1] = (unsigned char)0x3F;

	Serial.print("CCITT Recommendation X.25 (1984) Appendix I example:");
	Serial.print("\tThe crc of 0x03 0x3F is 0x5BEC. crc16 returned... Bytes: ");
	Serial.print(crcStringBytes(&string[0], 2), HEX);
	Serial.print("; Bits: ");
	Serial.println(crcStringBits(&string[0], 2), HEX);
	Serial.println();

	Serial.println(" - END -");
	Serial.println();

}


void loop() {
}

