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
	std::string getIdentifier() const;
	int getDLC() const;
	std::string getData() const;
	std::string getFormat() const;
	std::string getCRC() const;
	std::string getType() const;
	std::string binToHex(bool a, bool b, bool c, bool d) const;
};
}