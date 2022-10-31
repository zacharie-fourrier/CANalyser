#include "frame.h"

using namespace std;

namespace can
{
	Frame::Frame(string str) {
		
		unsigned i;
		vector<bool> tmp;
		for (i = 0; i < str.size(); i++) {
			stringstream ss;
			ss << str[i];
			bool b;

			if (!(ss >> b)) {
				throw "Parsing error";
			}
			else {
				tmp.push_back(b);
				if (tmp[0] == 1) { tmp.pop_back(); }
				else { _String += str[i]; }
			}
		}
		//preparing the stuffing bit information vector
		for (i = 0; i < tmp.size() - 5; i++) {
			//removing stuffing bits for 000001
			if (tmp[i] == 0 && tmp[i + 1] == 0 && tmp[i + 2] == 0 && tmp[i + 3] == 0 && tmp[i + 4] == 0 && tmp[i + 5] == 1) {
				_stuffing.push_back(i + 5); //filling the stuffing vector
			}
			//removing stuffing bits for 111110
			else if (tmp[i] == 1 && tmp[i + 1] == 1 && tmp[i + 2] == 1 && tmp[i + 3] == 1 && tmp[i + 4] == 1 && tmp[i + 5] == 0) {
				_stuffing.push_back(i + 5); //filling the stuffing vector
			}
		}

		cout << "\nStuffing bits are those :\n" << _String << endl;
		//filling the Frame vector without the stuffing bits
		unsigned stuffingBitsCount = 0;
		unsigned strCount = 0;
		for (unsigned s : _stuffing) {
			tmp.erase(tmp.begin() + s - stuffingBitsCount);
			stuffingBitsCount++;
			for (unsigned j = 0; j < s - strCount; j++) {
				cout << " ";
			}
			strCount = s + 1;
			cout << "^";
		}
		cout << endl;

		_Frame = tmp;

		_SOF = _Frame[0];
		_IDE = _Frame[13];

		for (unsigned k = 0; k < 11; k++) {
			_Identifier.push_back(_Frame.at(k + 1));
		}

		if (_IDE) {
			_SRR = _Frame[12];
			_RTR = _Frame[32];
			_R1 = _Frame[33];
			_R0 = _Frame[34];
			for (unsigned k = 0; k < 4; k++) {
				_DLC.push_back(_Frame.at(k + 35));
			}
			for (unsigned k = 0; k < 18; k++) {
				_Identifier.push_back(_Frame.at(k + 14));
			}
			if (!_RTR) {
				int crc = this->getDLC();
				for (unsigned k = 0; k < crc * 8; k++) {
					_Data.push_back(_Frame.at(k + 39));
				}
				for (unsigned k = 0; k < 15; k++) {
					_CRC.push_back(_Frame.at(k + 39 + (crc * 8)));
				}
				_CRCD = _Frame.at(55 + (crc * 8));
				_ACKS = _Frame.at(56 + (crc * 8));
				_ACKD = _Frame.at(57 + (crc * 8));
			}
			else {
				for (unsigned k = 0; k < 15; k++) {
					_CRC.push_back(_Frame.at(k + 39));
				}
				_CRCD = _Frame.at(55);
				_ACKS = _Frame.at(56);
				_ACKD = _Frame.at(57);
			}
		
		}
		else {
			_RTR = _Frame[12];
			_R0 = _Frame[14];
			_R1 = 0;
			_SRR = 0;
			for (unsigned k = 0; k < 4; k++) {
				_DLC.push_back(_Frame.at(k + 15));
			}
			if (!_RTR) {
				int crc = this->getDLC();
				for (unsigned k = 0; k < crc * 8; k++) {
					_Data.push_back(_Frame.at(k + 19));
				}
				for (unsigned k = 0; k < 15; k++) {
					_CRC.push_back(_Frame.at(k + 19 + (crc * 8)));
				}
				_CRCD = _Frame.at(35 + (crc * 8));
				_ACKS = _Frame.at(36 + (crc * 8));
				_ACKD = _Frame.at(37 + (crc * 8));
			}
			else {
				for (unsigned k = 0; k < 15; k++) {
					_CRC.push_back(_Frame.at(k + 19));
				}
				_CRCD = _Frame.at(35);
				_ACKS = _Frame.at(36);
				_ACKD = _Frame.at(37);
			}
		}
	}

	string Frame::toString() const {
		stringstream ss;
		ss << "Frame :" << endl;
		for (bool b : _Frame) {
			ss << b;
		}
		ss << endl;
		ss << "SOF = " << _SOF << endl;
		ss << "IDE = " << _IDE << " > " << this->getFormat() << endl;
		ss << "RTR = " << _RTR << " > " << this->getType() << endl;
		ss << "Identifier = ";
		for (bool b : _Identifier) {
			ss << b;
		}
		ss << " > " << this->getIdentifier() << endl << "DLC = ";
		for (bool b : _DLC) {
			ss << b;
		}
		ss << " > " << this->getDLC() << " Bytes of Data" << endl;
		if (!_RTR) {
			ss << "Data = ";
			for (bool b : _Data) {
				ss << b;
			}
			ss << " > " << this->getData() << endl;
		}
		else {
			ss << "Remote frame, no data" << endl;
		}
		ss << "CRC = ";
		for (bool b : _CRC) {
			ss << b;
		}
		ss << " > " << this->getCRC() << endl;
		return ss.str();
	}

