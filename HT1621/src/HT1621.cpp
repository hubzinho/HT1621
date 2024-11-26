/*******************************************************************************
Copyright 2016-2018 anxzhu (github.com/anxzhu)
Copyright 2018-2020 Valerio Nappi (github.com/5N44P) (changes)
Based on segment-lcd-with-ht1621 from anxzhu (2016-2018)
(https://github.com/anxzhu/segment-lcd-with-ht1621)

Partially rewritten and extended by Valerio Nappi (github.com/5N44P) in 2018

This file is part of the HT1621 arduino library, and thus under the MIT license.
More info on the project and the license conditions on :
https://github.com/5N44P/ht1621-7-seg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#include <Arduino.h>
#include "HT1621.h"

HT1621::HT1621(){
	_buffer[0] = 0x00;
	_buffer[1] = 0x00;
	_buffer[2] = 0x00;
	_buffer[3] = 0x00;
	_buffer[4] = 0x00;
	_buffer[5] = 0x00;
	_buffer[6] = 0x00;
	for (int i = 7; i < 10; i++) {
		_buffer[i] = 0x00;
	}
	_buffer_line2[2] = 0x00;
	_buffer_line2[3] = 0x00;
	_buffer_line2[4] = 0x00;
	_buffer_line2[5] = 0x00;
}

void HT1621::begin(int cs_p, int wr_p, int data_p, int backlight_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);
	pinMode(backlight_p, OUTPUT);
	_cs_p=cs_p;
	_wr_p=wr_p;
	_data_p=data_p;
	_backlight_p=backlight_p;
	_backlight_en=true;
	config();
}

void HT1621::begin(int cs_p, int wr_p, int data_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);
	_cs_p=cs_p;
	_wr_p=wr_p;
	_data_p=data_p;
	_backlight_en = false;
	config();
}

void HT1621::wrDATA(unsigned char data, unsigned char cnt) {
	unsigned char i;
	for (i = 0; i < cnt; i++) {
		digitalWrite(_wr_p, LOW);
		delayMicroseconds(4);
		if (data & 0x80) {
			digitalWrite(_data_p, HIGH);
		}
		else {
			digitalWrite(_data_p, LOW);
		}
		digitalWrite(_wr_p, HIGH);
		delayMicroseconds(4);
		data <<= 1;
	}
}
void HT1621::wrclrdata(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;
	digitalWrite(_cs_p, LOW);
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	digitalWrite(_cs_p, HIGH);
}

void HT1621::display()
{
	wrCMD(LCDON);
}

void HT1621::noDisplay()
{
	wrCMD(LCDOFF);
}

void HT1621::wrone(unsigned char addr, unsigned char sdata)
{
	addr <<= 3;
	digitalWrite(_cs_p, LOW);
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	digitalWrite(_cs_p, HIGH);
}

void HT1621::backlight()
{
	if (_backlight_en)
		digitalWrite(_backlight_p, LOW);
	delay(1);
}

void HT1621::noBacklight()
{
	if(_backlight_en)
		digitalWrite(_backlight_p, HIGH);
	delay(1);
}

void HT1621::wrCMD(unsigned char CMD) {  //100
	digitalWrite(_cs_p, LOW);
	wrDATA(0x80, 4);
	wrDATA(CMD, 8);
	digitalWrite(_cs_p, HIGH);
}

void HT1621::config()
{
	wrCMD(BIAS);
	wrCMD(RC256);
	wrCMD(SYSDIS);
	wrCMD(WDTDIS1);
	wrCMD(SYSEN);
	wrCMD(LCDON);
}

void HT1621::wrCLR(unsigned char len) {
	unsigned char addr = 0;
	unsigned char i;
	for (i = 0; i < len; i++) {
		wrclrdata(addr, 0x00);
		addr = addr + 2;
	}
}

void HT1621::Symbol(int symbol) {
	switch (symbol)
	{
	
		
	case 1:
		wrone(5, 0b00000001); //digito F *
		wrone(6, 0b10000000); //digito M *

		break;

	case 2:
		wrone(13, 0b00000100); //digito Hz *
		
		break;

	case 3:
		wrone(13, 0b00011100); //digito KHz *
		
		break;
	
	default:
	  	wrone(5, 0b00000000); //digito f *
		wrone(6, 0b00000000); //digito M *
		wrone(13,0b00000000); 
		break;
	}
}

void HT1621::setLevel(int level) {
	// zero out the previous (otherwise the or couldn't be possible)
	       
	switch(level){
		case 1: // battery on and all 3 segments
			wrone(6, 0b00001000); // 13 e 0
			//wrone(6, 0b10000000); //13
			wrone(7, 0b00000000);
	       
	        //_buffer_line2[5] |= 0b11111111; //digito 3
			//_buffer_line2[6] |= 0b11111111;
			//_buffer_line2[7] |= 0b11111111;
        break;
		case 2: 
			//_buffer_line2[3] |= 0b00000100; 
			wrone(6, 0b00001100);
			wrone(7, 0b00000000);	
        break;
		case 3: 
			wrone(6, 0b00001110);
			wrone(7, 0b00000000);
		break;
		case 4: 
			wrone(6, 0b00001111); 
			wrone(7, 0b00000000);
		break;
		case 5: 
			wrone(6, 0b00001111);
			wrone(7, 0b10000000);
		break;
		case 6: 
			wrone(7, 0b11000000);
		break;
		case 7: 
			wrone(7, 0b11100000);
		break;
		case 8: 
			wrone(7, 0b11110000);
		break;
		case 9: 
			wrone(7, 0b11111000);
		break;
		case 10:
			wrone(7, 0b11111100);
		break;
		case 11: 
			wrone(7, 0b11111110);
		break;
		case 12:
			wrone(7, 0b11111111);
			wrone(8, 0b00000000);
		break;
		case 13:
			wrone(8, 0b10000000);
		break;
		case 14:
			wrone(8, 0b11000000);
		break;
		case 15:
			wrone(8, 0b11100000);
		break;
		case 16:
			wrone(8, 0b11110000);
		break;
		case 17:
			wrone(8, 0b11111000); 
		break;

		case 0: // battery indication off
		default:
			wrone(5, 0b00000000); //digito 1 *
			wrone(6, 0b00000000); //digito 2
			wrone(7, 0b00000000); //digito 3
			wrone(8, 0b00000000); //digito 4
			wrone(13, 0b00000000); //digito 5
			wrone(0, 0b00000000); //digito 6
	    break;
	}

	//updateLine2();
	
}


void HT1621::clear(){
	wrCLR(16);
}

// takes the buffer and puts it straight into the driver
void HT1621::update(int line){

	// the buffer is backwards with respect to the lcd. could be improved
	 if (line == 1) {
        // Atualiza a linha 1
	wrone(1, _buffer[2]); //digito 1 *
	wrone(2, _buffer[3]); //digito 2
	wrone(3, _buffer[4]); //digito 3
	wrone(4, _buffer[5]); //digito 4
	
   } else if (line == 2) {
        // Atualiza a linha 2
	wrone(9, _buffer[2]); //digito 1 *
	wrone(10, _buffer[3]); //digito 2
	wrone(11, _buffer[4]); //digito 3
	wrone(12, _buffer[5]); //digito 4
   } 

}

void HT1621::updateLine2() {

	wrone(5, _buffer_line2[2]); //digito 1 *
	wrone(6, _buffer_line2[3]); //digito 2
	wrone(7, _buffer_line2[4]); //digito 3
	wrone(8, _buffer_line2[5]); //digito 4
	wrone(13, _buffer_line2[6]); //digito 5
	wrone(0, _buffer_line2[7]); //digito 6
	

}


void HT1621::print(long num, int line, const char* flags, int precision){
	if(num > 999999) // basic checks
		num = 999999; // clip into 999999
	if(num < -99999) // basic checks
		num = -99999; // clip into -99999

	char localbuffer[7]; //buffer to work within the function
	snprintf(localbuffer, 7, flags, num); // convert the decimal into string
	#ifdef _HTDEBUG
		Serial.begin(9600);
		Serial.print(localbuffer);
		Serial.print("\t");
	#endif

	// horrible handling but should get us working. needs refactor in next major
	if (precision > 0 && (num) < pow(10, precision)) {
		// we remove extra leading zeros
		for (int i = 0; i < (5 - precision); i++) {
			#ifdef _HTDEBUG
				Serial.print(localbuffer[1]);
			#endif // _HTDEBUG
			if(localbuffer[i+1] == '0' && localbuffer[i] != '-'){ // we remove only if there is another zero ahead AND if it's not a minus sign
				localbuffer[i] = ' ';
			}
			else{
				break;
			} 
			#ifdef _HTDEBUG
				Serial.println();buffer[1];
			#endif // _HTDEBUG
	}
	}


	for(int i=0; i<6; i++){
		_buffer[i] &= 0b00000001; // mask the first bit, used by batter and decimal point
		_buffer[i] |= charToSegBits(localbuffer[i]);
	}
	update(line);
}


//==============================================================================================

void HT1621::print(double num, int line, int precision){
	if(num > 999999) // basic checks
		num = 999999; // clip into 999999
	if(num < -99999) // basic checks
		num = -99999; // clip into -99999

	if(precision > 3 && num > 0)
		precision = 3; // if positive max precision allowed = 3
	else if(precision > 2 && num < 0)
		precision = 2;// if negative max precision allowed = 2
	if(precision < 0)
		precision = 0; // negative precision?!

	const char* flags = (precision > 0 && abs(num) < 1) ? "%06li" : "%6li";

	long integerpart;
	integerpart = ((long)(num*pow(10,precision)));

	print(integerpart, line, flags, precision); // draw the integerized number
	setdecimalseparator(precision); // draw the decimal point

	update(line);

}



void HT1621::print(const char* str, int line, bool leftPadded){
	int chars = strlen(str);
	int padding = 6 - chars;

	for(int i = 0; i < 6; i++){
		_buffer[i] &= 0b00000001; // mask the first bit, used by batter and decimal point
		char character = leftPadded
				 		 ? i < padding ? ' ' : str[i - padding]
				 		 : i >= chars ? ' ' : str[i];
		_buffer[i] |= charToSegBits(character);
	}

	setdecimalseparator(0); // Hide decimal point
	update(line);
}

//==================================================================================

void HT1621::setdecimalseparator(int decimaldigits) {
	// zero out the eight bit
	_buffer[2] &= 0b11111110;
	_buffer[3] &= 0b11111110;
	_buffer[4] &= 0b11111110;

	if( decimaldigits <= 0 || decimaldigits > 3){
		return;
	}

	// 3 is the digit offset
	// the first three eights bits in the buffer are for the battery signs
	// the last three are for the decimal point
	_buffer[5-decimaldigits] |= 0b00000001;
}

char HT1621::charToSegBits(char character) {
	switch (character) {
	
	case '*': // For degree for now
		return 0b11001100;
	case '|':
		return 0b00000110;
	case '-':
		return 0b01000000;
	case '_':
		return 0b0001000;
	case '0':
		return 0b10111110;
	case '1':
		return 0b0000110;
	case '2':
		return 0b01111100;
	case '3':
		return 0b01011110;
	case '4':
		return 0b11000110;
	case '5':
		return 0b11011010;
	case '6':
		return 0b11111010;
	case '7':
		return 0b00001110;
	case '8':
		return 0b11111110;
	case '9':
		return 0b11011110;
		
	case 'A':
	case 'a':
		return 0b11101110;
	case 'b':
	case 'B':
		return 0b11110010;
	case 'c':
	//	return 0b0001110;
	case 'C':
		return 0b10111000;
	case 'd':
	case 'D':
		return 0b01110110;
	case 'e':
	//	return 0b0001110;
	case 'E':
		return 0b11111000;
	case 'f':
	//	return 0b0000111;
	case 'F':
		return 0b11101000;
	case 'G':
	case 'g':
		return 0b1011101;
	case 'h':
	//	return 0b1000111;
	case 'H':
		return 0b1100111;
	case 'i':
	//	return 0b1000000;
	case 'I':
		return 0b1100000;
	case 'J':
	case 'j':
		return 0b1101000;
	case 'l':
	//	return 0b1100000;
	case 'L':
		return 0b10110000;
	case 'm':
	case 'M':
		return 0b01101010;
	case 'n':
	case 'N':
		return 0b1000110;
	case 'O': // we can keep this for zero
	//	return 0b1111101;
	case 'o':
		return 0b01110010;
	case 'P':
	case 'p':
		return 0b0110111;
	case 'q':
	case 'Q':
		return 0b1110011;
	case 'r':
	case 'R':
		return 0b01100000;
	case 'S':
	case 's':
		return 0b1011011;
	case 't':
	case 'T':
		return 0b0001111;
	case 'u':
	//	return 0b1001100;
	case 'U':
		return 0b1101101;
	case 'v':
	case 'V':
		return 0b10110110;
	case 'Y':
	case 'y':
		return 0b1101011;
	case 'z':
	case 'Z':
		return 0b0111110;
	
	case ' ':
	default:
		return 0b0000000;
	}
}