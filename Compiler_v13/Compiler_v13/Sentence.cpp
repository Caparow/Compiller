#pragma once

#include "Sentence.h"

vector<Sentence> getSentenceTable(vector<Token> tokVec)
{
	vector<Sentence> tmpSV;
	

	int i = 0;
	int curRow = 1;
	int tmpI = 0;
	while (i < tokVec.size())
	{
		Sentence tmpS;
		while (i < tokVec.size() && tokVec[i].lex.row == curRow)
		{
			if (tokVec[i].lexemType == LexType::USER_IDENT)
			{
				tmpS.nameTok = tokVec[i];
				++i;
				tmpS.nameTok.LLen = 1;
				tmpS.nameTok.LNum = i - tmpI; 
				if (i < tokVec.size())
					if (tokVec[i].lex.text == ":")
						++i;
			}
			else if (tokVec[i].lexemType == LexType::DIRECTIVE || tokVec[i].lexemType == LexType::COMMAND)
			{
				tmpS.comTok = tokVec[i];
				++i;
				tmpS.comTok.LLen = 1;
				tmpS.comTok.LNum = i - tmpI;
				if (i < tokVec.size())
				{
					Op tmpO;
					while (i <= tokVec.size() && tokVec[i].lex.row == curRow)
					{
						if (tokVec[i].lex.text == ",")
						{
							tmpO.OPLen = tmpO.OpV.size();
							tmpO.OPNum = i - tmpI - tmpO.OpV.size() + 1;
							tmpS.Operands.push_back(tmpO);
							tmpO.OpV.clear();
							++i;							
							if (i < tokVec.size())
								if (tokVec[i].lex.row != curRow)
									break;
						}
						tmpO.OpV.push_back(tokVec[i]);
						++i;
					}
					if (!tmpO.OpV.empty())
					{
						tmpO.OPLen = tmpO.OpV.size();
						tmpO.OPNum = i - tmpI - tmpO.OpV.size() + 1;
						tmpS.Operands.push_back(tmpO);
					}
				}				
			}
		}
		if (!tmpS.comTok.lex.text.empty() || !tmpS.nameTok.lex.text.empty())
			tmpSV.push_back(tmpS);
		++curRow;
		tmpI = i;
	}
	return tmpSV;
}

void OutputSentences(vector<Sentence> s, string filename)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ofstream file;
	file.open(filename + "_Sentences.txt");
	cout << endl << endl;

	for (int i = 0; i < s.size(); ++i)
	{
		SetConsoleTextAttribute(hConsole, 3);
		cout << "ROW:";
		file << "ROW:";
		SetConsoleTextAttribute(hConsole, 7);
		if (s[i].nameTok.lex.text.empty())
		{
			cout << setw(3) << s[i].comTok.lex.row;
			file << setw(3) << s[i].comTok.lex.row;
		}
		else
		{
			cout << setw(3) << s[i].nameTok.lex.row;
			file << setw(3) << s[i].nameTok.lex.row;
		}
		if (!s[i].nameTok.lex.text.empty())
		{
			SetConsoleTextAttribute(hConsole, 3);
			cout << setw(8) << "LABEL:";
			SetConsoleTextAttribute(hConsole, 7);
			cout << setw(9) << s[i].nameTok.lex.text << "(" << s[i].nameTok.LNum << "," << s[i].nameTok.LLen << ")  ";
			file << setw(8) << "LABEL:";
			file << setw(9) << s[i].nameTok.lex.text << "(" << s[i].nameTok.LNum << "," << s[i].nameTok.LLen << ")  ";;
		}		
		if (!s[i].comTok.lex.text.empty())
		{
			SetConsoleTextAttribute(hConsole, 3);
			cout << setw(10) << " COMM/DIR: ";
			SetConsoleTextAttribute(hConsole, 7);
			cout << s[i].comTok.lex.text << "(" << s[i].comTok.LNum << "," << s[i].comTok.LLen << ")  ";
			file << setw(10) << " COMM/DIR: ";
			file << s[i].comTok.lex.text << "(" << s[i].comTok.LNum << "," << s[i].comTok.LLen << ")  ";
		}
		if (!s[i].Operands.empty())
		{
			for (int j = 0; j < s[i].Operands.size(); ++j)
			{
				SetConsoleTextAttribute(hConsole, 3);
				cout << "OP #" << j + 1 << ": ";
				SetConsoleTextAttribute(hConsole, 7);
				file << "OP #" << j + 1 << ": ";
				
				for (int p = 0; p < s[i].Operands[j].OpV.size(); ++p)
				{
					cout << s[i].Operands[j].OpV[p].lex.text << " ";
					file << s[i].Operands[j].OpV[p].lex.text << " ";
				}
				cout << "(" << s[i].Operands[j].OPNum << "," << s[i].Operands[j].OPLen << ")  ";
				file << "(" << s[i].Operands[j].OPNum << "," << s[i].Operands[j].OPLen << ")  ";
			}
		}
		cout << endl;
		file << endl;
	}
}