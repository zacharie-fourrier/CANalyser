#include <iostream>
#include <string>
#include <regex>
#include "frame.h"

using namespace std;
using namespace can;

int main(int argc, char * argv[]) {

	string frame;
	bool ui = 0, frameValid = 0;

	if (argv[1]) {
		frame = argv[1];
	}
	else {
		cout << "Binary CAN frame :" << endl;
		cin >> frame;
		ui = 1;
	}

	regex r1 (R"([0,1]+)");
	smatch m1;
	regex r2 (R"(.+\.txt)");
	smatch m2;

	do {
		if (regex_match(frame, m1, r1)) { //binary frame
			frameValid = 1;

		}
		else if (regex_match(frame, m2, r2)) { //text file
			frameValid = 1;

		}
		else { //neither a binary frame nor a text file
			cout << "Invalid CAN frame format, please use a valid CAN frame." << endl;
			if (!ui) {
				return -1; //Invalid frame format error code
			}
			else { //ask for frame or text file again, only if not using command lines arguments
				cout << "Binary CAN frame :" << endl;
				cin >> frame;
			}
		}
	} while (!frameValid);
	
	Frame CAN(frame);
	cout << CAN.toString() << endl;

}