#include <iostream>
#include <string>
#include <regex>
#include "frame.h"

using namespace std;
using namespace can;

int main(int argc, char * argv[]) {

	string frame;

	if (argv[1]) {
		frame = argv[1];
	}
	else {
		cout << "Binary CAN frame :" << endl;
		cin >> frame;
	}

	regex r (R"([^0,1]+)");
	smatch m;

	if (regex_search(frame, m, r)) {
		cout << "Invalid CAN frame format, please use a valid CAN frame." << endl;
		return 1; //Invalid frame format
	}
	
	Frame CAN(frame);
	cout << CAN.toString() << endl;

}