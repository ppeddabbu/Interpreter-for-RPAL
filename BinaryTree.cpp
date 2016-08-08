
#include "stdafx.h"
#include <iostream>
#include <stack>
#include "BinaryTree.h"
#include <string>

using namespace std;

/*
*End of the execution of parser, stack should contain only one element, if it contains more then there is an error
*/

BlrTree :: BlrTree(){
	root = NULL;
	count = 0;
}

BlrTree :: ~BlrTree() {
	//TODO remove any allocated buffers
	deleteTree(root);
}

void BlrTree :: deleteTree(TNode *n) {
	if (n != NULL) {
		//delete in post order
		deleteTree(n->left);
		deleteTree(n->right);
		delete n;
	}
}

void BlrTree::buildTree(string val, int pc) {
	if (pc == 0) {
		TNode *temp = new TNode;
		temp->data = string(val);
		//push to stack
		treeStack.push(temp);
	}else{
		
		try {// create current node
			size_t s = treeStack.size();
			if (pc > s)
				throw s;

			stack<TNode*> tempStack;
			for (int i = 0; i < pc; i++) {
				tempStack.push(treeStack.top());
				treeStack.pop();
			}

			//pop & combine fist left ann other right while count = pc
			TNode *root = new TNode, *curr;
			root->data = string(val); 

			curr = root;
			curr->left = tempStack.top();
			tempStack.pop();
			curr = curr->left;
			
			while (!tempStack.empty()) {
				curr->right = tempStack.top();
				tempStack.pop();
				curr = curr->right;
			}
			// and push the result
			treeStack.push(root);
		}
		catch(int e){
			cout << "Error: stack contains only " << e << " tree(s) " << endl;
			exit(1);
		}
	}
	//cout << "----------------------" << endl;
	//preOrderTravPrint();

}

void BlrTree:: preOrderPrint(TNode *n){

	if (n != NULL) {
		for (int i = 0; i < count; i++)
		{
			cout << ".";
		}
		cout << n->data << endl;
		count++;
		preOrderPrint(n->left);
		count--;
		preOrderPrint(n->right);
	}
}

//void BlrTree::printTree(TNode *n) {
//
//	if (n != NULL) {
//		for (int i = 0; i < count; i++)
//		{
//			cout << ".";
//		}
//		cout << n->data << endl;
//		count++;
//		preOrderPrint(n->left);
//		preOrderPrint(n->right);
//	}
//}

void BlrTree::sTreePrint() {
	count = 0;
	preOrderPrint(root);
}

void BlrTree::preOrderTravPrint() {

	if(treeStack.size())
	preOrderPrint(treeStack.top());
	root = treeStack.top(); //it's important to have here
}

