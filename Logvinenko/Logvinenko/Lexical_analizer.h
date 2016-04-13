#pragma once

#include <clocale>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>


using namespace std;

enum class LexType {
	USER_IDENT, COMMAND, SINGLE_SYMB, TEXT_CONST, BIN_CONST, \
	DEC_CONST, HEX_CONST, DIRECTIVE, REG32, REG16, REG8, SEG_REG, WRONG_LEX
};


struct Lexem
{
	string text;
	int row;
	int column;
};

struct Token
{
	Lexem lex;
	LexType lexType;
};

const string directivesA[] = { "END", "SEGMENT", "ENDS", "EQU", "DB", "DW", "DD", "IF", "ENDIF", "EQU" };
const string reg32A[] = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };
const string reg8A[] = { "AH", "AL", "CH", "CL", "DH", "DL", "BH", "BL" };
const string segRegA[] = { "CS", "DS", "SS", "ES", "FS", "GS" };
const string commandsA[] = { "INC", "DEC", "ADD", "CMP", "MOV", "OR", "JZ", "STOSD", "AND" };
const string alphabet = string("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ_@?$,:()[]+-\'\";* ") + (char)0x9;
const string identChars = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ_@?$";
const string numChars = "1234567890";
const string singleCharLexems = ",:()[]+-*";
const string dividers = string(1, (char)0x9) + " ";
const string quoteChars = "\'\"";
const char   commentChar = ';';

const set<string> directives(directivesA, directivesA + 11);
const set<string> reg32(reg32A, reg32A + 8);
const set<string> reg8(reg8A, reg8A + 8);
const set<string> segReg(segRegA, segRegA + 6);
const set<string> commands(commandsA, commandsA + 9);

void GenerateLexemVector(vector<string> code, vector<Lexem> &lexems);
void AnalizeLexems(vector<Lexem> lexems, vector<Token> &tokens);
void OutputTokens(vector<Token> tokens, string);

bool isDirective(string s);
bool isDataType(string s);
bool is32Reg(string s);
bool is8Reg(string s);
bool isSegReg(string s);
bool isCommand(string s);
bool isInAlph(char ch);
bool isCharIden(char ch);
bool isCharNum(char ch);
bool isSingleCharLexem(char ch);
bool isDivider(char ch);
bool isQuote(char ch);
bool isComment(char ch);

