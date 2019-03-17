/**
 * Tiny and cross-device compatible CCITT CRC16 Bit Per Bit or Byte Per Byte calculator library - uCRC16BPBLib
 *
 * @copyright Naguissa
 * @author Naguissa
 * @email naguissa@foroelectro.net
 * @version 1.0.0
 * @created 2018-04-21
 */
#include "uCRC16BPBLib.h"

/**
 * Constructor
 *
 * Nothing to do here
 */
uCRC16BPBLib::uCRC16BPBLib() {
	reset();
}


void uCRC16BPBLib::reset() {
	_crc = 0xffff;
	_feeded8bits = 0;
	_tmp8bits = 0;
	_data = 0;
	_first = true;
}


/**
 * Calculate CRC16 function
 *
 * @param	data_p	*char	Pointer to data
 * @param	length	uint16_t	Length, in bytes, of data to calculate CRC16 of. Should be the same or inferior to data pointer's length.
 */
void uCRC16BPBLib::feedBit(bool data_p) {
//	_tmp8bits = (_tmp8bits << 1) | (data_p ? 0x01 : 0x00);
	_tmp8bits = (_tmp8bits << 1);
	if (data_p) {
		_tmp8bits |= 0x01;
	}
	_feeded8bits++;

	if (_feeded8bits == 8) {
		feedByte((char) _tmp8bits);
		_feeded8bits = 0;
	}
}

/**
 * Calculate CRC16 function
 *
 * @param	data_p	*char	Pointer to data
 * @param	length	uint16_t	Length, in bytes, of data to calculate CRC16 of. Should be the same or inferior to data pointer's length.
 */
void uCRC16BPBLib::feedByte(char data_p) {
	uint8_t i;

	// 1st byte received
	if (_first == true) {
		_crc = 0xffff;
		_first = false;
	}

	_data = (uint16_t) 0xff & data_p;
	for (i = 0; i < 8; i++, _data >>= 1) {
		if ((_crc & 0x0001) ^ (_data & 0x0001)) {
			_crc = (_crc >> 1) ^ uCRC16BPBLib_POLYNOMIAL;
		} else {
			_crc >>= 1;
		}
	}
}



uint16_t uCRC16BPBLib::getResult() {
	if (_first == true) {
		return (~_crc);
	}

	_crc = ~_crc;
	_data = _crc;
	_crc = (_crc << 8) | (_data >> 8 & 0xFF);
	return (_crc);
}


