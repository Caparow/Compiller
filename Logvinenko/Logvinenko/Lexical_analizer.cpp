#pragma once
#include "Lexical_analizer.h"

void GenerateLexemVector(vector<string> code, vector<Lexem> &lexems)
{
	Lexem bufLex;
	
	for (int i = 0; i < code.size(); ++i)
	{
		int j = 0;
		bufLex.row = i + 1;
		bufLex.column = 0;
		bufLex.text.clear();

		while (j < code[i].length())
		{

			if (!isInAlph(code[i][j]))
			{
				setlocale(LC_CTYPE, "rus");
				cout << "ÏÎÌÈËÊÀ: ";
				cout << "ÍÅÂ²ÄÎÌÈÉ ÑÈÌÂÎË:" << code[i] << endl; 

				bufLex.text += code[i][j];
				bufLex.column = ++j;
			}
			else if (isDivider(code[i][j]))
			{
				if (!bufLex.text.empty())
					lexems.push_back(bufLex);

				bufLex.column = j + 1;
				bufLex.text.clear();
				++j;
			}
			else if (isSingleCharLexem(code[i][j]))
			{
				if (!bufLex.text.empty())
				{
					lexems.push_back(bufLex);
					bufLex.column = j + 1;
					bufLex.text.clear();
				}

				bufLex.text += code[i][j];
				lexems.push_back(bufLex);
				bufLex.column = j + 1;
				bufLex.text.clear();
				++j;
			}
			else if (isQuote(code[i][j]))
			{
				bufLex.text += code[i][j];
				++j;
				while ((j < code[i].length()) && (!isQuote(code[i][j])))
				{
					bufLex.text += code[i][j];
					++j;
				}

				bufLex.text += code[i][j];
				lexems.push_back(bufLex);
				bufLex.column = j + 1;
				bufLex.text.clear();
				++j;
			}
			else
			{
				bufLex.text += code[i][j];
				++j;
			}
		}

		if (!bufLex.text.empty())
			lexems.push_back(bufLex);

		bufLex.column = j + 1;
	}
}

void AnalizeLexems(vector<Lexem> lexems, vector<Token> &tokens)
{
	Token bufToken;

	for (int i = 0; i < lexems.size(); ++i)
	{
		bufToken.lex.text = lexems[i].text;
		bufToken.lex.row = lexems[i].row;
		bufToken.lex.column = lexems[i].column;

		if (isCommand(lexems[i].text))
			bufToken.lexType = LexType::COMMAND;
		else if (is32Reg(lexems[i].text))
			bufToken.lexType = LexType::REG32;
		else if (is8Reg(lexems[i].text))
			bufToken.lexType = LexType::REG8;
		else if (isSegReg(lexems[i].text))
			bufToken.lexType = LexType::SEG_REG;
		else if (isDirective(lexems[i].text))
			bufToken.lexType = LexType::DIRECTIVE;
		else if (isQuote(lexems[i].text[0]) && isQuote(lexems[i].text[lexems[i].text.length() - 1]))
			bufToken.lexType = LexType::TEXT_CONST;
		else if (isQuote(lexems[i].text[0]) && !isQuote(lexems[i].text[lexems[i].text.length() - 1])) 
			bufToken.lexType = LexType::WRONG_LEX;
		else if (isCharNum(lexems[i].text[0]))
		{
			if (lexems[i].text[lexems[i].text.length() - 1] == 'B')
				bufToken.lexType = LexType::BIN_CONST;
			else if (lexems[i].text[lexems[i].text.length() - 1] == 'H')
				bufToken.lexType = LexType::HEX_CONST;
			else if (lexems[i].text[lexems[i].text.length() - 1] == 'D' || isCharNum(lexems[i].text[lexems[i].text.length() - 1]))
				bufToken.lexType = LexType::DEC_CONST;
			else if (!isCharNum(lexems[i].text[lexems[i].text.length() - 1]))
				bufToken.lexType = LexType::WRONG_LEX;
		}
		else if (lexems[i].text.length() == 1)
		{
			if (isSingleCharLexem(lexems[i].text[0]))
				bufToken.lexType = LexType::SINGLE_SYMB;
			else
				bufToken.lexType = LexType::USER_IDENT;
		}
		else
		{
			bufToken.lexType = LexType::USER_IDENT;
			for (auto n_iter : lexems[i].text)
				if (!isInAlph(n_iter))
					bufToken.lexType = LexType::WRONG_LEX;
		}

		tokens.push_back(bufToken);
	}
}

