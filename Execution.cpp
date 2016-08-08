#include "stdafx.h"
#include <iostream>
#include <queue>

#include "Execution.h"

using namespace std;
Execution::Execution(TNode * r, Lexar *l) {
	mstRoot = r;
	mnextdeltaVal = 0;
	mpresentEnv = 0;
	mLex = l;
}

Execution :: ~Execution() {

}

void Execution::executeCSEM() {
	envCount = 0;
	envMap = map<int, int>();
	envStk.push(0);
	PreOrderbreakST(mstRoot);
	queue <string> delQ = getControlsStruct();
	stack <string> execution;
	stack <string> control;
	execution.push(string("E.0"));
	control.push(string("E.0")); 

	while (delQ.size()!=0) {
		control.push(delQ.front());
		delQ.pop();

	}

	while (control.size() != 1) {
		ExecuteCS(control.top(),execution, control);
		control.pop();
	}
}

/*
* TO DO: Execute CS machine.
*/

void Execution::ExecuteCS(string &ctk,stack <string> &es, stack <string> &cs) {

	//first check operator.. it's a must to have this first, to not break the code
	if (mLex->isOPerator(ctk)) {
		//current, fsttoken, sec tkn
		string fT= es.top();es.pop();
		string sT= es.top();es.pop();
		string resultTk = applyRatorRand(ctk, fT, sT);
		es.push(resultTk);
	}
	else if (ctk.compare("neg") == 0) {
		string fT = es.top(); es.pop();
		int negVal = atoi(fT.data());
		negVal = -negVal;
		string newTk = to_string(negVal);
		es.push(newTk);
	}
	else if (ctk.compare("not") == 0) {
		string fT = es.top(); es.pop();

		if (fT.compare("true") == 0) 
			es.push("false");
		else
			es.push("true");
	}
	else if (ctk.compare("gamma")==0) {
		string fT = es.top(); es.pop();
		if (fT.at(0) == '<' && fT.at(1) == 'L') // I guess this one checks lamda closure?
		{
			string envTK = string("E").append(to_string(++envCount));
			envMap[envCount] = ctk.at(ctk.length()); //
			envStk.push(envCount);
			mpresentEnv = envCount;
		}
		else if (fT.compare("ystar")==0) {
			string tk = es.top();
			es.pop();
			tk = "alpha";
			es.push(tk);
		}
		else if (fT.compare("alpha")==0) {
			string tk = es.top(); es.pop();
			es.push(tk);
		}
		else if (fT.compare("Isinteger") == 0) {
			string tk = es.top(); es.pop();
			if (mLex->isINT(tk)) {
				es.push("true");
			}
			else
				es.push("false");
		}
		else if (fT.compare("Isstring") == 0) {
			string tk = es.top(); es.pop();
			if (mLex->isSTR(tk)) {
				es.push("true");
			}
			else
				es.push("false");
		}
		else if (fT.compare("Istruthvalue") == 0) {
			string tk = es.top(); es.pop();
			if (tk.compare("true")==0 || tk.compare("false") == 0) {
				es.push("true");
			}
			else
				es.push("false");
		}
		else if (fT.compare("Isdummy") == 0) {
			string tk = es.top(); es.pop();
			if (tk.compare("dummy") == 0) {
				es.push("true");
			}
			else
				es.push("false");
		}
		else if (fT.compare("Isfunction") == 0) {
			string tk = es.top(); es.pop();
			if (ctk.at(0) == '<' && ctk.at(1) == 'L'){ //  lamda closure check
				es.push("true");
			}
			else
				es.push("false");
		}
		else if (fT.compare("Order") == 0) {
			string tk = es.top(); es.pop();
				es.push(to_string(tk.size()));
		}
		else if (fT.compare("Null") == 0) {
			string tk = es.top(); es.pop();
			if (tk.compare("dummy") == 0) {
				es.push("true");
			}
			else
				es.push("false");
		}

		else if (fT.compare("Conc") == 0) {
			string ftk = es.top(); es.pop();
			string stk = es.top(); es.pop();
			string s = ftk.substr(1, ftk.size() - 2) + stk.substr(1, stk.size() - 2);

			s = "'" + s + "'";
			es.push(s);
			cs.pop();
		}
		else if (fT.compare("Stern")==0) {
			string tk = es.top(); es.pop();
			tk= tk.substr(2,tk.size()-3);
			tk = "'" + tk + "'";
			es.push(tk);
		}
		else if (fT.compare("Stem") == 0) {
			string tk = es.top();es.pop();

			tk = tk.substr(1,1);
			tk = "'" + tk + "'";
			es.push(tk);
		}
		else if (fT.compare("ItoS") == 0) {
			string tk = es.top(); es.pop();

			tk = tk.substr(1, 1);
			tk = "'" + tk + "'";
			es.push(tk);
		}
	}
	else if (mLex->isID(ctk)) // TO DO: check parameter)
	{
		string tk = es.top(); es.pop();
		es.push(tk);
	}
	else if (ctk.at(0) == 'E' && ctk.at(1) == '.') { //token  check for type env
		//current, fsttoken, sec tkn
		string fT = es.top(); es.pop();
		 es.pop();
		 es.push(fT);
		 envStk.pop();
		 mpresentEnv = envStk.top();
		//TO DO: take care of tuple cases
		//es.push(sT);
	}
	else if (ctk.compare("aug") == 0) {
		//current, fsttoken, sec tkn
		string fT = es.top(); es.pop();
		string sT = es.top(); es.pop();
		//TO DO: take care of tuple cases
			//es.push(sT);
	}
	else if (ctk.compare("nil") == 0) {
		es.push(ctk);
	}
	else if (ctk.at(0) == '<' && ctk.at(1) == 'L') //  lamda closure check
	{
		ctk.append(to_string(mpresentEnv));
		es.push(ctk);
	}
	else {
		es.push(ctk);
	}

//	else if () {
//	}
}

