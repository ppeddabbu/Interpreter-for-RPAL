#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H


int LIT_MAX = 64;
int STR_MAX = 256;
int OPT_MAX = 4;

char operators[] = {
'+','-','*','<','>','&','.',
'@','/',':','=','˜','|','$',
'!','#','%','ˆ','_','[',']',
'{','}','"','‘','?',';'
};

string keywords[]= {
"let","in","fn","where","aug","->","or",
"not","gr","ge",">=","ls","le","<=","eq","ne",
"**","neg","true","false","nil","dummy", "within",
"and","rec"
};
// CHANGE it's corresponding for loop hardcoded value '25' as of now
//"tau","lambda", "gamma", "fcn_form"

string stOperators[] = {
"+","-","*",".","/",
"or","not","gr","ge",
"ls","le","eq","ne",
"=","(",")","&","|"
};

#endif // !CONSTANTS_H


