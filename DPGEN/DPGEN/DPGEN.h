#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <tuple>
#include <vector>

using namespace std;

const map<string, double> compLatencies = {
	{ "REG1", 2.616 },{ "REG2", 2.644 } ,{ "REG8", 2.879 },{ "REG16",3.061 },{ "REG32",3.602 },{ "REG64",3.966 },
	{ "ADD1", 2.704 },{ "ADD2", 3.713 } ,{ "ADD8", 4.924 },{ "ADD16",5.638 },{ "ADD32",7.270 },{ "ADD64",9.566 },
	{ "SUB1", 3.024 },{ "SUB2", 3.412 } ,{ "SUB8", 4.890 },{ "SUB16",5.569 },{ "SUB32",7.253 },{ "SUB64",9.566 },
	{ "MUL1", 2.438 },{ "MUL2", 3.651 } ,{ "MUL8", 7.453 },{ "MUL16",7.811 },{ "MUL32",12.395 },{ "MUL64",15.354 },
	{ "COMP1", 3.031 },{ "COMP2", 3.934 } ,{ "COMP8", 5.949 },{ "COMP16",6.256 },{ "COMP32",7.264 },{ "COMP64",8.416 },
	{ "MUX1", 4.083 },{ "MUX2", 4.115 } ,{ "MUX8", 4.815 },{ "MUX16",5.623 },{ "MUX32",8.079 },{ "MUX64",8.766 },
	{ "SHR1", 3.644 },{ "SHR2", 4.007 } ,{ "SHR8", 5.178 },{ "SHR16",6.460 },{ "SHR32",8.819 },{ "SHR64",11.095 },
	{ "SHL1", 3.614 },{ "SHL2", 3.980 } ,{ "SHL8", 5.152 },{ "SHL16",6.549 },{ "SHL32",8.565 },{ "SHL64",11.220 },
	{ "DIV1", 0.619 },{ "DIV2", 2.144 } ,{ "DIV8", 15.439 },{ "DIV16",33.093 },{ "DIV32",86.312 },{ "DIV64",243.233 },
	{ "MOD1", 0.758 },{ "MOD2", 2.149 } ,{ "MOD8", 16.078 },{ "MOD16",35.563 },{ "MOD32",88.142 },{ "MOD64",250.583 },
	{ "INC1", 1.792 },{ "INC2", 2.218 } ,{ "INC8", 3.111 },{ "INC16",3.471 },{ "INC32",4.347 },{ "INC64",6.200 },
	{ "DEC1", 1.792 },{ "DEC2", 2.218 } ,{ "DEC8", 3.108 },{ "DEC16",3.701 },{ "DEC32",4.685 },{ "DEC64",6.503 },
};

void removeSubstrs(string& s, string& p);

int findIndex(string arr[], int len, string seek);