string Execution::applyRatorRand(string &ctk, string &ftk, string &stk) {
	if (mLex->isINT(ftk)) {
		int x = atoi(ftk.data());
		int y = atoi(stk.data());
		int out;
		bool b;
		if (ctk == "+") {
			out = x + y;
			return to_string(out);
		}else if (ctk == "-") {
			out = x - y;
			return to_string(out);
		}
		else if (ctk == "/") {
			out = x / y;
			return to_string(out);
		}
		else if (ctk == "*") {
			out = x * y;
			return to_string(out);
		}
		else if (ctk == "**") {
			out = pow(x, y);
			return to_string(out);
		}
		else if (ctk == "gr") {
			 b = x > y;
		}
		else if (ctk == "ge") {
			 b = x >= y;
		}
		else if (ctk == "ls") {
			 b = x < y;
		}
		else if (ctk == "le") {
			 b = x <= y;
		}
		else if (ctk == "eq") {
			 b = x == y;
		}
		else if (ctk == "ne") {
			 b = x != y;
		}

		if (b)
			return "true";
		else
			return "false";
	}
	else if (ftk.compare("true") == 0 || ftk.compare("true")==0) {
		
		if (ctk.compare("eq") == 0) { //check this?? not required becasue it will get only true/false
			if (ftk.compare("true") == 0 && stk.compare("true") == 0 ||
				ftk.compare("false") == 0 && stk.compare("false") == 0)
				return "true";
			else
				return "false";
		}
		else if (ctk.compare("&") == 0) {
			if (ftk.compare("true") == 0 && stk.compare("true") == 0)
				return "true";
			else
				return "false";
		}
		else if (ctk.compare("ne") == 0) {
			if (!(ftk.compare("true") == 0 && stk.compare("true") == 0) ||
				!(ftk.compare("false") == 0 && stk.compare("false") == 0))
				return "true";
			else
				return "false";
		}
		else if (ctk.compare("or") == 0) {
			if (ftk.compare("true") == 0 || stk.compare("true") == 0)
				return "true";
			else
				return "false";
		}
	}
	else if (mLex->isSTR(ftk)) {
		if (ctk.compare("eq") == 0) {
			if (ftk.compare(stk) == 0 )
				return "true";
			else
				return "false";
		}
		else if (ctk.compare("ne") == 0) {
			if (ftk.compare(stk) != 0)
				return "true";
			else
				return "false";
		}
	}
	return "";
}

void  Execution::CSEM() {
	//executeCSEM();
}

void Execution::PreOrderbreakST() {
	mcsList.push(mstRoot);
	PreOrderbreakST(mstRoot);

}

void Execution:: PreOrderbreakST(TNode *n){
	//TODO: take care of , -> node etc...
	if (n != NULL) {
		if (n->data.compare("lambda")) {
			//for lambda we can have  
			TNode *temp = n->left->right;

			n->left->right = NULL; //break the link
			if (temp != NULL) {
				mcsList.push(temp);
				PreOrderbreakST(temp);
			}
		}
		else {
			PreOrderbreakST(n->left);
			PreOrderbreakST(n->right);
		}
	}
}

queue<string> Execution::getControlsStruct() {
	TNode * r=mcsList.front();
	mcsList.pop();
	genCS(r);
	return this->mdeltaQ;
}

void Execution::genCS(TNode *n) {
	if (n != NULL) {
		if (n->data.compare("lambda")) {
			TNode *temp = n->left;
			//for now constuct <L.delval.x>
			++mnextdeltaVal;
			string s = "";
			s.append("<L.");
			s.append(to_string(mnextdeltaVal));
			s.append(".");
			s.append(temp->data);
			s.append(">");
			mdeltaQ.push(n->data);
			n = n->left;// and have to go to left
		}
		else {
			mdeltaQ.push(n->data);
		}
			genCS(n->left);
			genCS(n->right);
	}
}

