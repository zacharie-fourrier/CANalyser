#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

namespace can
{
class Frame
{
private:
	std::vector<bool> _Frame, _Identifier, _DLC, _Data, _CRC;
	bool _SOF, _IDE, _RTR, _R0, _R1, _SRR, _CRCD, _ACKS, _ACKD;
	std::vector<unsigned> _stuffing;
	std::string _String;
public:
	Frame(std::string str);
	std::string toString() const;
	unsigned getIdentifier() const;
	unsigned getDLC() const;
	unsigned getData() const;
	std::string getFormat() const;
	unsigned getCRC() const;
	std::string getType() const;
	bool getIDE() const;
	bool getRTR() const;
	unsigned boolArrayToInt(std::vector<bool> b) const;
};
}