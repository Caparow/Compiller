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

static vector<segment> segTable;
static vector<ident> identTable;
vector<string> getIntBytes(vector<string>, vector<Sentence>);
int getOpByte(vector<Token>, vector<ident>);
string intToHex(int);
void outputIntBytes(vector<string>, vector<Token>);
