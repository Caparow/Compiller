#pragma once

#include "LexicalAnalizer.h"

LexicalAnalizer::LexicalAnalizer(string fileName)
{
	ifstream inputCode(fileName);

	if (inputCode.is_open())
	{
		string tmp;
		while (getline(inputCode, tmp))
			code.push_back(tmp);

		inputCode.close();

		bool flag = false;
		for (int i = 0; i < code.size(); ++i)
		{
			int j = 0;

			while (j < code[i].length())
				if (!quoteCheck(code[i][j]))
				{
					code[i][j] = toupper(code[i][j]);
					++j;
				}
				else
				{
					++j;
					while (j < code[i].length() && !quoteCheck(code[i][j]))
						++j;
				}
		}

		GenerateLexemVector();
		AnalizeLexems();
		//OutputTokens(fileName);
	}
	else
	{
		cout << "Sorry, but file don't exist :C";
		exit(1);
	}
}


void LexicalAnalizer::GenerateLexemVector()
{
	Lexem tempL;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < code.size(); ++i)
	{
		int j = 0;
		tempL.row = i + 1;
		tempL.text.clear();

		while (j < code[i].length())
		{
			if (!alphabetCheck(code[i][j]))
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "ERROR: ";
				SetConsoleTextAttribute(hConsole, 7);
				cout << "Wrong character (" << i << " row): " << code[i] << endl;

				tempL.text += code[i][j];
				++j;
			}
			else if (dividerCheck(code[i][j]))
			{
				if (!tempL.text.empty())
					lexVec.push_back(tempL);

				tempL.text.clear();
				++j;
			}
			else if (singleCheck(code[i][j]))
			{
				if (!tempL.text.empty())
					lexVec.push_back(tempL);

				tempL.text.clear();
				tempL.text += code[i][j];
				lexVec.push_back(tempL);
				tempL.text.clear();
				++j;
			}
			else if (quoteCheck(code[i][j]))
			{
				tempL.text += code[i][j];
				++j;

				while (j < code[i].length() && !quoteCheck(code[i][j]))
				{
					tempL.text += code[i][j];
					++j;
				}

				if (j != code[i].length())
					tempL.text += code[i][j];
				else
				{
					SetConsoleTextAttribute(hConsole, 4);
					cout << "ERROR: ";
					SetConsoleTextAttribute(hConsole, 7);
					cout << "No close quote (" << i << " row): " << code[i] << endl;
				}

				lexVec.push_back(tempL);
				tempL.text.clear();
				++j;
			}
			else
			{
				tempL.text += code[i][j];
				++j;
			}
		}

		if (!tempL.text.empty())
			lexVec.push_back(tempL);
	}
}

void LexicalAnalizer::AnalizeLexems()
{
	Token tempT;	

	for (int i = 0; i < lexVec.size(); ++i)
	{
		tempT.lex = lexVec[i];
		tempT.lexemType = GetLexType(lexVec[i]);
		tokVec.push_back(tempT);
	}
}

void LexicalAnalizer::OutputTokens(string fileName)
{
	ofstream file;
	file.open(fileName + ".txt");
	string outLex;
	bool chRow = false;
	int outRow = 1;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n------------------------------------Row #" << setw(2) << outRow << "-------------------------------------";
	cout << setw(10) << "LEXEM" << setw(20) << "LENGTH" << setw(40) << "LEXEM TYPE" << endl;
	SetConsoleTextAttribute(hConsole, 7);
	file << "\n------------------------------------Row #" << setw(2) << outRow << "-------------------------------------" << endl;
	file << setw(10) << "LEXEM" << setw(20) << "LENGTH" << setw(40) << "LEXEM TYPE" << endl;

	for (int i = 0; i < tokVec.size(); ++i)
	{
		if (tokVec[i].lex.row != outRow)
		{
			chRow = true;
			outRow = tokVec[i].lex.row;
		}

		outLex = LexToStr(tokVec[i].lexemType);

		if (chRow == true)
		{
			chRow = false;
			SetConsoleTextAttribute(hConsole, 3);
			cout << "\n------------------------------------Row #" << setw(2) << outRow << "-------------------------------------";
			cout << setw(10) << "LEXEM" << setw(20) << "LENGTH" << setw(40) << "LEXEM TYPE" << endl;
			SetConsoleTextAttribute(hConsole, 7);
			file << "\n------------------------------------Row #" << setw(2) << outRow << "-------------------------------------" << endl;
			file << setw(10) << "LEXEM" << setw(20) << "LENGTH" << setw(40) << "LEXEM TYPE" << endl;

		}	

		cout << setw(10) << tokVec[i].lex.text << setw(20) << tokVec[i].lex.text.length() << setw(40) << outLex << endl;
		file << setw(10) << tokVec[i].lex.text << setw(20) << tokVec[i].lex.text.length() << setw(40) << outLex<< endl;
	}

	file.close();
}

