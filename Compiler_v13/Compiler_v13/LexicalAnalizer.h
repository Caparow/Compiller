#pragma once
#include "SymbTable.h"
#include "LexemTable.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <Windows.h>

class LexicalAnalizer
{
private:
	vector<Lexem> lexVec;
	vector<Token> tokVec;
	vector<string> code;

	void GenerateLexemVector();
	void AnalizeLexems();
	void OutputTokens(string fileName);
	string LexToStr(LexType type);
	LexType GetLexType(Lexem type);
public:
	LexicalAnalizer() = delete;
	LexicalAnalizer(string fileName);
	vector<Token> getTokens();
	vector<string> getCode();
	~LexicalAnalizer();
};