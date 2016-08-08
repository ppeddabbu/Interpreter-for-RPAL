#pragma once
#ifndef EXECUTION_H
#define EXECUTION_H

#include <vector>
#include <queue>
#include <string>
#include <map>


#include "stdafx.h"
#include "BinaryTree.h"
#include "LexicalAnalyzer.h"

using namespace std;

/*struct Token {
	string type;
	string token;
};*/

class Execution {

public:

	Execution(TNode * r, Lexar *l);
	~Execution();

	void PreOrderbreakST();
	queue<string> getControlsStruct();
	void CSEM();

private:
	int mnextdeltaVal;
	int mpresentEnv;
	Lexar *mLex;
	TNode *mstRoot;
	map<int,int> envMap;
	map<int, vector<string>> delMp;
	queue<TNode *> mcsList;
	queue<string> mdeltaQ;
	pair<int,string> keytuple;
	int envCount;
	stack<int> envStk;
	void PreOrderbreakST(TNode *stRoot);
	void genCS(TNode *r);
	void ExecuteCS(string &str, stack <string> &e, stack <string> &c);
	string applyRatorRand(string &ctk, string &ftk, string &stk);
	void executeCSEM();

};

#endif // !EXECUTION_H
