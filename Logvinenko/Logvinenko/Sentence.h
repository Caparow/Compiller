#pragma once

#include "Lexical_analizer.h"

struct Op
{
	vector<Token> OpV;
	int OPNum;
	int OPLen;
};

struct Sentence
{
	Token tokName;
	Token tokCom;
	vector<struct Op> Operands;
};


vector<Sentence> getSentenceTable(vector<Token>);
void OutputSentences(vector<Sentence> s, string);