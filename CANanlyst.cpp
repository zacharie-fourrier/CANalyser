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
			Frame CAN(frame);
			cout << CAN.toString() << endl;
		}
		else if (regex_match(frame, m2, r2)) { //text file
			frameValid = 1;

		}
		else if (frame == "-h" || frame == "--help") { //help menu
			cout << "CANalyst.exe [frame] [options]\n\n"
				"List of options :\n"
				"[frame]     : Either a binary CAN frame or a text file containing several of them (1 per line)\n"
				"-h --help   : Display help menu\n"
				"-v --version: Displays current version\n"
				"-o [path]   : Will write the output in a file\n"
				"              Currently supports JSON or TXT\n"
				"-D --Data   : Returns / Writes only the data as an output\n"
				"-ID         : Returns / Writes only the Identifier as an output\n"
				"-"
				<< endl;
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
}