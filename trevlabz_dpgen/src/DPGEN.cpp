// DPGEN.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Trevor LaBanz
// ECE574A Assignment 2

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <tuple>
#include <vector>

#include "dpgen.h"

using namespace std;

vector<tuple<string, vector<string>, string, bool>> graph;		//<name, inputs, output, scheduled> 


int main(int argc, char *argv[]) {

	if (argc < 3) {
		cout << "Usage: " << argv[0] << " netlistFile verilogFile" << endl;
		return 1;
	}

	ifstream iFile(argv[1]);
	if (iFile.fail()) {
		cerr << "Input file " << argv[1] << " failed to open" << endl;
		return 1;
	}
	else if (iFile.peek() == std::ifstream::traits_type::eof()) {
		cerr << "Input File is empty!" << endl;
		return 1;
	}


	string inputFile = argv[1];
	string tempFile = "temp.txt";
	string outputFile = argv[2];

	if (outputFile.substr(outputFile.length() - 2, outputFile.length()) != ".v") {
		cerr << "Output file does not have .v extension" << endl;
		return 1;
	}


	ofstream tFile(tempFile);

	string currentLine;
	string currentWord;


	//create input/output/wire info arrays
	string inputNames[16];
	int inputSize[16];
	bool inputIsSigned[16];		//true denotes signed
	int inputCount = 0;

	string outputNames[16];
	int outputSize[16];
	bool outputIsSigned[16];	//true denotes signed
	int outputCount = 0;

	string wireNames[32];
	int wireSize[32];
	bool wireIsSigned[32];	//true denotes signed
	int wireCount = 0;

	string regNames[16];
	int regCount = 0;
	

	//need register array

	int componentCount = 0;
	bool containsRegister = false;
	
	while ( getline(iFile,currentLine)) {

		stringstream  lineStream(currentLine);
		
		string outputLine = "";									//reset outputLine variable
				
		lineStream >> currentWord;

		if (currentLine.empty()) {							//handle empty lines
			tFile << '\n';
		}

		else if ((currentLine.substr(0,2) == "//")) {		//handle comment lines
			tFile << '\n';
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INPUT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		else if (currentWord == "input") {		//handle input lines
			outputLine.append("input ");
			
			lineStream >> currentWord;

			bool isSigned;
			int dataSize;

			if (currentWord.substr(0, 3) == "Int") {		//determine if signed or unsigned int
				outputLine.append("signed ");
				isSigned = true;
				dataSize = stoi(currentWord.substr(3, 2));
							   				 
			}
			else if(currentWord.substr(0, 4) == "UInt"){
				isSigned = false;
				dataSize = stoi(currentWord.substr(4, 2));
			}
			else {
				cerr << "Unknown input dataType" << endl;			//unknown type
				return 1;
			}

			if (dataSize > 1) {																//if datasize >1 convert to "[dataSize-1:0]"
				outputLine.append("[" + to_string(dataSize - 1) + ":0] ");
			}
			
			bool isFirst = true;

			while (lineStream >> currentWord) {			//create inputs in array
				if (currentWord.substr(0,2) == "//") {
					break;
				}
				else if (isFirst == true) {
					isFirst = false;
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					inputNames[inputCount] = currentWord;
					inputIsSigned[inputCount] = isSigned;
					inputSize[inputCount] = dataSize;

					outputLine.append(currentWord);
					inputCount++;
				}
				else{
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					inputNames[inputCount] = currentWord;
					inputIsSigned[inputCount] = isSigned;
					inputSize[inputCount] = dataSize;
					
					outputLine.append(", " + currentWord);

					inputCount++;
				}

			}

			outputLine.append(";");
			tFile << outputLine << '\n';		
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~OUTPUT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		else if (currentWord == "output") {		//handle output lines
			outputLine.append("output ");

			lineStream >> currentWord;

			bool isSigned;
			int dataSize;

			if (currentWord.substr(0, 3) == "Int") {		//determine if signed or unsigned int
				outputLine.append("signed ");
				isSigned = true;
				dataSize = stoi(currentWord.substr(3, 2));

			}
			else if (currentWord.substr(0, 4) == "UInt") {
				isSigned = false;
				dataSize = stoi(currentWord.substr(4, 2));
			}
			else {
				cerr << "Unknown output dataType" << endl;			//unknown type
				return 1;
			}

			if (dataSize > 1) {																//if datasize >1 convert to "[dataSize-1:0]"
				outputLine.append("[" + to_string(dataSize - 1) + ":0] ");
			}

			bool isFirst = true;

			while (lineStream >> currentWord) {			//create inputs in array
				if (currentWord.substr(0, 2) == "//") {
					break;
				}
				else if (isFirst == true) {
					isFirst = false;
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					outputNames[outputCount] = currentWord;
					outputIsSigned[outputCount] = isSigned;
					outputSize[outputCount] = dataSize;

					outputLine.append(currentWord);
					outputCount++;
				}
				else {
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					outputNames[outputCount] = currentWord;
					outputIsSigned[outputCount] = isSigned;
					outputSize[outputCount] = dataSize;

					outputLine.append(", " + currentWord);

					outputCount++;
				}

			}
			outputLine.append(";");
			tFile << outputLine << '\n';
		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~WIRE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		else if (currentWord == "wire") {		//handle wire declaration lines
			outputLine.append("wire ");

			lineStream >> currentWord;

			bool isSigned;
			int dataSize;

			if (currentWord.substr(0, 3) == "Int") {		//determine if signed or unsigned int
				outputLine.append("signed ");
				isSigned = true;
				dataSize = stoi(currentWord.substr(3, 2));

			}
			else if (currentWord.substr(0, 4) == "UInt") {
				isSigned = false;
				dataSize = stoi(currentWord.substr(4, 2));
			}
			else {
				cerr << "Unknown wire dataType" << endl;			//unknown type
				return 1;
			}

			if (dataSize > 1) {																//if datasize >1 convert to "[dataSize-1:0]"
				outputLine.append("[" + to_string(dataSize - 1) + ":0] ");
			}

			bool isFirst = true;

			while (lineStream >> currentWord) {			//create wire array
				if (currentWord.substr(0, 2) == "//") {
					break;
				}
				else if (isFirst == true) {
					isFirst = false;
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					wireNames[wireCount] = currentWord;
					wireIsSigned[wireCount] = isSigned;
					wireSize[wireCount] = dataSize;

					outputLine.append(currentWord);
					wireCount++;
				}
				else {
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					wireNames[wireCount] = currentWord;
					wireIsSigned[wireCount] = isSigned;
					wireSize[wireCount] = dataSize;

					outputLine.append(", " + currentWord);

					wireCount++;
				}

			}

			outputLine.append(";");
			tFile << outputLine << '\n';
			}
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~REGISTER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		else if (currentWord == "register") {		//handle register as wire declaration
			containsRegister = true;
			outputLine.append("wire ");

			lineStream >> currentWord;

			bool isSigned;
			int dataSize;

			if (currentWord.substr(0, 3) == "Int") {		//determine if signed or unsigned int
				outputLine.append("signed ");
				isSigned = true;
				dataSize = stoi(currentWord.substr(3, 2));

			}
			else if (currentWord.substr(0, 4) == "UInt") {
				isSigned = false;
				dataSize = stoi(currentWord.substr(4, 2));
			}
			else {
				cerr << "Unknown wire dataType" << endl;			//unknown type
				return 1;
			}

			if (dataSize > 1) {																//if datasize >1 convert to "[dataSize-1:0]"
				outputLine.append("[" + to_string(dataSize - 1) + ":0] ");
			}

			bool isFirst = true;

			while (lineStream >> currentWord) {			//create wire array
				if (currentWord.substr(0, 2) == "//") {
					break;
				}
				else if (isFirst == true) {
					isFirst = false;
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					wireNames[wireCount] = currentWord;
					regNames[regCount] = currentWord;
					wireIsSigned[wireCount] = isSigned;
					wireSize[wireCount] = dataSize;

					outputLine.append(currentWord);
					wireCount++;
					regCount++;
				}
				else {
					string a = ",";
					removeSubstrs(currentWord, a);	//remove commas

					wireNames[wireCount] = currentWord;
					regNames[regCount] = currentWord;
					wireIsSigned[wireCount] = isSigned;
					wireSize[wireCount] = dataSize;

					outputLine.append(", " + currentWord);

					wireCount++;
					regCount++;
				}

			}

			outputLine.append(";");
			tFile << outputLine << '\n';
		}



		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~COMPONENTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		else if (currentLine.find("=") != std::string::npos) {

					
			string compOut = currentWord;			//set compOut variable to component output wire
			int compWidth = 0;
			bool compIsSigned = false;


			int i = findIndex(outputNames, 16, compOut);		//search output name array for output
			if (i == -1) {
				i = findIndex(wireNames, 32, compOut);			//search wire name array for wire
				if (i == -1) {
					cerr << "Undefined output/wire: "<< compOut << endl;
					return 1;
				}
				else {											//set component size to found wire size and set component signed/unsigned
					compWidth = wireSize[i];
					compIsSigned = wireIsSigned[i];
				}
			}
			else {										//set component size to found output size and set component signed/unsigned
				compWidth = outputSize[i];
				compIsSigned = outputIsSigned[i];
			}



			string compIn1 = "";
			string compIn1_orig = "";
			int compIn1Width = 0;
			bool compIn1IsSigned = false;

			string compIn2 = "";
			string compIn2_orig = "";
			int compIn2Width = 0;
			bool compIn2IsSigned = false;

			string compIn3 = "";
			string compIn3_orig = "";
			int compIn3Width = 0;
			bool compIn3IsSigned = false;
			
			lineStream >> currentWord;				//move to what should be "=" char


			if (currentWord != "=") {
				cerr << "Unable to parse line: "<< currentLine << endl;
				return 1;
			}
			else {
				lineStream >> currentWord;			//move to first component input

				compIn1 = currentWord;

				int i = findIndex(inputNames, 16, compIn1);		//search input name array for input
				if (i == -1) {
					i = findIndex(wireNames, 32, compIn1);			//search wire name array for wire
					if (i == -1) {
						cerr << "Undefined input/wire: " << compIn1 << endl;
						return 1;
					}
					else {											//set input size and signed/unsigned
						compIn1Width = wireSize[i];
						compIn1IsSigned = wireIsSigned[i];
					}
				}
				else {										//set input size and signed/unsigned
					compIn1Width = inputSize[i];
					compIn1IsSigned = inputIsSigned[i];
				}

				compIn1_orig = compIn1;
				if (!compIn1IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
					compIn1 = "$signed({1'b0," + compIn1 + "})";
				}


				lineStream >> currentWord;				//move to operator

				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ADD/INC~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				if (currentWord == "+") {
					lineStream >> currentWord;

					if (currentWord == "1") {			//if inc
						if (compIsSigned) {				//if signed append S
							outputLine.append("S");
						}
						outputLine.append("INC #(.DATAWIDTH(" + to_string(compWidth) + ")) INC" + to_string(componentCount) + "(" + compIn1 + ", " + compOut + ");");	//incrementer declaration
						graph.push_back(make_tuple("INC" + to_string(compWidth), vector<string>{compIn1_orig}, compOut, false));	//add component to graph
					}
					else {
						compIn2 = currentWord;
						int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
						if (i == -1) {
							i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
							if (i == -1) {
								cerr << "Undefined input/wire: " << compIn2 << endl;
								return 1;
							}
							else {											//set input size and signed/unsigned
								compIn2Width = wireSize[i];
								compIn2IsSigned = wireIsSigned[i];
							}
						}
						else {										//set input size and signed/unsigned
							compIn2Width = inputSize[i];
							compIn2IsSigned = inputIsSigned[i];
						}
						compIn2_orig = compIn2;
						if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
							compIn2 = "$signed({1'b0," + compIn2 + "})";
						}

						if (compIsSigned) {				//if signed append S
							outputLine.append("S");
						}
						outputLine.append("ADD #(.DATAWIDTH(" + to_string(compWidth) + ")) ADD" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ");");
						graph.push_back(make_tuple("ADD" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph
					}

					tFile << outputLine << '\n';
					componentCount++;
				}

				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SUB/DEC~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "-") {
					lineStream >> currentWord;

					if (currentWord == "1") {			//if dec
						if (compIsSigned) {				//if signed append S
							outputLine.append("S");
						}
						outputLine.append("DEC #(.DATAWIDTH(" + to_string(compWidth) + ")) DEC" + to_string(componentCount) + "(" + compIn1 + ", " + compOut + ");");	//incrementer declaration
						graph.push_back(make_tuple("DEC" + to_string(compWidth), vector<string>{compIn1_orig}, compOut, false));	//add component to graph
					}
					else {
						compIn2 = currentWord;
						int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
						if (i == -1) {
							i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
							if (i == -1) {
								cerr << "Undefined input/wire: " << compIn2 << endl;
								return 1;
							}
							else {											//set input size and signed/unsigned
								compIn2Width = wireSize[i];
								compIn2IsSigned = wireIsSigned[i];
							}
						}
						else {										//set input size and signed/unsigned
							compIn2Width = inputSize[i];
							compIn2IsSigned = inputIsSigned[i];
						}

						compIn2_orig = compIn2;
						if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
							compIn2 = "$signed({1'b0," + compIn2 + "})";
						}

						if (compIsSigned) {				//if signed append S
							outputLine.append("S");
						}
						outputLine.append("SUB #(.DATAWIDTH(" + to_string(compWidth) + ")) SUB" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ");");
						graph.push_back(make_tuple("SUB" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph
					}

					tFile << outputLine << '\n';
					componentCount++;
				}

				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MUL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "*") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("MUL #(.DATAWIDTH(" + to_string(compWidth) + ")) MUL" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ");");
					graph.push_back(make_tuple("MUL" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}


				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == ">") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("COMP #(.DATAWIDTH(" + to_string(compWidth) + ")) COMP" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ", ,);");
					graph.push_back(make_tuple("COMP" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}



				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "<") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("COMP #(.DATAWIDTH(" + to_string(compWidth) + ")) COMP" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", ," + compOut + ",);");
					graph.push_back(make_tuple("COMP" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}


				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~EQ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "==") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("COMP #(.DATAWIDTH(" + to_string(compWidth) + ")) COMP" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", , ," + compOut + ");");
					graph.push_back(make_tuple("COMP" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}

				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MUX2x1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "?") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					lineStream >> currentWord;
					if (currentWord != ":") {
						cerr << "Undefined component found in: " << currentLine << endl;
						return 1;
					}

					lineStream >> currentWord;
					compIn3 = currentWord;
					i = findIndex(inputNames, 16, compIn3);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn3);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn3 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn3Width = wireSize[i];
							compIn3IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn3Width = inputSize[i];
						compIn3IsSigned = inputIsSigned[i];
					}
					compIn3_orig = compIn3;
					if (!compIn3IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn3 = "$signed({1'b0," + compIn3 + "})";
					}


					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("MUX2x1 #(.DATAWIDTH(" + to_string(compWidth) + ")) MUX" + to_string(componentCount) + "(" + compIn3 + ", " + compIn2 + ", " + compIn1 + ", " + compOut + ");");
					graph.push_back(make_tuple("MUX" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig, compIn3_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}



				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SHR~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == ">>") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("SHR #(.DATAWIDTH(" + to_string(compWidth) + ")) SHR" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ");");
					graph.push_back(make_tuple("SHR" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}


				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SHL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "<<") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("SHL #(.DATAWIDTH(" + to_string(compWidth) + ")) SHL" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ");");
					graph.push_back(make_tuple("SHL" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}

				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DIV~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "/") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("DIV #(.DATAWIDTH(" + to_string(compWidth) + ")) DIV" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ");");
					graph.push_back(make_tuple("DIV" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}

				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MOD~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (currentWord == "%") {
					lineStream >> currentWord;

					compIn2 = currentWord;
					int i = findIndex(inputNames, 16, compIn2);		//search input name array for input
					if (i == -1) {
						i = findIndex(wireNames, 32, compIn2);			//search wire name array for wire
						if (i == -1) {
							cerr << "Undefined input/wire: " << compIn2 << endl;
							return 1;
						}
						else {											//set input size and signed/unsigned
							compIn2Width = wireSize[i];
							compIn2IsSigned = wireIsSigned[i];
						}
					}
					else {										//set input size and signed/unsigned
						compIn2Width = inputSize[i];
						compIn2IsSigned = inputIsSigned[i];
					}
					compIn2_orig = compIn2;
					if (!compIn2IsSigned && compIsSigned) {		//if component is signed and input unsigned then convert to signed
						compIn2 = "$signed({1'b0," + compIn2 + "})";
					}

					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("MOD #(.DATAWIDTH(" + to_string(compWidth) + ")) MOD" + to_string(componentCount) + "(" + compIn1 + ", " + compIn2 + ", " + compOut + ");");
					graph.push_back(make_tuple("MOD" + to_string(compWidth), vector<string>{compIn1_orig, compIn2_orig}, compOut, false));	//add component to graph

					tFile << outputLine << '\n';
					componentCount++;
				}

				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~REG~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				else if (lineStream.eof()) {		//check if end of line is reached
					if (compIsSigned) {				//if signed append S
						outputLine.append("S");
					}
					outputLine.append("REG #(.DATAWIDTH(" + to_string(compWidth) + ")) REG" + to_string(componentCount) + "(" + compIn1 + ", clk, rst, " + compOut + ");");
					graph.push_back(make_tuple("REG" + to_string(compWidth), vector<string>{compIn1_orig}, compOut, false));	//add component to graph
					containsRegister = true;
					tFile << outputLine << '\n';
					
					componentCount++;
				}
				
				else {
					cerr << "Undefined operator in line:" << currentLine << endl;
					return 1;
				}

			}
		}

		else {
			tFile << currentLine << '\n'; 		
		}

	}
	
	iFile.close();
	tFile.close();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~CREATE VERILOG OUTPUT FILE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//create .v file from temp.txt
	ifstream tiFile(tempFile);
	ofstream oFile(outputFile);

	currentLine = "";

	oFile << "`timescale 1ns / 1ns" << '\n' << '\n' << '\n';									//use a standard 1ns timescale
	oFile << "module " << outputFile.substr(0, (outputFile.length() - 2)) << "(";					//create module from output file name without .v extension
	if (containsRegister) {																		//append clk and reset if circuit contains register
		oFile << "clk, rst, ";
	}
	oFile << inputNames[0];																		//append input names
	for (int z = 1; z < (sizeof(inputNames) / sizeof(*inputNames)); z++) {
		if (inputNames[z] != "") {
			oFile << ", " << inputNames[z];
		}
	}
	for (int z = 0; z < (sizeof(outputNames) / sizeof(*outputNames)); z++) {						//append output names
		if (outputNames[z] != "") {
			oFile << ", " << outputNames[z];
		}
	}
	oFile << ");" << '\n';									//end module declaration
	
	if (containsRegister) {
		oFile << '\t' << "input clk, rst;" << '\n';
	}

	while (getline(tiFile, currentLine)) {					//add temp file contents to verilog file
		oFile << '\t' << currentLine << '\n';	
	}

	oFile << '\n' << "endmodule";

	tiFile.close();
	oFile.close();

	//if everything is working include following line to delete temp.txt file

	remove("temp.txt");
	cout << "Conversion completed. Output file: " << outputFile << endl;



	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~CALCULATE CRITICAL PATH~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	double criticalPathTime = 0;

	map<string, double> graphLatencies;

	vector<bool> scheduled;


	for (auto input : inputNames) {
		if (input != "") {
			graphLatencies[input] = 0;
		}
		else {
			break;
		}
	}
	for (auto reg : regNames) {
		if (reg != "") {
			graphLatencies[reg] = 0;
		}
		else {
			break;
		}
		
	}

	
	for (auto& node : graph) {							//for each component in graph
		if (!get<3>(node)) {							//if node is unscheduled
			vector<string> vins = get<1>(node);			//get component inputs
			bool ready = true;
			double maxInTime = 0;

			for (auto vin : vins) {					//for each input of component
				auto inputTime = graphLatencies.find(vin);
				if (inputTime == graphLatencies.end())
					ready = false;
				else {
					double t = inputTime->second;
					if (maxInTime < t) {		//if t is greater than current max input time then set maxInTime to t
						maxInTime = t;
					}
				}
			}


			if (ready) {
				get<3>(node) = true;
				//find out time
				double nodeOutTime = maxInTime + compLatencies.find(get<0>(node))->second;		//add component latency to max input time
				if (criticalPathTime < nodeOutTime) {		//if node output time is larger than current critical path time then update critical path time
					criticalPathTime = nodeOutTime;
				}
				graphLatencies[get<2>(node)] = nodeOutTime;
			}
		}
	}

	cout << "Critical Path : " << criticalPathTime << " ns" << endl;
	return 0;
}


void removeSubstrs(string& s, string& p) {
	string::size_type n = p.length();
	for (string::size_type i = s.find(p);
		i != string::npos;
		i = s.find(p))
		s.erase(i, n);
}


int findIndex(string arr[], int len, string seek) {
	for (int i = 0; i < len; ++i) {
		if (arr[i] == seek) return i;
	}
	return -1;
}