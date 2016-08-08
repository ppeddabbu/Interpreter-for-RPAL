
#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "LexicalAnalyzer.h"
#include "Constants.h"

using namespace std;

Lexar :: Lexar(char* filename) {
	fRef.open(filename, fstream::in);
}

Lexar::~Lexar() {
	//delocate if any memory allocated!
	//close opened file

	fRef.close();
}

// THIS METHOD SHOULD BE CALLED everytime nexttoken is called. check if fRef is going forward or not.
bool Lexar ::isOperSymbol(char c) {

	for (int i = 0; i < 27; i++) {
		if (operators[i] == c)
			return true;
	}
	return false;
}

string Lexar::getNextToken() {
	char *token = NULL;
	int tSize = 0;
//	string s(s1);
	fRef.peek();
	while (!fRef.eof()) {

		char c;
		fRef.get(c);
		tSize++;
		//IF comment encoutnered skip all n go to end of line, can be encounted
		if (c == '/') {

			if (fRef.peek() == '/') { //peek won't move pointer
				fRef.ignore(1024, '\n'); //? HOLE
				tSize = 0;
			}
			else {
				//it's an operator
				if (token == NULL)
					token = new char[OPT_MAX];
				token[tSize - 1] = c;
				break;
			}
		}
		//suppose blank, move forward
		else if (isblank(c) || c == '\n') {// works in/after c++11 only
			if (token == NULL) {
				tSize = 0; // reset
				continue;
			}
			else {
				//? return token is also fine...check
				break;
			}
		}
		else if (isalnum(c) || c == '_') {
			// 'let' 'let12'
			//more_token = (char*) realloc(token, tSize*sizeof(char));
			if (token == NULL)
				token = new char[LIT_MAX]; //HANDLE MORE CASE

			token[tSize - 1] = c;
			if (!isalnum(fRef.peek())) {
				if (fRef.peek() == '_')
					continue;
				break;
			}
		}
		else if (c == '(' || c == ')' || c == ',' || c == ';') {
			if (token == NULL)
				token = new char[2];
			token[tSize - 1] = c;
			break;
		}
		else if (c == '\'') {
			if (token == NULL)
				token = new char[STR_MAX];

			token[tSize - 1] = c;
			char tp;
			fRef.get(tp);
			while (tp != '\'') {
				tSize++;
				token[tSize - 1] = tp;
				fRef.get(tp);
				if (tp == '\\' && fRef.peek() == '\'') {
					tSize++;
					token[tSize - 1] = tp;
					fRef.get(tp);
					tSize++;
					token[tSize - 1] = tp;
					fRef.get(tp);
				}
			}
			tSize++;
			token[tSize - 1] = tp;
			break;
		}
		else if (isOperSymbol(c)) {
			if (token == NULL)
				token = new char[OPT_MAX];

			token[tSize - 1] = c;
			if (c == '-' && fRef.peek() == '>' || c == '>' && fRef.peek() == '='
			 || c == '<' && fRef.peek() == '=' || c == '*' && fRef.peek() == '*') { //peek
				tSize++;
				fRef.get(c);
				token[tSize - 1] = c;
			}
			break;
		}


	}
	if (token != NULL)
		token[tSize] = '\0';
	else
		return string("");

	return string(token);
}

bool Lexar :: isINT(string tk) {
	if (tk.length() == 0)
		return false;
	for (int i = 0; i < tk.length(); i++) {
		if (!isdigit(tk[i]))
			return false;
	}
	return true;
}

bool Lexar :: isSTR(string tk) {
	if (tk.length() == 0)
		return false;

	if (tk[0] == '\'')
		return true;

	return false;
}

bool Lexar :: isID(string tk) {
	if (tk.length() == 0)
		return false;
	//what is an id?
	// it is not a keyword.
	// it is not an operator/punct
	// or not true /false,dummy
	if ((tk.length() == 1 && ( isOperSymbol(tk[0]) || tk[0] == '(' || tk[0] == ')' || tk[0] == ',' || tk[0] == ';') )|| isKeyWord(tk))
		return false;
	return true;
}

bool Lexar :: isKeyWord(string tk) {
	for (int i = 0; i < 25; i++) {
		if (tk.compare(keywords[i]) == 0) {
			return true;
		}
	}
	return false;
}

bool Lexar::isOPerator(string tk) {
	for (int i = 0; i < 18; i++) {
		if (tk.compare(stOperators[i]) == 0) {
			return true;
		}
	}
	return false;
}
