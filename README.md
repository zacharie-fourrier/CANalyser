# CANalyser
A simple CAN bus frame analyser


Command line launch :
.\CANalyser.exe [binary CAN frame - optional, will be asked if empty]

Exemple :
```
PS C:\User\Documents> .\CANalyser.exe 001010111011000010001010100010010000100101010011111011101011111010010111111111111

Stuffing bits are those :
001010111011000010001010100010010000100101010011111011101011111010010111111111111
                                                   ^           ^
Frame :
0010101110110000100010101000100100001001010100111111110101111110010111111111111
SOF = 0
IDE = 0 > Standard format
RTR = 0 > Data frame
Identifier = 01010111011 > 0x2BB
DLC = 0100 > 4 Bytes of Data
Data = 01010100010010000100101010011111 > 0x54484A9F
CRC = 111010111111001 > 0x75F9
```

Makefile for v1.0.0 :
```
g++ CANanlyst.cpp frame.cpp -o CANalyser.exe
```

Planned :
Command line arguments : -type (remote / data) -format (standard / extended) -data (returns data in decimal) -identifier (returns identifier in decimal)
Support for multiple frame at a time with text files.
