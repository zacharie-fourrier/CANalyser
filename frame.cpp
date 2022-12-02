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
				unsigned crc = this->getDLC();
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
				unsigned dlc = this->getDLC();
				for (unsigned k = 0; k < dlc * 8; k++) {
					_Data.push_back(_Frame.at(k + 19));
				}
				for (unsigned k = 0; k < 15; k++) {
					_CRC.push_back(_Frame.at(k + 19 + (dlc * 8)));
				}
				_CRCD = _Frame.at(35 + (dlc * 8));
				_ACKS = _Frame.at(36 + (dlc * 8));
				_ACKD = _Frame.at(37 + (dlc * 8));
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
		ss << " > 0x" << hex << this->getIdentifier() << endl << "DLC = ";
		for (bool b : _DLC) {
			ss << b;
		}
		ss << " > " << this->getDLC() << " Bytes of Data" << endl;
		if (!_RTR) {
			ss << "Data = ";
			for (bool b : _Data) {
				ss << dec << b;
			}
			ss << " > 0x" << hex << this->getData() << endl;
		}
		else {
			ss << "Remote frame, no data" << endl;
		}
		ss << "CRC = ";
		for (bool b : _CRC) {
			ss << b;
		}
		ss << " > 0x" << hex << this->getCRC() << endl;
		return ss.str();
	}

	unsigned Frame::getIdentifier() const {
		unsigned identifier = boolArrayToInt(_Identifier);
		return identifier;
	}

	unsigned Frame::getDLC() const {
		unsigned data = boolArrayToInt(_DLC);
		return data;
	}

	unsigned Frame::getData() const {
		unsigned data = boolArrayToInt(_Data);
		return data;
	}

	string Frame::getFormat() const {
		string format;
		if (_IDE) { format = "Extended format"; }
		else { format = "Standard format"; }
		return format;
	}

	unsigned Frame::getCRC() const {
		unsigned crc = boolArrayToInt(_CRC);
		return crc;
	}

	string Frame::getType() const {
		string type;
		if (_RTR) { type = "Remote frame"; }
		else { type = "Data frame"; }
		return type;
	}

	unsigned Frame::boolArrayToInt(vector<bool> b) const {
		unsigned res = 0;
		for (unsigned i = 0; i < b.size(); i++) {
			res += b.at(i) << (b.size() - i - 1);
		}
		return res;
	}
}