	string Frame::getIdentifier() const {
		string identifier = "0x";
		identifier += this->binToHex(0, _Identifier.at(0), _Identifier.at(1), _Identifier.at(2));
		identifier += this->binToHex(_Identifier.at(3), _Identifier.at(4), _Identifier.at(5), _Identifier.at(6));
		identifier += this->binToHex(_Identifier.at(7), _Identifier.at(8), _Identifier.at(9), _Identifier.at(10));
		if (_IDE) {
			identifier += this->binToHex(0, 0, _Identifier.at(11), _Identifier.at(12));
			identifier += this->binToHex(_Identifier.at(13), _Identifier.at(14), _Identifier.at(15), _Identifier.at(16));
			identifier += this->binToHex(_Identifier.at(17), _Identifier.at(18), _Identifier.at(19), _Identifier.at(20));
			identifier += this->binToHex(_Identifier.at(21), _Identifier.at(22), _Identifier.at(23), _Identifier.at(24));
			identifier += this->binToHex(_Identifier.at(25), _Identifier.at(26), _Identifier.at(27), _Identifier.at(28));
		}
		return identifier;
	}

	int Frame::getDLC() const {
		string data;
		data += this->binToHex(_DLC.at(0), _DLC.at(1), _DLC.at(2), _DLC.at(3));
		int bytes;
		if (data == "0") { bytes = 0; }
		else if (data == "1") { bytes = 1; }
		else if (data == "2") { bytes = 2; }
		else if (data == "3") { bytes = 3; }
		else if (data == "4") { bytes = 4; }
		else if (data == "5") { bytes = 5; }
		else if (data == "6") { bytes = 6; }
		else if (data == "7") { bytes = 7; }
		else if (data == "8") { bytes = 8; }
		else if (data == "9") { bytes = 9; }
		else if (data == "A") { bytes = 10; }
		else if (data == "B") { bytes = 11; }
		else if (data == "C") { bytes = 12; }
		else if (data == "D") { bytes = 13; }
		else if (data == "E") { bytes = 14; }
		else if (data == "F") { bytes = 15; }
		return bytes;
	}

	string Frame::getData() const {
		string data = "0x";
		int size = this->getDLC();
		for (int k = 0; k < size * 8; k += 4) {
			data += this->binToHex(_Data.at(k), _Data.at(k + 1), _Data.at(k + 2), _Data.at(k + 3));
		}
		return data;
	}

	string Frame::getFormat() const {
		string format;
		if (_IDE) { format = "Extended format"; }
		else { format = "Standard format"; }
		return format;
	}

	string Frame::getCRC() const {
		string crc = "0x";
		crc += this->binToHex(0, _CRC.at(0), _CRC.at(1), _CRC.at(2));
		crc += this->binToHex(_CRC.at(3), _CRC.at(4), _CRC.at(5), _CRC.at(6));
		crc += this->binToHex(_CRC.at(7), _CRC.at(8), _CRC.at(9), _CRC.at(10));
		crc += this->binToHex(_CRC.at(11), _CRC.at(12), _CRC.at(13), _CRC.at(14));
		return crc;
	}

	string Frame::getType() const {
		string type;
		if (_RTR) { type = "Remote frame"; }
		else { type = "Data frame"; }
		return type;
	}

	string Frame::binToHex(bool a, bool b, bool c, bool d) const {
		string letter;
		if (a) {
			if (b) {
				if (c) {
					if (d) { //1111
						letter = "F";
					}
					else { //1110
						letter = "E";
					}
				}
				else {
					if (d) { //1101
						letter = "D";
					}
					else { //1100
						letter = "C";
					}
				}
			}
			else {
				if (c) {
					if (d) { //1011
						letter = "B";
					}
					else { //1010
						letter = "A";
					}
				}
				else { //1001
					if (d) {
						letter = "9";
					}
					else { //1000
						letter = "8";
					}
				}
			}
		}
		else {
			if (b) {
				if (c) {
					if (d) { //0111
						letter = "7";
					}
					else { //0110
						letter = "6";
					}
				}
				else {
					if (d) { //0101
						letter = "5";
					}
					else { //0100
						letter = "4";
					}
				}
			}
			else {
				if (c) {
					if (d) { //0011
						letter = "3";
					}
					else { //0010
						letter = "2";
					}
				}
				else {
					if (d) { //0001
						letter = "1";
					}
					else { //0000
						letter = "0";
					}
				}
			}
		}
		return letter;
	}
}
