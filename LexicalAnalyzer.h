#pragma once
#ifndef LEXICALANALYZER_H

#include<string>
#include <iostream>
#include <fstream>

using namespace std;

class Lexar {

public:
	Lexar(char* filename);
	~Lexar();

	string getNextToken();
	bool isINT(string tk);
	bool isSTR(string tk);
	bool isID(string tk);
	bool isOPerator(string tk);
	fstream fRef;

private:

	bool isOperSymbol(char c);
	bool isKeyWord(string tk);


};
#endif // !LEXICALANALYZER_H
