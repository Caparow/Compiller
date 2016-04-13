#pragma once

#include "SyntaxAnalizer.h"

vector<string> getIntBytes(vector<string> code, vector<Sentence> s)
{
	ident tmpIdent;
	vector<string> tmp;
	string tmpByteS;
	int tmpByte = 0;
	string segName;
	bool flag;
	bool endsF = false;
	int codeInd = 0;
	string tmpVal = "";
	segment tmpSeg;

	struct equ
	{
		string name;
		bool flag;
	};

	vector<equ> equV;


	int i = 0;
	while (i < s.size())
	{
		if (!tmpIdent.name.empty())
		{
			identTable.push_back(tmpIdent);
			tmpIdent.name.clear();
		}

		if (!endsF)
		{
			if (!tmpByteS.empty())
			{
				tmpVal = tmpByteS;
				if (code[codeInd] != "")
					tmpByteS += " " + code[codeInd];
				else
				{
					while (code[codeInd] == "")
						++codeInd;
					tmpByteS += " " + code[codeInd];
				}
				++codeInd;
				tmp.push_back(tmpByteS);
				tmpByteS.clear();
			}
			else
			{
				int tmpLenOp = intToHex(tmpByte).length();

				for (int j = tmpLenOp; j < 4; ++j)
					tmpByteS += "0";

				tmpByteS += intToHex(tmpByte);
				tmpVal = tmpByteS;
				if (code[codeInd] != "")
					tmpByteS += " " + code[codeInd];
				else
				{
					while (code[codeInd] == "")
						++codeInd;
					tmpByteS += " " + code[codeInd];
				}
				++codeInd;
				tmp.push_back(tmpByteS);
				tmpByteS.clear();
			}
		}
		

		if (s[i].comTok.lex.text == "SEGMENT")
		{
			tmpByte = 0;
			segName = s[i].nameTok.lex.text;
			endsF = false;
		}
		else if (s[i].comTok.lex.text == "DB")
		{
			tmpIdent.value = tmpVal;

			if (s[i].Operands[0].OpV[0].lexemType == LexType::TEXT_CONST)
				tmpByte += s[i].Operands[0].OpV[0].lex.text.length() -2;
			else
				tmpByte += 1;
			tmpIdent.attr = segName;
			tmpIdent.name = s[i].nameTok.lex.text;
			tmpIdent.type = "L BYTE";
		}
		else if (s[i].comTok.lex.text == "DD")
		{
			tmpIdent.value = tmpVal;
			tmpByte += 4;
			tmpIdent.attr = segName;
			tmpIdent.name = s[i].nameTok.lex.text;
			tmpIdent.type = "L WORD";
		}
		else if (s[i].comTok.lex.text == "DW")
		{
			tmpIdent.value = tmpVal;
			tmpByte += 2;
			tmpIdent.attr = segName;
			tmpIdent.name = s[i].nameTok.lex.text;
			tmpIdent.type = "L DWORD";
		}
		else if (s[i].comTok.lex.text == "ENDS")
		{
			tmpSeg.length = tmpVal;
			tmpSeg.name = segName;
			segTable.push_back(tmpSeg);
			segName.clear();
			tmpByte = 0;
			endsF = true;
		}
		else if (s[i].comTok.lex.text == "EQU")
		{
			tmpIdent.value = "";
			tmpIdent.attr = "";
			tmpIdent.name = s[i].nameTok.lex.text;
			tmpIdent.type = "NUMBER";

			int tmpLenOp = s[i].Operands[0].OpV[0].lex.text.length();
			tmpByteS = "= ";

			for (int j = tmpLenOp; j < 4; ++j)
			{
				tmpByteS += "0";
				tmpIdent.value += "0";
			}				

			tmpByteS += s[i].Operands[0].OpV[0].lex.text;
			tmpIdent.value += s[i].Operands[0].OpV[0].lex.text;
			

			if (code[codeInd] != "")
				tmpByteS += " " + code[codeInd];
			else
			{
				while (code[codeInd] == "")
					++codeInd;
				tmpByteS += " " + code[codeInd];
			}
			++codeInd;
			tmp.push_back(tmpByteS);
			tmpByteS.clear();

			equ tmpEqu;
			tmpEqu.name = s[i].nameTok.lex.text;
			if (s[i].Operands[0].OpV[0].lex.text == "1")
				tmpEqu.flag = 1;
			else
				tmpEqu.flag = 0;

			equV.push_back(tmpEqu);

			if (s[i+1].comTok.lex.text == "SEGMENT")
				endsF = false;

		}
		else if (s[i].comTok.lex.text == "NOP")
		{
			tmpByte += 1;
		}
		else if (s[i].comTok.lex.text == "INC")
		{
			tmpByte += 1+getOpByte(s[i].Operands[0].OpV, identTable);
		}
		else if (s[i].comTok.lex.text == "DEC")
		{
			tmpByte += 1+getOpByte(s[i].Operands[0].OpV, identTable);
		}
		else if (s[i].comTok.lex.text == "MOV")
		{	
			if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
				tmpByte += 1;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
				tmpByte += 4;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
				tmpByte += 1;
			else 
				tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "AND")
		{

			if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
				tmpByte += 1;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
				tmpByte += 4;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
				tmpByte += 1;
			else
				tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "ADD")
		{
			if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
				tmpByte += 1;
			else if(s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
				tmpByte += 4;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
				tmpByte += 1;
			else
				tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "CMP")
		{
			if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
				tmpByte += 1;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
				tmpByte += 1;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
				tmpByte += 1;
			else
				tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);

			tmpByte += getOpByte(s[i].Operands[1].OpV, identTable);

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "OR")
		{
			if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
				tmpByte += 1;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
				tmpByte += 4;
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
				tmpByte += 1;
			else
				tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "IF")
		{
			tmp.pop_back();

			for (int j = 0; j < equV.size(); ++j)
			{
				if (s[i].Operands[0].OpV[0].lex.text == equV[j].name)
					flag = equV[j].flag;
			}


			if (!flag)
			{
				while (s[i+1].comTok.lex.text != "ELSE")
				{
					++codeInd;
					++i;
				}
			}
			else
				tmp.push_back(code[codeInd - 1]);
		}
		else if (s[i].comTok.lex.text == "ELSE")
		{
			tmp.pop_back();
			if (flag)
			{
				while (s[i].comTok.lex.text != "ENDIF")
				{
					++codeInd;
					++i;
				}
				tmp.push_back(code[codeInd - 1]);
				++i;
				++codeInd;
			}
			else
				tmp.push_back(code[codeInd - 1]);
				
		}
		else if (s[i].comTok.lex.text == "ENDIF")
		{
			tmp.pop_back();
			tmp.push_back(code[codeInd - 1]);
		}
		else if (s[i].comTok.lex.text == "END")
		{
			tmpSeg.length = tmpVal;
			tmpSeg.name = segName;
			segTable.push_back(tmpSeg);
			tmp.push_back(code[codeInd]);
		}
		else if (s[i].comTok.lex.text == "JB")
		{
			bool uppF = false;
			for (int j = 0; j < identTable.size(); ++j)
				if (identTable[j].name == s[i].Operands[0].OpV[0].lex.text)
					uppF = true;

			if (uppF)
				tmpByte += 2;
			else
				tmpByte += 6;
		}
		else if (s[i].comTok.lex.text == "")
		{
			tmpIdent.value = tmpVal;
			tmpIdent.attr = segName;
			tmpIdent.name = s[i].nameTok.lex.text;
			tmpIdent.type = "L NEAR";
		}

		++i;
	}
	return tmp;
};