string LexicalAnalizer::LexToStr(LexType type)
{
	switch (type)
	{
	case LexType::USER_IDENT:
		return "User ident.";
	case LexType::COMMAND:
		return "Machine comm.";
	case LexType::DIRECTIVE:
		return "Directive";
	case LexType::OPERATOR:
		return "Operator of type def.";
	case LexType::IDENT_TYPE:
		return "Type of ident.";
	case LexType::SINGLE_SYMB:
		return "Single lexem";
	case LexType::TEXT_CONST:
		return "Text const";
	case LexType::BIN_CONST:
		return "Bin const";
	case LexType::DEC_CONST:
		return "Dec const";
	case LexType::HEX_CONST:
		return "Hex const";
	case LexType::REG32:
		return "32b register";
	case LexType::REG8:
		return "8b register";
	case LexType::SEG_REG:
		return "Segment reg";
	case LexType::WRONG_LEX:
		return "Wrong lexem";
	}
}

LexType LexicalAnalizer::GetLexType(Lexem type)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (commandCheck(type.text))
		return LexType::COMMAND;
	else if (reg32Check(type.text))
		return LexType::REG32;
	else if (reg8Check(type.text))
		return LexType::REG8;
	else if (segmentRegCheck(type.text))
		return LexType::SEG_REG;
	else if (directiveCheck(type.text))
		return LexType::DIRECTIVE;
	else if (identCheck(type.text))
		return LexType::IDENT_TYPE;
	else if (operatorCheck(type.text))
		return LexType::OPERATOR;
	else if (quoteCheck(type.text[0]) && quoteCheck(type.text[type.text.length() - 1]))
		return LexType::TEXT_CONST;
	else if (quoteCheck(type.text[0]) && !quoteCheck(type.text[type.text.length() - 1]))
		return LexType::WRONG_LEX;
	else if (numCheck(type.text[0]))
	{
		if (type.text[type.text.length() - 1] == 'B')
			return LexType::BIN_CONST;
		else if (type.text[type.text.length() - 1] == 'H')
			return LexType::HEX_CONST;
		else if (type.text[type.text.length() - 1] == 'D' || numCheck(type.text[type.text.length() - 1]))
			return LexType::DEC_CONST;
		else if (!numCheck(type.text[type.text.length() - 1]))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Wrong number: ";
			SetConsoleTextAttribute(hConsole, 7);
			cout << "(" << type.row << " row): " << type.text << endl;

			return LexType::WRONG_LEX;
		}
	}
	else if (type.text.length() == 1)
	{
		if (singleCheck(type.text[0]))
			return LexType::SINGLE_SYMB;
		else
			return LexType::USER_IDENT;
	}
	else
	{
		if (type.text.length() > 8)
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Identeficator length more then 8 symbols:";
			SetConsoleTextAttribute(hConsole, 7);
			cout << "(" << type.row << " row): " << type.text << endl;

			return LexType::WRONG_LEX;
		}
		
		for (auto n_iter : type.text)
			if (!alphabetCheck(n_iter))
				return LexType::WRONG_LEX;

		return LexType::USER_IDENT;
	}	
}

vector<Token> LexicalAnalizer::getTokens()
{
	return tokVec;
}

vector<string> LexicalAnalizer::getCode()
{
	return code;
}

LexicalAnalizer::~LexicalAnalizer()
{
}