TNode* BlrTree::standardizeTree(TNode *r) {
	//bool isFirstChild = true;
	if (r == NULL)
		return r;
	TNode *child = r->left, *tempr=r;

	if (child != NULL) { // Is there any holes?? check again
		tempr->left= standardizeTree(child);
		tempr = tempr->left;				//important...here child will hold sib link
		child = child->right;
		while (child != NULL) {
			tempr->right = standardizeTree(child);
			tempr = tempr->right;
			child = child->right;
		}
	}

	if (r->data.compare("let")==0) {
		TNode *gamma = new TNode;
		gamma->data = "gamma";
		TNode *lmda = new TNode;
		lmda->data = "lambda";
		TNode *E = r->left->left->right;
		lmda->left = r->left->left; //X
		lmda->left->right = r->left->right; //P
		gamma->left = lmda;
		gamma->left->right =E ; //E
		return gamma;
	}
	else if (r->data.compare("where") == 0) {
		TNode *gamma = new TNode;
		gamma->data = "gamma";
		TNode *lmda = new TNode;
		lmda->data = "lambda";
		TNode *E = r->left->right->left->right;
		lmda->left = r->left->right->left; //X
		TNode *P = r->left;
		P->right = NULL;
		lmda->left->right = P;  //P
		gamma->left = lmda;
		gamma->left->right = E; //E
		return gamma;
	}
	else if (r->data.compare("function_form") == 0) {
		//P V+ E
		TNode *transformN = r, *stp, *astp;
		stp = transformN;
		stp->data = "=";
		//P is already intact
		stp = stp->left;
		astp = stp;
		astp = astp->right;

		do {
			TNode *lmda = new TNode;
			lmda->data = "lambda";
			stp->right = lmda;
			lmda->left = astp;
			stp = astp;
			astp=astp->right;
		} while (astp->right != NULL);
		stp->right = astp;
		return transformN;
	}
	else if (r->data.compare("@") == 0) {
		TNode *gamma1 = new TNode;
		gamma1->data = "gamma";
		TNode *gamma2 = new TNode;
		gamma2->data = "gamma";
		TNode *E2= r->left->right->right; //E2
		TNode *N = r->left->right; //N
		N->right = NULL;//break link
		gamma2->left = N;
		TNode *E1 = r->left;//E1
		E1->right = NULL;
		gamma2->left->right = E1; 
		gamma1->left = gamma2;
		gamma1->left->right = E2;
		return gamma1;
	}
	else if (r->data.compare("within") == 0) { 
		TNode *gamma = new TNode;
		gamma->data = "gamma";
		TNode *lmda = new TNode;
		lmda->data = "lambda";

		TNode *sEq = r->left->right; // = and X2 will be covered
		TNode *E2 = sEq->left->right;
		lmda->left= r->left->left; //X1
		lmda->left->right = E2; //E2
		lmda->right = r->left->left->right;//E1
		sEq->left->right = gamma;
		gamma->left = lmda;
		return sEq;
		//fEq is a memory leak, need to free it
	}
	else if (r->data.compare("lambda") == 0) {
		TNode *V = r->left;
		if (V->right->right == NULL) {
			return r;
		}
		else {
			stack<TNode*> s;
			do {
				s.push(V);
				TNode* E = V->right;
				E->right = NULL;
				V=E;
			} while (V != NULL);

			while (s.size() > 1) {
				TNode *l = s.top();
				s.pop();
				TNode *r = s.top();
				s.pop();
				TNode *lmda = new TNode;
				lmda->data = "lambda";
				lmda->left = l;
				lmda->right = r;
				s.push(lmda);
			}
			r = s.top();
			s.pop();
		}
		return r;
	}
	else if (r->data.compare("and") == 0) { 
		TNode *tau = new TNode;
		tau->data = "tau";
		TNode *comma = new TNode;
		comma->data = ",";
		TNode *eq = new TNode;
		eq->data = "=";
		eq->left = comma;
		eq->right = tau;

		TNode *eqN = r->left;
		TNode *X=NULL, *E=NULL; 
		do {
			TNode *x=eqN->left;
			TNode *e = eqN->left->right;
			x->right = NULL;
			if (X == NULL) {
				X = x;
				E = e;
				comma->left = X;
				tau->left = E;
			}
			else {
				X->right = x;
				E->right = e;
				X = x;
				E = e;
			}
			eqN = eqN->right;
		} while (eqN!=NULL);
		return eq;
	}
	else if (r->data.compare("rec") == 0) {
		TNode *gamma = new TNode;
		gamma->data = "gamma";
		TNode *lmda = new TNode;
		lmda->data = "lambda";
		TNode *ystar = new TNode;
		ystar->data = "Ystar";
		TNode *X2 = new TNode;

		TNode *X1 = r->left->left; //X1
		TNode *E = r->left->left->right; //E
		X1->right = NULL;
		X2->data = X1->data;
		TNode *eq = r->left; // = and X
		eq->left->right = gamma; 
		gamma->left = ystar;
		gamma->left->right = lmda;
		lmda->left = X2;
		lmda->left->right = E; //E

		return eq;
	}

	return r;
}

void BlrTree:: modifyTree() {
	root = standardizeTree(root);
}

void BlrTree::setRoot() {
	 this->STRoot=root;
}