string intToHex(int dec)
{
	stringstream stream;
	string result = "";

	stream << std::hex << dec;
	result = stream.str();
	for (int i = 0; i < result.length(); ++i)
		result[i] = toupper(result[i]);
	return result;
}

int getOpByte(vector<Token> t, vector<ident> identTable)
{
	int tmpByte = 0;

	if (t[0].lexemType == LexType::REG8 && t.size() == 1)
		return 1;
	else if (t[0].lexemType == LexType::REG32 && t.size() == 1)
		return 0;
	else if (t[0].lexemType == LexType::IDENT_TYPE)
	{
		if (t[0].lex.text == "BYTE")
			tmpByte += 1;
		else if (t[0].lex.text == "WORD")
			tmpByte += 1;
		else if (t[0].lex.text == "DWORD")
			tmpByte += 1;

		for (int i = 1; i < t.size(); ++i)
		{
			if (t[i].lexemType == LexType::SEG_REG || t[i].lexemType == LexType::REG32)
				++tmpByte;
		}
	}
	else if (t[0].lex.text == "[")
		for (int i = 1; i < t.size(); ++i)
		{
			if (t[i].lexemType == LexType::SEG_REG || t[i].lexemType == LexType::REG32)
				++tmpByte;
		}
	else if (t[0].lexemType == LexType::USER_IDENT)
	{
		for (int i = 0; i < identTable.size(); ++i)
			if (t[0].lex.text == identTable[i].name)
				if (identTable[i].type == "L BYTE")
					tmpByte += 1;
				else if (identTable[i].type == "L WORD")
					tmpByte += 2;
				else if (identTable[i].type == "L DWORD")
					tmpByte += 4;
	}
	else if (t[0].lexemType == LexType::DEC_CONST || t[0].lexemType == LexType::BIN_CONST || t[0].lexemType == LexType::HEX_CONST)
		return 0;

	return tmpByte;
};

void outputIntBytes(vector<string> s, vector<Token> lex)
{
	int j = 0;
	for (int i = 0; i < s.size(); ++i)
	{
		int row = lex[j].lex.row;
		cout << s[i];
		cout << endl;
	}

	cout << endl << endl;
	cout << setw(8) << "NAME" << setw(10) << "SIZE" << setw(10) << "LENGTH" << setw(10) << "ALIGN" << setw(20) << "COMB. CLASS" << endl << endl;

	for (int i = segTable.size()-1; i >= 0 ; --i)
	{
		cout << setw(8) << segTable[i].name << setw(10) << "32 BIT" << setw(10) << segTable[i].length << setw(10) << "PARA" << setw(20) << "NONE" << endl << endl;
	}

	cout << endl << endl;
	cout << setw(8) << "NAME" << setw(15) << "TYPE" << setw(15) << "VALUE" << setw(15) << "ATTR" << endl << endl;

	for (int i = 0; i < identTable.size(); ++i)
	{
		cout << setw(8) << identTable[i].name << setw(15) << identTable[i].type << setw(15) << identTable[i].value << setw(15) << identTable[i].attr << endl << endl;
	}
	
};