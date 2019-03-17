/**
 * Tiny and cross-device compatible CCITT CRC16 Bit Per Bit or Byte Per Byte calculator library - uCRC16BPBLib
 *
 * @copyright Naguissa
 * @author Naguissa
 * @email naguissa@foroelectro.net
 * @version 1.0.0
 * @created 2018-04-21
 */
#ifndef _uCRC16BPBLib_
	#define _uCRC16BPBLib_

	#include <Arduino.h>

	#define uCRC16BPBLib_POLYNOMIAL 0x8408

	class uCRC16BPBLib {
		public:
			uCRC16BPBLib();

			void reset();

			void feedBit(bool);
			void feedByte(char);

			uint16_t getResult();

			static const uint16_t crc_ok = 0x470F;
		private:
			uint16_t _data;
			uint16_t _crc;
			uint8_t _feeded8bits = 0;
			char _tmp8bits = 0;
			bool _first = true;

	};

#endif

