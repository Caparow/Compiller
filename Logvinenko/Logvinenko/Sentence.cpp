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
			if (tokVec[i].lexType == LexType::USER_IDENT)
			{
				tmpS.tokName = tokVec[i];
				++i;
				tmpS.tokName.LLen = 1;
				tmpS.tokName.LNum = i - tmpI;
				if (i < tokVec.size())
					if (tokVec[i].lex.text == ":")
						++i;
			}
			else if (tokVec[i].lexType == LexType::DIRECTIVE || tokVec[i].lexType == LexType::COMMAND)
			{
				tmpS.tokCom = tokVec[i];
				++i;
				tmpS.tokCom.LLen = 1;
				tmpS.tokCom.LNum = i - tmpI;
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
		if (!tmpS.tokCom.lex.text.empty() || !tmpS.tokName.lex.text.empty())
			tmpSV.push_back(tmpS);
		++curRow;
		tmpI = i;
	}
	return tmpSV;
}

void OutputSentences(vector<Sentence> s, string filename)
{
	ofstream ofile;
	ofile.open(filename + "_Sentences.txt");
	setlocale(LC_CTYPE, "rus");

	cout << endl << endl;
	cout << "--------------------------------------------------------------------------------";
	cout << "|ÑÒÐÎÊÀ|" << setw(12) << "ÌÅÒÊÀ|" << setw(19) << "ÊÎÌÀÍÄA|" << setw(20) << "ÎÏÅÐÀÍÄ¹1|" << setw(20) << "ÎÏÅÐÀÍÄ¹2|" << endl;

	ofile << "--------------------------------------------------------------------------------" << endl;;
	ofile << "|ÑÒÐÎÊÀ|" << setw(12) << "ÌÅÒÊÀ|" << setw(19) << "ÊÎÌÀÍÄA|" << setw(20) << "ÎÏÅÐÀÍÄ¹1|" << setw(20) << "ÎÏÅÐÀÍÄ¹2|" << endl;


	for (int i = 0; i < s.size(); ++i)
	{
		cout << "--------------------------------------------------------------------------------";
		ofile << "--------------------------------------------------------------------------------" << endl;;
		if (s[i].tokName.lex.text.empty())
		{
			cout << "|" << setw (3) << s[i].tokCom.lex.row << setw(4) << "|";
			ofile << "|" << setw(3) << s[i].tokCom.lex.row << setw(4) << "|";
		}
		else
		{
			cout << "|" << setw(3) << s[i].tokName.lex.row << setw(4) << "|";
			ofile << "|" << setw(3) << s[i].tokName.lex.row << setw(4) << "|";
		}
		if (!s[i].tokName.lex.text.empty())
		{
			cout << setw(5) << s[i].tokName.LNum << "," << s[i].tokName.LLen << setw(5) << "|";
			ofile << setw(5) << s[i].tokName.LNum << "," << s[i].tokName.LLen << setw(5) << "|";
		}
		else
		{
			cout << setw(12) << "|";
			ofile << setw(12) << "|";
		}
		if (!s[i].tokCom.lex.text.empty())
		{
			cout << setw(8) << s[i].tokCom.LNum << "," << s[i].tokCom.LLen << setw(9) << "|";
			ofile << setw(8) << s[i].tokCom.LNum << "," << s[i].tokCom.LLen << setw(9) << "|";
		}
		else
		{
			cout << setw(19) << "|";
			ofile << setw(19) << "|";
		}
		if (!s[i].Operands.empty())
		{
			for (int j = 0; j < s[i].Operands.size(); ++j)
			{
				cout << setw(9) << s[i].Operands[j].OPNum << "," << s[i].Operands[j].OPLen << setw(9) << "|";
				ofile << setw(9) << s[i].Operands[j].OPNum << "," << s[i].Operands[j].OPLen << setw(9) << "|";
			}
			if (s[i].Operands.size() == 1)
			{
				cout << setw(20) << "|";
				ofile << setw(20) << "|" << endl;;
			}
			else
				ofile << endl;
		}
		else
		{
			cout << setw(20) << "|";
			cout << setw(20) << "|";
			ofile << setw(20) << "|";
			ofile << setw(20) << "|" << endl;;
		}

		cout << endl;

	}
	cout << "--------------------------------------------------------------------------------";
	ofile << "--------------------------------------------------------------------------------" << endl;;
	ofile.close();
}