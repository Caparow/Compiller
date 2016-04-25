#pragma once

#include "SymbTable.h"

const vector<string> directives = { "END", "SEGMENT", "ENDS", "EQU", "DB", "DW", "DD", "IF", "ELSE", "ENDIF", "EQU" };
const vector<string> reg32 = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };
const vector<string> reg8 = { "AH", "AL", "CH", "CL", "DH", "DL", "BH", "BL" };
const vector<string> segReg = { "CS", "DS", "SS", "ES", "FS", "GS" };
const vector<string> commands = { "NOP", "INC", "DEC", "ADD", "CMP", "AND", "MOV", "OR", "JB" };
const vector<string> identTypes = { "BYTE", "WORD", "DWORD", "FWORD", "QWORD", "PWORD", "TBYTE", "DATAPTR", "CODEPTR" };
const vector<string> operators = { "PTR" };
const string alphabet = string(":()[]+-\'\";* 1234567890AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz_@?$,") + (char)0x9;
const string identChars = "ABCDEFGHIJKLMNOPQRSTUVXWYZ1234567890_@?$";
const string numChars = "1234567890";
const string singleCharLexems = ",:()[]+-*";
const string dividers = string(1, (char)0x9) + " ";
const string quoteChars = "\'\"";
const char   commentChar = ';';

bool directiveCheck(string s)
{
	for (int i = 0; i < directives.size(); ++i)
		if (s == directives[i])
			return true;
	return false;
}

bool reg32Check(string s)
{
	for (int i = 0; i < reg32.size(); ++i)
		if (s == reg32[i])
			return true;
	return false;
}


bool reg8Check(string s)
{
	for (int i = 0; i < reg8.size(); ++i)
		if (s == reg8[i])
			return true;
	return false;
}

bool segmentRegCheck(string s)
{
	for (int i = 0; i < segReg.size(); ++i)
		if (s == segReg[i])
			return true;
	return false;
}

bool commandCheck(string s)
{
	for (int i = 0; i < commands.size(); ++i)
		if (s == commands[i])
			return true;
	return false;
}

bool identCheck(string s)
{
	for (int i = 0; i < identTypes.size(); ++i)
		if (s == identTypes[i])
			return true;
	return false;
}

bool operatorCheck(string s)
{
	for (int i = 0; i < operators.size(); ++i)
		if (s == operators[i])
			return true;
	return false;
}

bool alphabetCheck(char ch)
{
	for (int i = 0; i < alphabet.size(); ++i)
		if (ch == alphabet[i])
			return true;
	return false;
}

bool identCharCheck(char ch)
{
	for (int i = 0; i < identChars.size(); ++i)
		if (ch == identChars[i])
			return true;
	return false;
}

bool numCheck(char ch)
{
	for (int i = 0; i < numChars.size(); ++i)
		if (ch == numChars[i])
			return true;
	return false;
}

bool singleCheck(char ch)
{
	for (int i = 0; i < singleCharLexems.size(); ++i)
		if (ch == singleCharLexems[i])
			return true;
	return false;
}

bool dividerCheck(char ch)
{
	for (int i = 0; i < dividers.size(); ++i)
		if (ch == dividers[i])
			return true;
	return false;
}

bool quoteCheck(char ch)
{
	for (int i = 0; i < quoteChars.size(); ++i)
		if (ch == quoteChars[i])
			return true;
	return false;
}

bool commentCheck(char ch)
{
	return ch == commentChar;
}