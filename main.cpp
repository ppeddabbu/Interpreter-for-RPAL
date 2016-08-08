// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "BinaryTree.h"
#include "LexicalAnalyzer.h"
#include "Execution.h"

using namespace std;

void E();
void D();
void read(string);

Lexar *l;
BlrTree *bt;
Execution *exeInterpreter;
string NT;

void Vl() {
	read(NT);
		//TO DO while,then read =
	if (NT.compare(",") == 0) {
		int n = 0;
		do {
			n++;
			read(",");
			if (l->isID(NT))
				read(NT); //read id
			else
			{
				cerr << "Error : ID expected" << endl;
				exit(1);
			}

		} while (NT.compare(",") == 0);
		bt->buildTree(",", n + 1);
	}
}
void Vb() {

	if (l->isID(NT)) {
		read(NT);
	}
	else if (NT.compare("(") == 0) {
		read("(");
		if (NT.compare(")") == 0)
		{
			read(")");
			bt->buildTree("()", 2);
		}
		else {
			Vl();
			read(")");
		}
	}
}

void Db() {

	if (l->isID(NT)) {
		read(NT);
      if (l->isID(NT)) {
			int n = 0;
			do {
				n++;
				Vb();
			} while (l->isID(NT) || NT.compare("(")==0);
			//cout << "function_form" << endl;
			read("=");
			E();
			bt->buildTree("function_form", n + 2);
		  }
	  else if (NT.compare("(") == 0) {
		 int n = 0;
		  do {
			  n++;
			  read("(");
			  if (NT.compare(")") == 0)
			  {
				  read(")");
				  bt->buildTree("()", 0); 
			  }
			  else {
				  Vl();
				  read(")");
			  }
		  } while (NT.compare("(") == 0);
		  read("=");
		  E();
		  bt->buildTree("function_form", n + 2);
	  }
	else{
		if (NT.compare(",") == 0) {
			//TO DO while,then read =
			int n = 0;
			do {
				n++;
				read(",");
				if (l->isID(NT))
					read(NT); //read id
				else
				{
					cerr << "Error : ID expected" << endl;
					exit(1);
				}

			} while (NT.compare(",") == 0);
			bt->buildTree(",", n + 1);
		}
		//if (NT.compare("=") == 0) { // Is this fixes the problem?
			read("=");
			E();
			bt->buildTree("=", 2);
		//}
		}
		}
	    else if (NT.compare("(") == 0) {
		read("(");
		D();
		read(")");
	}
}

void Dr() {
	if (NT.compare("rec") == 0) {
		read("rec");
		Db();
		bt->buildTree("rec", 1);
		//cout << "rec" << endl;
	}
	else
		Db();
}

void Da() {
	Dr();
	if (NT.compare("and") == 0) {
		int n = 0;
		do {
			n++;
			read("and");
			Dr();
		} while (NT.compare("and") == 0);
		bt->buildTree("and", n + 1);
		//cout << "and" << endl;
	}
}

void D() {
	Da();
	if (NT.compare("within") == 0) {
		read("within");
		D();
		bt->buildTree("within", 2);
		//cout << "within" << endl;
	}
}
void Rn() {
	string temp;
	if (l->isID(NT) || l->isSTR(NT) || l->isID(NT)) {
		read(NT);
	}
	else if (NT.compare("true") == 0 || NT.compare("false") == 0 || NT.compare("nil") == 0 || NT.compare("dummy") == 0) {
		temp = NT;
		read(temp);
		string s = "<";
		s.append(temp);
		s.append(">");
		bt->buildTree(s, 0);
	}
	else if (NT.compare("(") == 0) {
		read("(");
		E();
		read(")");
	}
	else if (NT.compare("") == 0) {
	}
	else {
		cout << "no leaf node matched \'"<< NT << "\' token" << endl;
		exit(1);
	}
}

void R() {
	Rn();
	//while next token
	if (NT.compare("(") == 0 || l->isID(NT) || l->isSTR(NT) || l->isID(NT)
		|| NT.compare("true") == 0 || NT.compare("false") == 0 || NT.compare("nil") == 0 || NT.compare("dummy") == 0) {
		//int n = 0;
		do {
			//n++;
			Rn();
			bt->buildTree("gamma", 2);
			//cout << "gamma" << endl;
		} while (NT.compare("(") == 0 || l->isID(NT) || l->isSTR(NT) || l->isID(NT)
			|| NT.compare("true") == 0 || NT.compare("false") == 0 || NT.compare("nil") == 0 || NT.compare("dummy") == 0);

	}
}

void Ap() {
	R();
	if (NT.compare("@") == 0) {
		int n = 0;
		while (NT.compare("@") == 0) {
			read("@");
			if (l->isID(NT)){
				read(NT);
			    R();
		    }
			else {
				cerr << "Next token is not ID" << endl;
				exit(1);
			}
			bt->buildTree("@", 3);
		} 
	}
}
void Af() {
	Ap();
	if (NT.compare("**") == 0) {
		read("**");
		Ap();
		bt->buildTree("**", 2);
	}
}

void At() {
	Af();
	string temp;
	while (NT.compare("*") == 0 || NT.compare("/") == 0) {
		temp = NT;
		read(NT);
		Af();
		bt->buildTree(temp, 2);
	}
}

