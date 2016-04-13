#pragma once

#include <set>
#include <vector>
#include <map>
#include <string>
#include <fstream>

using namespace std;

enum class LexType {
	USER_IDENT, COMMAND, SINGLE_SYMB, TEXT_CONST, BIN_CONST, \
	DEC_CONST, HEX_CONST, DIRECTIVE, REG32, REG8, SEG_REG, WRONG_LEX, \
	OPERATOR, IDENT_TYPE
};


struct Lexem
{
	string text;
	int row;
};

struct Token
{
	Lexem lex;
	LexType lexemType;
	int LNum, LLen;
};