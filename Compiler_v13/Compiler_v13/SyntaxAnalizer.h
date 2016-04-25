#pragma once

#include "Sentence.h"

struct ident
{
	string name;
	string attr;
	string type;
	string value;
};

struct segment
{
	string name;
	string length;
};

struct list
{
	string byte;
	string code;
	string com;
};

struct equ
{
	string name;
	bool flag;
};

static vector<equ> equV;
static vector<list> listing;
static vector<segment> segTable;
static vector<ident> identTable;


vector<list> getIntBytes(vector<string>, vector<Sentence>);
vector<list> getComCodes(vector<list>, vector<Sentence>);
int getOpByte(vector<Token>, vector<ident>);
OpType getOpType(vector<Token>);
string stringToHex(const string);
string getSCom(string);
string intToHex(int);
string setReversedByteSequence(const string input, short size);
void outputIntBytes(vector<list>, vector<Token>, string);