void A() {
	if (NT.compare("+") == 0) {
		read("+");
		At();
	}
	else if (NT.compare("-") == 0) {
		read("-");
		At();
		bt->buildTree("neg", 1);
		//cout << "neg" << endl;
	}
	else {
		At();
		string temp;
		while (NT.compare("+") == 0 || NT.compare("-") == 0) {
			temp = NT;
			read(temp);
			At();
			bt->buildTree(temp, 2);
		}
	}

}
void Bp() {
	A();
	if (NT.compare("gr") == 0) {
		read("gr");
		A();
		bt->buildTree("gr", 2);
	}
	else if (NT.compare(">") == 0) {
		read(">");
		A();
		bt->buildTree("gr", 2);
	}
	else if (NT.compare("ge") == 0) {
		read("ge");
		A();
		bt->buildTree("ge", 2);
	}
	else if (NT.compare(">=") == 0) {
		read(">=");
		A();
		bt->buildTree("ge", 2);
	}
	else if (NT.compare("ls") == 0) {
		read("ls");
		A();
		bt->buildTree("ls", 2);
	}
	else if (NT.compare("<") == 0) {
		read("<");
		A();
		bt->buildTree("ls", 2);
	}
	else if (NT.compare("le") == 0) {
		read("le");
		A();
		bt->buildTree("le", 2);
	}
	else if (NT.compare("<=") == 0) {
		read("<=");
		A();
		bt->buildTree("le", 2);
	}
	else if (NT.compare("eq") == 0) {
		read("eq");
		A();
		bt->buildTree("eq", 2);
	}
	else if (NT.compare("ne") == 0) {
		read("ne");
		A();
		bt->buildTree("ne", 2);
	}
}

void Bs() {
	if (NT.compare("not") == 0) {
		read("not");
		Bp();
		bt->buildTree("not", 1);
	}
	else
		Bp();
}

void Bt() {
	Bs();
	while (NT.compare("&") == 0) {
		read("&");
		Bs();
		bt->buildTree("&", 2);
	}
}

void B() {
	Bt();
	while (NT.compare("or") == 0) {
		read("or");
		Bt();
		bt->buildTree("or", 2);
	}
}

void Tc() {
	B();
	if (NT.compare("->") == 0) {
		read("->");
		Tc();
		read("|");
		Tc();
		bt->buildTree("->", 3); //TODO is it 4, mostly no?
	}
}

void Ta() {
	Tc();
	while (NT.compare("aug") == 0) {
		read("aug");
		Tc();
		bt->buildTree("aug", 2);
		//cout << "aug" << endl;
	}
}

void T() {
	Ta();
	if (NT.compare(",") == 0) {
		int n = 0;
		do {
			n++;
			read(",");
			Ta();
		} while (NT.compare(",") == 0);
		bt->buildTree("tau", n + 1);
		//cout << "tau" << endl;
	}
}
void Ew() {
	T();
	if (NT.compare("where") == 0) {
		read("where");
		Dr();
		bt->buildTree("where", 2);
		//cout << "where" << endl;
	}
}

void E() {
	if (NT.compare("let") == 0) {
		read("let");
		D(); //-> should push one terminal
		read("in");
		E(); //-> shpuld push one terminal
		bt->buildTree("let", 2);
	}
	else if (NT.compare("fn") == 0) {
		read("fn");
		int n = 0;
		do {
			Vb();
			n++;
		} while (NT.compare("(") == 0 || l->isID(NT));
		read(".");
		E();
		bt->buildTree("lambda", n + 1);
		//cout << "lambda" << endl;
	}
	else {
		Ew();
	}
}

void read(string tok) {
	if (NT.compare(tok) != 0) {
		cerr << "Expected '" << tok << "' but found '" << NT << "' " << endl;
		exit(1);
	}

	if (l->isINT(tok)) {

		//modify token at this phase, how is that?
		string s = "<INT:";
		s.append(tok);
		s.append(">");
		bt->buildTree(s, 0);
		//cout << s << endl;

	}
	else if (l->isSTR(tok)) {
		string s = "<STR:";
		s.append(tok);
		s.append(">");
		bt->buildTree(s, 0);
		//cout << s << endl;
	}
	else if (l->isID(tok)) {
		string s = "<ID:";
		s.append(tok);
		s.append(">");
		bt->buildTree(s, 0);
		//cout << s << endl;
	}

	NT = l->getNextToken();
}

int main(int parmc, char *parmv[])
{
	string arg1a = "-ast", arg1b="-st";
	if (parmc > 3 )
	{
		cout << "Please provide right set of arguments" << endl;
		cout << "Usage :> <Program_Name> [switch_flag] <Input_File>" << endl;
		cout << cin.get();
		exit(0);
	}

	l = new Lexar(parmv[2]);
	bt = new BlrTree();

	NT = l->getNextToken();

	E();

	if (arg1a.compare(parmv[1]) == 0) {
		bt->preOrderTravPrint();			//output ast
	}
	bt->modifyTree(); // Standardize tree

	if(arg1b.compare(parmv[1]) == 0) {
		bt->sTreePrint(); // out put Standardized tree
	}
	bt->setRoot();
	exeInterpreter = new Execution(bt->STRoot, l);
	//exeInterpreter->CSEM(); //to execute CSE Machine
	return 0;

}