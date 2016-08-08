#pragma once
#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stack>
#include "stdafx.h"
using namespace std;


struct TNode {

	string data; //
	TNode *left=NULL;
	TNode *right=NULL;
};


class BlrTree {

public:

	BlrTree();
	~BlrTree();

	void buildTree(string v, int num);

	void deleteTree(TNode *root);
	void preOrderTravPrint();
	void modifyTree();
	void sTreePrint();
	void setRoot();
	TNode *STRoot;
private:
	
	int count ;
	TNode *root;
	stack<TNode*> treeStack;
	void preOrderPrint(TNode *root);
	TNode* standardizeTree(TNode *root);
};


#endif // !BINARYTREE_H
