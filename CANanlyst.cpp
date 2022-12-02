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

	short unsigned returnType = 0;
	string outputPath = "./output.txt";
	bool outputType = 0; //0 = console print, 1 = text file;

	for (int i = 2; i < argc; i++) {
		if (argv[i] == "-t" || argv[i] == "--type") {
			++i;
			if (argv[i] == "Data") { returnType = 1; }
			else if (argv[i] == "ID") { returnType = 2; }
			else if (argv[i] == "CRC") { returnType = 3; }
			else if (argv[i] == "DLC") { returnType = 4; }
			else if (argv[i] == "IDE") { returnType = 5; }
			else if (argv[i] == "RTR") { returnType = 6; }
		}
		else if (argv[i] == "-o" || argv[i] == "--output") {
			outputPath = argv[++i];
		}
	}

	regex r1 (R"([0,1]+)");
	smatch m1;
	regex r2 (R"(.+\.txt)");
	smatch m2;

	do {
		if (regex_match(frame, m1, r1)) { //binary frame
			frameValid = 1;

			Frame CAN(frame);
			
			unsigned code = 0;

			switch (returnType) {
			case 0: //print Everything
				cout << CAN.toString() << endl;
				code = 1;
				break;

			case 1: //print Data
				code = CAN.getData();
				break;

			case 2: //print ID
				code = CAN.getIdentifier();
				break;

			case 3: //print CRC
				code = CAN.getCRC();
				break;

			case 4: //print DLC
				code = CAN.getDLC();
				break;

			case 5: //print IDE
				code = CAN.getIDE();
				break;

			case 6: //print RTR
				code = CAN.getRTR();
				break;
			}
			return code;
		}
		else if (regex_match(frame, m2, r2)) { //text file
			frameValid = 1;

		}
		else if (frame == "-h" || frame == "--help") { //help menu
			cout << "CANalyst.exe [frame] [options]\n\n"
				"List of options :\n"
				"[frame]              : Either a binary CAN frame or a text file containing several of them (1 per line)\n"
				"-h --help            : Display help menu\n"
				"-v --version         : Displays current version\n"
				"-o --output [path]   : Will write the output in a file\n"
				"                       Currently supports JSON or TXT\n"
				"-t --type [value]    : Returns / Writes only the value of the specified field\n"
				"                       Possibilities are Data / ID / CRC / DLC / IDE / RTR (Case sensitive)"
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

	/*
	Frame CAN(frame);
	cout << CAN.toString() << endl;
	*/
}