void OutputTokens(vector<Token> tokens, string outPutFile)
{
	ofstream ofile;
	ofile.open(outPutFile+".txt");

	setlocale(LC_CTYPE, "rus");
	int i = 0;
	string outLex;

	ofile << setw(4) << "|ÑÒÐÎÊÀ|" << setw(8) << "ÊÎËÎÍÊÀ|" << setw(22) << "ËÅÊÑÅÌA|" << setw(40) << "ÒÈÏ ËÅÊÑÅÌÛ|" << endl;
	ofile << "--------------------------------------------------------------------------------" << endl;
	cout << setw(4) << "|ÑÒÐÎÊÀ|" << setw(8) << "ÊÎËÎÍÊÀ|" << setw(22) << "ËÅÊÑÅÌA|" << setw(40) << "ÒÈÏ ËÅÊÑÅÌÛ|" << endl;
	cout << "--------------------------------------------------------------------------------";

	for (int i = 0; i < tokens.size(); ++i)
	{
		switch (tokens[i].lexType)
		{
		case LexType::USER_IDENT:
			outLex = "ÈÄÅÍÒÈÔÈÊÀÒÎÐ";
			break;
		case LexType::COMMAND:
			outLex = "ÊÎÌÀÍÄÀ";
			break;
		case LexType::DIRECTIVE:
			outLex = "ÄÈÐÅÊÒÈÂÀ";
			break;
		case LexType::SINGLE_SYMB:
			outLex = "ÎÄÍÎÑÈÌÂÎËÜÍÀß";
			break;
		case LexType::TEXT_CONST:
			outLex = "ÒÅÊÑÒÎÂÀß Ê.";
			break;
		case LexType::BIN_CONST:
			outLex = "ÄÂÎÈ×ÍÀß Ê.";
			break;
		case LexType::DEC_CONST:
			outLex = "ÄÅÑßÒÈ×ÍÀß Ê.";
			break;
		case LexType::HEX_CONST:
			outLex = "ØÅÑÒÍÀÄÖÀÒÈÐÈÈ×ÍÀß Ê.";
			break;
		case LexType::REG32:
			outLex = "32 Ð-É ÐÅÃ.";
			break;
		case LexType::REG8:
			outLex = "8 Ð-É ÐÅÃ.";
			break;
		case LexType::SEG_REG:
			outLex = "ÑÅÃÌÅÍÒÍÛÉ ÐÅÃ.";
			break;
		case LexType::WRONG_LEX:
			outLex = "ÍÅÂÅÐÍÀß ËÅÊÑÅÌÀ";
			break;
		}

		cout << "|" << setw(4) << tokens[i].lex.row << setw(3) << "|" << setw(4) << tokens[i].lex.column << setw(4) << "|" << setw(21) << tokens[i].lex.text << "|" << setw(39) << outLex << "|" << endl;
		ofile << "|" << setw(4) << tokens[i].lex.row << setw(3) << "|" << setw(4) << tokens[i].lex.column << setw(4) << "|" << setw(21) << tokens[i].lex.text << "|" << setw(39) << outLex << "|" << endl;
		if (i+1 != tokens.size())
			if (tokens[i].lex.row != tokens[i + 1].lex.row)
			{
				cout << "--------------------------------------------------------------------------------";
				ofile << "--------------------------------------------------------------------------------" << endl;
			}
	}	

	ofile.close();
}

bool isDirective(string s)
{
	return directives.find(s) != directives.end();
}

bool is32Reg(string s)
{
	return reg32.find(s) != reg32.end();
}

bool is8Reg(string s)
{
	return reg8.find(s) != reg8.end();
}

bool isSegReg(string s)
{
	return segReg.find(s) != segReg.end();
}

bool isCommand(string s)
{
	return commands.find(s) != commands.end();
}

bool isInAlph(char ch)
{
	return alphabet.find(ch) != string::npos;
}

bool isCharIden(char ch)
{
	return identChars.find(ch) != string::npos;
}

bool isCharNum(char ch)
{
	return numChars.find(ch) != string::npos;
}

bool isSingleCharLexem(char ch)
{
	return singleCharLexems.find(ch) != string::npos;
}

bool isDivider(char ch)
{
	return dividers.find(ch) != string::npos;
}

bool isQuote(char ch)
{
	return quoteChars.find(ch) != string::npos;
}

bool isComment(char ch)
{
	return ch == commentChar;
}
