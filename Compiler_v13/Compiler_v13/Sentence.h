#pragma once

#include "LexicalAnalizer.h"

struct Op
{
	vector<Token> OpV;
	int OPNum;
	int OPLen;
};

struct Sentence
{
	Token nameTok;
	Token comTok;
	vector<struct Op> Operands;	
};


vector<Sentence> getSentenceTable(vector<Token>);
void OutputSentences(vector<Sentence> s, string);