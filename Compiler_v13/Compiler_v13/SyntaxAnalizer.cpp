#pragma once

#include "SyntaxAnalizer.h"

vector<list> getIntBytes(vector<string> code, vector<Sentence> s)
{
	ident tmpIdent;
	list tmp;
	vector<list> tmpList;
	string tmpByteS;
	int tmpByte = 0;
	string segName;
	bool flag;
	bool endsF = false;
	int codeInd = 0;
	string tmpVal = "";
	segment tmpSeg;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
					tmp.code = code[codeInd];
				else
				{
					while (code[codeInd] == "")
						++codeInd;
					tmp.code = code[codeInd];
				}
				++codeInd;
				tmp.byte = tmpByteS;
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
					tmp.code = code[codeInd];
				else
				{
					while (code[codeInd] == "")
						++codeInd;
					tmp.code = code[codeInd];
				}
				++codeInd;
				tmp.byte = tmpByteS;
				tmpByteS.clear();
			}
			tmpList.push_back(tmp);
		}
				

		if (s[i].comTok.lex.text == "SEGMENT")
		{
			tmpByte = 0;
			segName = s[i].nameTok.lex.text;
			endsF = false;
		}
		else if (s[i].comTok.lex.text == "DB")
		{
			if (s[i].Operands.size() > 1 || s[i].Operands[0].OpV.size() > 1)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:" ;
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}


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
			if (s[i].Operands.size() > 1 || s[i].Operands[0].OpV.size() > 1)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}

			tmpIdent.value = tmpVal;
			tmpByte += 4;
			tmpIdent.attr = segName;
			tmpIdent.name = s[i].nameTok.lex.text;
			tmpIdent.type = "L WORD";
		}
		else if (s[i].comTok.lex.text == "DW")
		{
			if (s[i].Operands.size() > 1 || s[i].Operands[0].OpV.size() > 1)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}

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
			if (s[i].Operands.size() > 1 || s[i].Operands[0].OpV.size() > 1)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}

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
				tmp.code = code[codeInd];
			else
			{
				while (code[codeInd] == "")
					++codeInd;
				tmp.code = code[codeInd];
			}
			++codeInd;
			tmp.byte = tmpByteS;
			tmpList.push_back(tmp);
			tmpByteS.clear();

			equ tmpEqu;
			tmpEqu.name = s[i].nameTok.lex.text;
			if (s[i].Operands[0].OpV[0].lex.text == "0")
				tmpEqu.flag = 0;
			else
				tmpEqu.flag = 1;

			equV.push_back(tmpEqu);

			if (s[i+1].comTok.lex.text == "SEGMENT")
				endsF = false;
		}
		else if (s[i].comTok.lex.text == "NOP")
		{
			if (s[i].Operands.size() > 0)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
				tmpByte += 1;
		}
		else if (s[i].comTok.lex.text == "INC")
		{
			if (s[i].Operands.size() > 1 || getOpType(s[i].Operands[0].OpV) != OpType::reg)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
				tmpByte += 1+getOpByte(s[i].Operands[0].OpV, identTable);
		}
		else if (s[i].comTok.lex.text == "DEC")
		{
			if (s[i].Operands.size() > 1 || getOpType(s[i].Operands[0].OpV) != OpType::mem)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
				tmpByte += 1+getOpByte(s[i].Operands[0].OpV, identTable);
		}
		else if (s[i].comTok.lex.text == "MOV")
		{	
			if (s[i].Operands.size() < 2 || getOpType(s[i].Operands[0].OpV) != OpType::reg || getOpType(s[i].Operands[1].OpV) != OpType::imm)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
			{
				if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
					tmpByte += 1;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
					tmpByte += 4;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
					tmpByte += 1;
				else
					tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);
			}			

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "AND")
		{
			if (s[i].Operands.size() < 2 || getOpType(s[i].Operands[0].OpV) != OpType::mem || getOpType(s[i].Operands[1].OpV) != OpType::reg)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
			{
				if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
					tmpByte += 1;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
					tmpByte += 4;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
					tmpByte += 1;
				else
					tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);
			}			

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "ADD")
		{
			if (s[i].Operands.size() < 2 || (getOpType(s[i].Operands[0].OpV) != OpType::reg && getOpType(s[i].Operands[0].OpV) != OpType::mem) || getOpType(s[i].Operands[1].OpV) != OpType::reg)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
			{
				if (getOpType(s[i].Operands[0].OpV) == OpType::mem)
				{
					tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);
				}
				else if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
					tmpByte += 1;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
					tmpByte += 4;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
					tmpByte += 1;
				else
					tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);
			}			

			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "CMP")
		{
			if (s[i].Operands.size() < 2 || getOpType(s[i].Operands[0].OpV) != OpType::reg || getOpType(s[i].Operands[1].OpV) != OpType::mem)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
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
				--tmpByte;
			}
			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "OR")
		{
			if (s[i].Operands.size() < 2 || getOpType(s[i].Operands[0].OpV) != OpType::mem || getOpType(s[i].Operands[1].OpV) != OpType::imm)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong operands:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
			}
			else
			{
				if (s[i].Operands.size() > 0 && s[i].Operands[0].OpV[0].lexemType == s[i].Operands[1].OpV[0].lexemType)
					tmpByte += 1;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
					tmpByte += 4;
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
					tmpByte += 1;
				else
					tmpByte += getOpByte(s[i].Operands[0].OpV, identTable);



				if (s[i].Operands[1].OpV[0].lexemType == LexType::BIN_CONST)
				{
					int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text, nullptr, 2);

					if (tmpI < 256)
						tmpByte += 1;
					else if (tmpI < 65535)
					{
						if (s[i].Operands[0].OpV[0].lex.text == "DWORD")
							tmpByte += 4;
						else
							tmpByte += 2;
					}
					else
						tmpByte += 4;
				}
				else if (s[i].Operands[1].OpV[0].lexemType == LexType::DEC_CONST)
				{
					int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text);

					if (tmpI < 256)
						tmpByte += 1;
					else if (tmpI < 65535)
					{
						if (s[i].Operands[0].OpV[0].lex.text == "DWORD")
							tmpByte += 4;
						else
							tmpByte += 2;
					}
					else
						tmpByte += 4;
				}
				else if (s[i].Operands[1].OpV[0].lexemType == LexType::HEX_CONST)
				{
					int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text, nullptr, 16);

					if (tmpI < 256)
						tmpByte += 1;
					else if (tmpI < 65535)
					{
						if (s[i].Operands[0].OpV[0].lex.text == "DWORD")
							tmpByte += 4;
						else
							tmpByte += 2;
					}
					else
						tmpByte += 4;
				}
			}
			++tmpByte;
		}
		else if (s[i].comTok.lex.text == "IF")
		{
			tmpList.pop_back();

			bool labelIsHere = false;

			for (int j = 0; j < equV.size(); ++j)
			{
				if (s[i].Operands[0].OpV[0].lex.text == equV[j].name)
				{
					labelIsHere = true;
					flag = equV[j].flag;
				}
			}

			if (!labelIsHere)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong identeficator:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << code[codeInd - 1] << endl;
				ERR_FLAG = true;
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
			{
				list tmpO;
				tmpO.code = code[codeInd - 1];
				tmpList.push_back(tmpO);
			}
		}
		else if (s[i].comTok.lex.text == "ELSE")
		{
			tmpList.pop_back();
			if (flag)
			{
				while (s[i].comTok.lex.text != "ENDIF")
				{
					++codeInd;
					++i;
				}
				list tmpO;
				tmpO.code = code[codeInd - 1];
				tmpList.push_back(tmpO);
				++i;
				++codeInd;
			}
			else
			{
				list tmpO;
				tmpO.code = code[codeInd - 1];
				tmpList.push_back(tmpO);
			}
				
		}
		else if (s[i].comTok.lex.text == "ENDIF")
		{
			tmpList.pop_back();
			list tmpO;
			tmpO.code = code[codeInd - 1];
			tmpList.push_back(tmpO);
		}
		else if (s[i].comTok.lex.text == "END")
		{
			tmpSeg.length = tmpVal;
			tmpSeg.name = segName;
			segTable.push_back(tmpSeg);

			list tmpO;
			tmpO.code = code[codeInd];
			tmpList.push_back(tmpO);
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
	return tmpList;
};

vector<list> getComCodes(vector<list> l, vector<Sentence> s)
{
	ident tmpIdent;
	vector<list> tmpL;
	list tmp;
	string tmpCode;
	bool flag;
	bool endsF = false;
	int codeInd = 0;
	segment tmpSeg;
	int lIndex = 0;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int i = 0;
	while (i < s.size())
	{
		tmpCode = "";
		if (lIndex > l.size()-1)
			break;
		else
			tmp = l[lIndex];

		if (s[i].comTok.lex.text == "SEGMENT")
		{
			tmpCode = "";
		}
		else if (s[i].comTok.lex.text == "DB")
		{
			if (s[i].Operands[0].OpV[0].lexemType == LexType::TEXT_CONST)
			{
				
				tmpCode = stringToHex(s[i].Operands[0].OpV[0].lex.text);

				for (int i = 0; i < tmpCode.length(); ++i)
					tmpCode[i] = toupper(tmpCode[i]);
			}
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::DEC_CONST)
			{
				int j = 0;
				string tmpOp;
				tmpOp = s[i].Operands[0].OpV[0].lex.text;
				while (tmpOp[j] != char(68))
				{
					tmpCode += tmpOp[j];
					++j;
				}
				int tmpInt = stoi(tmpCode, nullptr);
				tmpCode = intToHex(tmpInt);
			}
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::BIN_CONST)
			{
				int j = 0;
				string tmpOp;
				tmpOp = s[i].Operands[0].OpV[0].lex.text;
				while (tmpOp[j] != char(66))
				{
					tmpCode += tmpOp[j];
					++j;
				}
				int tmpInt = stoi(tmpCode, nullptr, 2);
				tmpCode = intToHex(tmpInt);
			}
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::HEX_CONST)
			{
				string tmpOp;
				tmpOp = s[i].Operands[0].OpV[0].lex.text;
				tmpOp.pop_back();
				tmpCode = setReversedByteSequence(tmpOp, 1);
			}
		}
		else if (s[i].comTok.lex.text == "DD")
		{
			if (s[i].Operands[0].OpV[0].lexemType == LexType::DEC_CONST)
			{
				int j = 0;
				string tmpOp;
				tmpOp = s[i].Operands[0].OpV[0].lex.text;
				while (tmpOp[j] != char(68))
				{
					tmpCode += tmpOp[j];
					++j;
				}
				int tmpInt = stoi(tmpCode, nullptr);
				tmpCode = intToHex(tmpInt);
			}
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::BIN_CONST)
			{
				int j = 0;
				string tmpOp;
				tmpOp = s[i].Operands[0].OpV[0].lex.text;
				while (tmpOp[j] != char(66))
				{
					tmpCode += tmpOp[j];
					++j;
				}
				int tmpInt = stoi(tmpCode, nullptr, 2);
				tmpCode = intToHex(tmpInt);
			}
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::HEX_CONST)
			{
				string tmpOp;
				tmpOp = s[i].Operands[0].OpV[0].lex.text;
				tmpOp.pop_back();
				tmpCode = setReversedByteSequence(tmpOp, 2);
				for (int j = tmpCode.length(); j < 8; ++j)
					tmpCode += "0";
			}
		}
		else if (s[i].comTok.lex.text == "DW")
		{
			if (s[i].Operands[0].OpV[0].lexemType == LexType::DEC_CONST)
			{
				int tmpInt = stoi(s[i].Operands[0].OpV[0].lex.text, nullptr);
				tmpCode = intToHex(tmpInt);
			}
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::BIN_CONST)
			{
				int tmpInt = stoi(s[i].Operands[0].OpV[0].lex.text, nullptr, 2);
				tmpCode = intToHex(tmpInt);
			}
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::HEX_CONST)
			{
				string tmpOp;
				tmpOp = s[i].Operands[0].OpV[0].lex.text;
				tmpOp.pop_back();
				tmpCode = setReversedByteSequence(tmpOp, 4);
			}
		}
		else if (s[i].comTok.lex.text == "ENDS")
		{
			tmpCode.clear();
		}
		else if (s[i].comTok.lex.text == "EQU")
		{
			tmpCode.clear();
		}
		else if (s[i].comTok.lex.text == "NOP")
		{
			tmpCode = "90";
		}
		else if (s[i].comTok.lex.text == "INC")
		{
			if (s[i].Operands[0].OpV[0].lex.text == "EAX" || s[i].Operands[0].OpV[0].lex.text == "AX")
				tmpCode = "40";
			else if (s[i].Operands[0].OpV[0].lex.text == "ECX" || s[i].Operands[0].OpV[0].lex.text == "CX")
				tmpCode = "41";
			else if (s[i].Operands[0].OpV[0].lex.text == "EDX" || s[i].Operands[0].OpV[0].lex.text == "DX")
				tmpCode = "42";
			else if (s[i].Operands[0].OpV[0].lex.text == "EBX" || s[i].Operands[0].OpV[0].lex.text == "BX")
				tmpCode = "43";
			else if (s[i].Operands[0].OpV[0].lex.text == "ESP" || s[i].Operands[0].OpV[0].lex.text == "SP")
				tmpCode = "44";
			else if (s[i].Operands[0].OpV[0].lex.text == "EBP" || s[i].Operands[0].OpV[0].lex.text == "BP")
				tmpCode = "45";
			else if (s[i].Operands[0].OpV[0].lex.text == "ESI" || s[i].Operands[0].OpV[0].lex.text == "SI")
				tmpCode = "46";
			else if (s[i].Operands[0].OpV[0].lex.text == "EDI" || s[i].Operands[0].OpV[0].lex.text == "DI")
				tmpCode = "47";
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
			{
				tmpCode = "FE";

				if (s[i].Operands[0].OpV[0].lex.text == "AL")
					tmpCode += " C0";
				else if (s[i].Operands[0].OpV[0].lex.text == "CL")
					tmpCode += " C1";
				else if (s[i].Operands[0].OpV[0].lex.text == "DL")
					tmpCode += " C2";
				else if (s[i].Operands[0].OpV[0].lex.text == "BL")
					tmpCode += " C3";
				else if (s[i].Operands[0].OpV[0].lex.text == "AH")
					tmpCode += " C4";
				else if (s[i].Operands[0].OpV[0].lex.text == "CH")
					tmpCode += " C5";
				else if (s[i].Operands[0].OpV[0].lex.text == "DH")
					tmpCode += " C6";
				else if (s[i].Operands[0].OpV[0].lex.text == "BH")
					tmpCode += " C7";
			}
		}
		else if (s[i].comTok.lex.text == "DEC")
		{
			for ( int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
			{
				if (s[i].Operands[0].OpV[j].lex.text == "ES")
					tmpCode += "26: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "CS")
					tmpCode += "2E: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "SS")
					tmpCode += "36: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "DS")
					tmpCode += "3E: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "FS")
					tmpCode += "64: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "GS")
					tmpCode += "65: ";
			}

			if (s[i].Operands[0].OpV[0].lex.text == "WORD")
				tmpCode += "66| ";

			if (s[i].Operands[0].OpV[0].lex.text == "BYTE")
				tmpCode += "FE ";
			else
				tmpCode += "FF ";

			bool lflag = false;
			string tmpL = "";
			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
				if (s[i].Operands[0].OpV[j].lexemType == LexType::USER_IDENT)
				{
					lflag = true;
					tmpL = s[i].Operands[0].OpV[j].lex.text;
				}

			if (lflag == true)
				tmpCode += "8C";
			else
				tmpCode += "4C";

			string tmpRs = "";
			int tmpI;

			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
				if (s[i].Operands[0].OpV[j].lex.text == "[")
				{
					if (j + 6 <= s[i].Operands[0].OpV.size())
					{
						tmpRs += s[i].Operands[0].OpV[j + 1].lex.text + s[i].Operands[0].OpV[j + 2].lex.text + s[i].Operands[0].OpV[j + 3].lex.text;
						tmpI = stoi(s[i].Operands[0].OpV[j + 5].lex.text);
						if (s[i].Operands[0].OpV[j + 6].lex.text != "]")
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Wrong operands:";
							SetConsoleTextAttribute(hConsole, 7);
							cout << tmp.code << endl;
							ERR_FLAG = true;
						}
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Wrong operands:";
						SetConsoleTextAttribute(hConsole, 7);
						cout << tmp.code << endl;
						ERR_FLAG = true;
					}
				}

			if (lflag != true)
				tmpCode += getSCom(tmpRs) + " " + "0" + intToHex(tmpI);
			else
			{
				for (int p = 0; p < identTable.size(); ++p)
					if (identTable[p].name == tmpL)
						tmpI += stoi(identTable[p].value, nullptr, 16);

				string tmpSm;
				for (int p = 0; p < 8 - intToHex(tmpI).length(); ++p)
					tmpSm += "0";

				tmpSm += intToHex(tmpI);
				tmpCode += getSCom(tmpRs) + " " + tmpSm + " R";
			}
		}
		else if (s[i].comTok.lex.text == "MOV")
		{
			bool equFlag = false;

			for (int j = 0; j < equV.size(); ++j)
				if (s[i].Operands[1].OpV[0].lex.text == equV[j].name)
					equFlag = true;

			if (equFlag == true || s[i].Operands[1].OpV[0].lexemType == LexType::DEC_CONST || s[i].Operands[1].OpV[0].lexemType == LexType::HEX_CONST || s[i].Operands[1].OpV[0].lexemType == LexType::BIN_CONST)
			{
				if (s[i].Operands[0].OpV[0].lex.text == "CL")
					tmpCode += "B1";
				else if (s[i].Operands[0].OpV[0].lex.text == "DL")
					tmpCode += "B2";
				else if (s[i].Operands[0].OpV[0].lex.text == "BL")
					tmpCode += "B3";
				else if (s[i].Operands[0].OpV[0].lex.text == "AH")
					tmpCode += "B4";
				else if (s[i].Operands[0].OpV[0].lex.text == "CH")
					tmpCode += "B5";
				else if (s[i].Operands[0].OpV[0].lex.text == "DH")
					tmpCode += "B6";
				else if (s[i].Operands[0].OpV[0].lex.text == "BH")
					tmpCode += "B7";
				else if (s[i].Operands[0].OpV[0].lex.text == "EAX")
					tmpCode += "B8";
				else if (s[i].Operands[0].OpV[0].lex.text == "ECX")
					tmpCode += "B9";
				else if (s[i].Operands[0].OpV[0].lex.text == "EDX")
					tmpCode += "BA";
				else if (s[i].Operands[0].OpV[0].lex.text == "EBX")
					tmpCode += "BB";
				else if (s[i].Operands[0].OpV[0].lex.text == "ESP")
					tmpCode += "BC";
				else if (s[i].Operands[0].OpV[0].lex.text == "EBP")
					tmpCode += "BD";
				else if (s[i].Operands[0].OpV[0].lex.text == "ESI")
					tmpCode += "BE";
				else if (s[i].Operands[0].OpV[0].lex.text == "EDI")
					tmpCode += "BF";

				if (s[i].Operands[1].OpV[0].lexemType == LexType::BIN_CONST)
				{
					int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text, nullptr, 2);
					tmpCode += " ";
					if (s[i].Operands[1].OpV[0].lexemType == LexType::REG32)
						for (int j = intToHex(tmpI).length(); j < 8;++j)
							tmpCode += "0";
					else
						for (int j = intToHex(tmpI).length(); j < 2; ++j)
							tmpCode += "0";
					tmpCode += intToHex(tmpI);
				}
				else if (s[i].Operands[1].OpV[0].lexemType == LexType::DEC_CONST)
				{
					int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text);
					tmpCode += " ";
					if (s[i].Operands[1].OpV[0].lexemType == LexType::REG32)
						for (int j = intToHex(tmpI).length(); j < 8; ++j)
							tmpCode += "0";
					else
						for (int j = intToHex(tmpI).length(); j < 2; ++j)
							tmpCode += "0";
					tmpCode += intToHex(tmpI);
				}
				else if (s[i].Operands[1].OpV[0].lexemType == LexType::HEX_CONST)
				{
					int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text, nullptr, 16);
					tmpCode += " ";
					if (s[i].Operands[1].OpV[0].lexemType == LexType::REG32)
						for (int j = intToHex(tmpI).length(); j < 8; ++j)
							tmpCode += "0";
					else
						for (int j = intToHex(tmpI).length(); j < 2; ++j)
							tmpCode += "0";
					tmpCode += intToHex(tmpI);
				}
				else if (s[i].Operands[1].OpV[0].lexemType == LexType::USER_IDENT)
				{
					tmpCode += " ";

					string tmpV;
					for (int j = 0; j < identTable.size(); ++j)
					{
						if (identTable[j].name == s[i].Operands[1].OpV[0].lex.text)
						{
							tmpV += identTable[j].value;
							break;
						}
					}
						

					if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
					{
						for (int j = 0; j < 4; ++j)
							tmpCode += "0";
						tmpCode += tmpV;
					}
					else
						tmpCode += tmpV[2] + tmpV[3];
				}

			}
			else if (s[i].Operands[1].OpV[0].lexemType == LexType::USER_IDENT)
			{
				if (s[i].Operands[0].OpV[0].lex.text == "AL")
					tmpCode += "A0";
				else if (s[i].Operands[0].OpV[0].lex.text == "AX" || s[i].Operands[0].OpV[0].lex.text == "EAX")
					tmpCode += "A1";
				else 
				{
					if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
						tmpCode += "8B";
					else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
						tmpCode += "8A";

					tmpCode += " ";

					if (s[i].Operands[0].OpV[0].lex.text == "AL" || s[i].Operands[0].OpV[0].lex.text == "EAX")
						tmpCode += "05";
					else if (s[i].Operands[0].OpV[0].lex.text == "CL" || s[i].Operands[0].OpV[0].lex.text == "ECX")
						tmpCode += "0D";
					else if (s[i].Operands[0].OpV[0].lex.text == "DL" || s[i].Operands[0].OpV[0].lex.text == "EDX")
						tmpCode += "15";
					else if (s[i].Operands[0].OpV[0].lex.text == "BL" || s[i].Operands[0].OpV[0].lex.text == "EBX")
						tmpCode += "1D";
					else if (s[i].Operands[0].OpV[0].lex.text == "AH" || s[i].Operands[0].OpV[0].lex.text == "ESP")
						tmpCode += "25";
					else if (s[i].Operands[0].OpV[0].lex.text == "CH" || s[i].Operands[0].OpV[0].lex.text == "EBP")
						tmpCode += "2D";
					else if (s[i].Operands[0].OpV[0].lex.text == "DH" || s[i].Operands[0].OpV[0].lex.text == "ESI")
						tmpCode += "35";
					else if (s[i].Operands[0].OpV[0].lex.text == "BH" || s[i].Operands[0].OpV[0].lex.text == "EDI")
						tmpCode += "3D";
				}

				tmpCode += " ";

				string tmpV;
				for (int j = 0; j < identTable.size(); ++j)
				{
					if (identTable[j].name == s[i].Operands[1].OpV[0].lex.text)
					{
						tmpV += identTable[j].value;
						break;
					}
				}


				if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
				{
					for (int j = 0; j < 4; ++j)
						tmpCode += "0";
					tmpCode += tmpV;
				}
				else
					tmpCode += tmpV[2] + tmpV[3];

				tmpCode += " R";
			}
		}
		else if (s[i].comTok.lex.text == "CMP")
		{
			for (int j = 0; j < s[i].Operands[1].OpV.size(); ++j)
			{
				if (s[i].Operands[1].OpV[j].lex.text == "ES")
					tmpCode += "26: ";
				else if (s[i].Operands[1].OpV[j].lex.text == "CS")
					tmpCode += "2E: ";
				else if (s[i].Operands[1].OpV[j].lex.text == "SS")
					tmpCode += "36: ";
				else if (s[i].Operands[1].OpV[j].lex.text == "DS")
					tmpCode += "3E: ";
				else if (s[i].Operands[1].OpV[j].lex.text == "FS")
					tmpCode += "64: ";
				else if (s[i].Operands[1].OpV[j].lex.text == "GS")
					tmpCode += "65: ";
			}

			if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
				tmpCode += "3B ";
			else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
				tmpCode += "3A ";

			bool lflag = false;
			string tmpL = "";
			for (int j = 0; j < s[i].Operands[1].OpV.size(); ++j)
				if (s[i].Operands[1].OpV[j].lexemType == LexType::USER_IDENT)
				{
					lflag = true;
					tmpL = s[i].Operands[1].OpV[j].lex.text;
				}

			if (lflag)
			{
				if (s[i].Operands[0].OpV[0].lex.text == "AL" || s[i].Operands[0].OpV[0].lex.text == "EAX")
					tmpCode += "84";
				else if (s[i].Operands[0].OpV[0].lex.text == "CL" || s[i].Operands[0].OpV[0].lex.text == "ECX")
					tmpCode += "8C";
				else if (s[i].Operands[0].OpV[0].lex.text == "DL" || s[i].Operands[0].OpV[0].lex.text == "EDX")
					tmpCode += "94";
				else if (s[i].Operands[0].OpV[0].lex.text == "BL" || s[i].Operands[0].OpV[0].lex.text == "EBX")
					tmpCode += "9C";
				else if (s[i].Operands[0].OpV[0].lex.text == "AH" || s[i].Operands[0].OpV[0].lex.text == "ESP")
					tmpCode += "A4";
				else if (s[i].Operands[0].OpV[0].lex.text == "CH" || s[i].Operands[0].OpV[0].lex.text == "EBP")
					tmpCode += "AC";
				else if (s[i].Operands[0].OpV[0].lex.text == "DH" || s[i].Operands[0].OpV[0].lex.text == "ESI")
					tmpCode += "B4";
				else if (s[i].Operands[0].OpV[0].lex.text == "BH" || s[i].Operands[0].OpV[0].lex.text == "EDI")
					tmpCode += "BC";
			}
			else
			{
				if (s[i].Operands[0].OpV[0].lex.text == "AL" || s[i].Operands[0].OpV[0].lex.text == "EAX")
					tmpCode += "44";
				else if (s[i].Operands[0].OpV[0].lex.text == "CL" || s[i].Operands[0].OpV[0].lex.text == "ECX")
					tmpCode += "4C";
				else if (s[i].Operands[0].OpV[0].lex.text == "DL" || s[i].Operands[0].OpV[0].lex.text == "EDX")
					tmpCode += "54";
				else if (s[i].Operands[0].OpV[0].lex.text == "BL" || s[i].Operands[0].OpV[0].lex.text == "EBX")
					tmpCode += "5C";
				else if (s[i].Operands[0].OpV[0].lex.text == "AH" || s[i].Operands[0].OpV[0].lex.text == "ESP")
					tmpCode += "64";
				else if (s[i].Operands[0].OpV[0].lex.text == "CH" || s[i].Operands[0].OpV[0].lex.text == "EBP")
					tmpCode += "6C";
				else if (s[i].Operands[0].OpV[0].lex.text == "DH" || s[i].Operands[0].OpV[0].lex.text == "ESI")
					tmpCode += "74";
				else if (s[i].Operands[0].OpV[0].lex.text == "BH" || s[i].Operands[0].OpV[0].lex.text == "EDI")
					tmpCode += "7C";
			}
			

			string tmpRs = "";
			int tmpI;

			for (int j = 0; j < s[i].Operands[1].OpV.size(); ++j)
				if (s[i].Operands[1].OpV[j].lex.text == "[")
					if (j + 6 <= s[i].Operands[1].OpV.size())
					{
						tmpRs += s[i].Operands[1].OpV[j + 1].lex.text + s[i].Operands[1].OpV[j + 2].lex.text + s[i].Operands[1].OpV[j + 3].lex.text;
						tmpI = stoi(s[i].Operands[1].OpV[j + 5].lex.text);
						if (s[i].Operands[1].OpV[j + 6].lex.text != "]")
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Wrong operands:";
							SetConsoleTextAttribute(hConsole, 7);
							cout << tmp.code << endl;
							ERR_FLAG = true;
						}
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Wrong operands:";
						SetConsoleTextAttribute(hConsole, 7);
						cout << tmp.code << endl;
						ERR_FLAG = true;
					}

			if (lflag != true)
				tmpCode += getSCom(tmpRs) + " " + "0" + intToHex(tmpI);
			else
			{
				for (int p = 0; p < identTable.size(); ++p)
					if (identTable[p].name == tmpL)
						tmpI += stoi(identTable[p].value, nullptr, 16);

				string tmpSm;
				for (int p = 0; p < 8 - intToHex(tmpI).length(); ++p)
					tmpSm += "0";

				tmpSm += intToHex(tmpI);
				tmpCode += getSCom(tmpRs) + " " + tmpSm + " R";
			}

		}
		else if (s[i].comTok.lex.text == "ADD")
		{
			if (getOpType(s[i].Operands[0].OpV) == OpType::mem)
			{
				for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
				{
					if (s[i].Operands[0].OpV[j].lex.text == "ES")
						tmpCode += "26: ";
					else if (s[i].Operands[0].OpV[j].lex.text == "CS")
						tmpCode += "2E: ";
					else if (s[i].Operands[0].OpV[j].lex.text == "SS")
						tmpCode += "36: ";
					else if (s[i].Operands[0].OpV[j].lex.text == "DS")
						tmpCode += "3E: ";
					else if (s[i].Operands[0].OpV[j].lex.text == "FS")
						tmpCode += "64: ";
					else if (s[i].Operands[0].OpV[j].lex.text == "GS")
						tmpCode += "65: ";
				}

				if (s[i].Operands[1].OpV[0].lexemType == LexType::REG32)
					tmpCode += "01 ";
				else if (s[i].Operands[1].OpV[0].lexemType == LexType::REG8)
					tmpCode += "00 ";

				bool lflag = false;
				string tmpL = "";
				for (int j = 0; j < s[i].Operands[1].OpV.size(); ++j)
					if (s[i].Operands[1].OpV[j].lexemType == LexType::USER_IDENT)
					{
					lflag = true;
					tmpL = s[i].Operands[1].OpV[j].lex.text;
					}

				if (lflag)
				{
					if (s[i].Operands[1].OpV[0].lex.text == "AL" || s[i].Operands[1].OpV[0].lex.text == "EAX")
						tmpCode += "84";
					else if (s[i].Operands[1].OpV[0].lex.text == "CL" || s[i].Operands[1].OpV[0].lex.text == "ECX")
						tmpCode += "8C";
					else if (s[i].Operands[1].OpV[0].lex.text == "DL" || s[i].Operands[1].OpV[0].lex.text == "EDX")
						tmpCode += "94";
					else if (s[i].Operands[1].OpV[0].lex.text == "BL" || s[i].Operands[1].OpV[0].lex.text == "EBX")
						tmpCode += "9C";
					else if (s[i].Operands[1].OpV[0].lex.text == "AH" || s[i].Operands[1].OpV[0].lex.text == "ESP")
						tmpCode += "A4";
					else if (s[i].Operands[1].OpV[0].lex.text == "CH" || s[i].Operands[1].OpV[0].lex.text == "EBP")
						tmpCode += "AC";
					else if (s[i].Operands[1].OpV[0].lex.text == "DH" || s[i].Operands[1].OpV[0].lex.text == "ESI")
						tmpCode += "B4";
					else if (s[i].Operands[1].OpV[0].lex.text == "BH" || s[i].Operands[1].OpV[0].lex.text == "EDI")
						tmpCode += "BC";
				}
				else
				{
					if (s[i].Operands[1].OpV[0].lex.text == "AL" || s[i].Operands[1].OpV[0].lex.text == "EAX")
						tmpCode += "44";
					else if (s[i].Operands[1].OpV[0].lex.text == "CL" || s[i].Operands[1].OpV[0].lex.text == "ECX")
						tmpCode += "4C";
					else if (s[i].Operands[1].OpV[0].lex.text == "DL" || s[i].Operands[1].OpV[0].lex.text == "EDX")
						tmpCode += "54";
					else if (s[i].Operands[1].OpV[0].lex.text == "BL" || s[i].Operands[1].OpV[0].lex.text == "EBX")
						tmpCode += "5C";
					else if (s[i].Operands[1].OpV[0].lex.text == "AH" || s[i].Operands[1].OpV[0].lex.text == "ESP")
						tmpCode += "64";
					else if (s[i].Operands[1].OpV[0].lex.text == "CH" || s[i].Operands[1].OpV[0].lex.text == "EBP")
						tmpCode += "6C";
					else if (s[i].Operands[1].OpV[0].lex.text == "DH" || s[i].Operands[1].OpV[0].lex.text == "ESI")
						tmpCode += "74";
					else if (s[i].Operands[1].OpV[0].lex.text == "BH" || s[i].Operands[1].OpV[0].lex.text == "EDI")
						tmpCode += "7C";
				}

				string tmpRs = "";
				int tmpI;

				for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
					if (s[i].Operands[0].OpV[j].lex.text == "[")
						if (j + 6 <= s[i].Operands[0].OpV.size())
						{
					tmpRs += s[i].Operands[0].OpV[j + 1].lex.text + s[i].Operands[0].OpV[j + 2].lex.text + s[i].Operands[0].OpV[j + 3].lex.text;
					tmpI = stoi(s[i].Operands[0].OpV[j + 5].lex.text);
					if (s[i].Operands[0].OpV[j + 6].lex.text != "]")
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Wrong operands:";
						SetConsoleTextAttribute(hConsole, 7);
						cout << tmp.code << endl;
						ERR_FLAG = true;
					}
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Wrong operands:";
							SetConsoleTextAttribute(hConsole, 7);
							cout << tmp.code << endl;
							ERR_FLAG = true;
						}

				if (lflag != true)
					tmpCode += getSCom(tmpRs) + " " + "0" + intToHex(tmpI);
				else
				{
					for (int p = 0; p < identTable.size(); ++p)
						if (identTable[p].name == tmpL)
							tmpI += stoi(identTable[p].value, nullptr, 16);

					string tmpSm;
					for (int p = 0; p < 8 - intToHex(tmpI).length(); ++p)
						tmpSm += "0";

					tmpSm += intToHex(tmpI);
					tmpCode += getSCom(tmpRs) + " " + tmpSm + " R";
				}
			}
			else
			{
				string tmpModRM = "11";

				if (s[i].Operands[0].OpV[0].lexemType == LexType::REG32)
					tmpCode += "03 ";
				else if (s[i].Operands[0].OpV[0].lexemType == LexType::REG8)
					tmpCode += "02 ";

				for (int j = 0; j < s[i].Operands.size(); ++j)
				{
					if (s[i].Operands[j].OpV[0].lex.text == "AL" || s[i].Operands[j].OpV[0].lex.text == "EAX")
						tmpModRM += "000";
					else if (s[i].Operands[j].OpV[0].lex.text == "CL" || s[i].Operands[j].OpV[0].lex.text == "ECX")
						tmpModRM += "001";
					else if (s[i].Operands[j].OpV[0].lex.text == "DL" || s[i].Operands[j].OpV[0].lex.text == "EDX")
						tmpModRM += "010";
					else if (s[i].Operands[j].OpV[0].lex.text == "BL" || s[i].Operands[j].OpV[0].lex.text == "EBX")
						tmpModRM += "011";
					else if (s[i].Operands[j].OpV[0].lex.text == "AH" || s[i].Operands[j].OpV[0].lex.text == "ESP")
						tmpModRM += "100";
					else if (s[i].Operands[j].OpV[0].lex.text == "CH" || s[i].Operands[j].OpV[0].lex.text == "EBP")
						tmpModRM += "101";
					else if (s[i].Operands[j].OpV[0].lex.text == "DH" || s[i].Operands[j].OpV[0].lex.text == "ESI")
						tmpModRM += "110";
					else if (s[i].Operands[j].OpV[0].lex.text == "BH" || s[i].Operands[j].OpV[0].lex.text == "EDI")
						tmpModRM += "111";
				}

				int tmpI = stoi(tmpModRM, nullptr, 2);

				tmpCode += intToHex(tmpI);
			}			
		}
		else if (s[i].comTok.lex.text == "JB")
		{
			int labelInt, jumpInt;
			jumpInt = stoi(tmp.byte, nullptr, 16);
			bool labelIsHere = false;

			for (int j = 0; j < identTable.size(); ++j)
			{
				if (identTable[j].name == s[i].Operands[0].OpV[0].lex.text)
				{
					labelInt = stoi(identTable[j].value, nullptr, 16);
					labelIsHere = true;
					break;
				}
			}

			if (!labelIsHere)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong label:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << tmp.code;
				ERR_FLAG = true;
			}

			if (jumpInt < labelInt)
			{
				tmpCode += "0F 82 ";

				string tmpSm;
				for (int p = 0; p < 8 - intToHex(labelInt).length(); ++p)
					tmpSm += "0";

				tmpCode += tmpSm + intToHex(labelInt) + " R";
			}
			else
			{
				tmpCode += "72 " + intToHex(255 - (jumpInt + 1 - labelInt));
			}
		}
		else if (s[i].comTok.lex.text == "AND")
		{
			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
			{
				if (s[i].Operands[0].OpV[j].lex.text == "ES")
					tmpCode += "26: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "CS")
					tmpCode += "2E: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "SS")
					tmpCode += "36: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "DS")
					tmpCode += "3E: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "FS")
					tmpCode += "64: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "GS")
					tmpCode += "65: ";
			}

			if (s[i].Operands[1].OpV[0].lexemType == LexType::REG32)
				tmpCode += "21 ";
			else
				tmpCode += "20 ";


			bool lflag = false;
			string tmpL = "";
			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
				if (s[i].Operands[0].OpV[j].lexemType == LexType::USER_IDENT)
				{
					lflag = true;
					tmpL = s[i].Operands[0].OpV[j].lex.text;
				}

			if (lflag)
			{
				if (s[i].Operands[1].OpV[0].lex.text == "AL" || s[i].Operands[1].OpV[0].lex.text == "EAX")
					tmpCode += "84";
				else if (s[i].Operands[1].OpV[0].lex.text == "CL" || s[i].Operands[1].OpV[0].lex.text == "ECX")
					tmpCode += "8C";
				else if (s[i].Operands[1].OpV[0].lex.text == "DL" || s[i].Operands[1].OpV[0].lex.text == "EDX")
					tmpCode += "94";
				else if (s[i].Operands[1].OpV[0].lex.text == "BL" || s[i].Operands[1].OpV[0].lex.text == "EBX")
					tmpCode += "9C";
				else if (s[i].Operands[1].OpV[0].lex.text == "AH" || s[i].Operands[1].OpV[0].lex.text == "ESP")
					tmpCode += "A4";
				else if (s[i].Operands[1].OpV[0].lex.text == "CH" || s[i].Operands[1].OpV[0].lex.text == "EBP")
					tmpCode += "AC";
				else if (s[i].Operands[1].OpV[0].lex.text == "DH" || s[i].Operands[1].OpV[0].lex.text == "ESI")
					tmpCode += "B4";
				else if (s[i].Operands[1].OpV[0].lex.text == "BH" || s[i].Operands[1].OpV[0].lex.text == "EDI")
					tmpCode += "BC";
			}
			else
			{
				if (s[i].Operands[1].OpV[0].lex.text == "AL" || s[i].Operands[1].OpV[0].lex.text == "EAX")
					tmpCode += "44";
				else if (s[i].Operands[1].OpV[0].lex.text == "CL" || s[i].Operands[1].OpV[0].lex.text == "ECX")
					tmpCode += "4C";
				else if (s[i].Operands[1].OpV[0].lex.text == "DL" || s[i].Operands[1].OpV[0].lex.text == "EDX")
					tmpCode += "54";
				else if (s[i].Operands[1].OpV[0].lex.text == "BL" || s[i].Operands[1].OpV[0].lex.text == "EBX")
					tmpCode += "5C";
				else if (s[i].Operands[1].OpV[0].lex.text == "AH" || s[i].Operands[1].OpV[0].lex.text == "ESP")
					tmpCode += "64";
				else if (s[i].Operands[1].OpV[0].lex.text == "CH" || s[i].Operands[1].OpV[0].lex.text == "EBP")
					tmpCode += "6C";
				else if (s[i].Operands[1].OpV[0].lex.text == "DH" || s[i].Operands[1].OpV[0].lex.text == "ESI")
					tmpCode += "74";
				else if (s[i].Operands[1].OpV[0].lex.text == "BH" || s[i].Operands[1].OpV[0].lex.text == "EDI")
					tmpCode += "7C";
			}

			string tmpRs = "";
			int tmpI;

			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
				if (s[i].Operands[0].OpV[j].lex.text == "[")
					if (j + 6 <= s[i].Operands[0].OpV.size())
					{
						tmpRs += s[i].Operands[0].OpV[j + 1].lex.text + s[i].Operands[0].OpV[j + 2].lex.text + s[i].Operands[0].OpV[j + 3].lex.text;
						tmpI = stoi(s[i].Operands[0].OpV[j + 5].lex.text);
						if (s[i].Operands[0].OpV[j + 6].lex.text != "]")
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Wrong operands:";
							SetConsoleTextAttribute(hConsole, 7);
							cout << tmp.code;
							ERR_FLAG = true;
						}
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Wrong operands:";
						SetConsoleTextAttribute(hConsole, 7);
						cout << tmp.code;
						ERR_FLAG = true;
					}

			if (lflag != true)
				tmpCode += getSCom(tmpRs) + " " + "0" + intToHex(tmpI);
			else
			{
				for (int p = 0; p < identTable.size(); ++p)
					if (identTable[p].name == tmpL)
						tmpI += stoi(identTable[p].value, nullptr, 16);

				string tmpSm;
				for (int p = 0; p < 8 - intToHex(tmpI).length(); ++p)
					tmpSm += "0";

				tmpSm += intToHex(tmpI);
				tmpCode += getSCom(tmpRs) + " " + tmpSm + " R";
			}
		}
		else if (s[i].comTok.lex.text == "OR")
		{
			if (s[i].Operands[0].OpV[0].lex.text == "WORD")
				tmpCode += "66| ";

			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
			{
				if (s[i].Operands[0].OpV[j].lex.text == "ES")
					tmpCode += "26: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "CS")
					tmpCode += "2E: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "SS")
					tmpCode += "36: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "DS")
					tmpCode += "3E: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "FS")
					tmpCode += "64: ";
				else if (s[i].Operands[0].OpV[j].lex.text == "GS")
					tmpCode += "65: ";
			}

			if (s[i].Operands[0].OpV[0].lex.text == "DWORD" || s[i].Operands[0].OpV[0].lex.text == "WORD")
				if (s[i].Operands[1].OpV[0].lexemType == LexType::BIN_CONST || s[i].Operands[1].OpV[0].lexemType == LexType::DEC_CONST)
					tmpCode += "83 ";
				else
					if (s[i].Operands[1].OpV[0].lex.text.length() < 4)
						tmpCode += "83 ";
					else
						tmpCode += "81 ";
			else
				tmpCode += "80 ";
		

			bool lflag = false;
			string tmpL = "";
			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
				if (s[i].Operands[0].OpV[j].lexemType == LexType::USER_IDENT)
				{
					lflag = true;
					tmpL = s[i].Operands[0].OpV[j].lex.text;
				}

			if (!lflag)
				tmpCode += "4C";
			else
				tmpCode += "8C";

			string tmpRs = "";
			int tmpI;

			for (int j = 0; j < s[i].Operands[0].OpV.size(); ++j)
				if (s[i].Operands[0].OpV[j].lex.text == "[")
					if (j + 6 <= s[i].Operands[0].OpV.size())
					{
						tmpRs += s[i].Operands[0].OpV[j + 1].lex.text + s[i].Operands[0].OpV[j + 2].lex.text + s[i].Operands[0].OpV[j + 3].lex.text;
						tmpI = stoi(s[i].Operands[0].OpV[j + 5].lex.text);
						if (s[i].Operands[0].OpV[j + 6].lex.text != "]")
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Wrong operands:";
							SetConsoleTextAttribute(hConsole, 7);
							cout << tmp.code;
							ERR_FLAG = true;
						}
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Wrong operands:";
						SetConsoleTextAttribute(hConsole, 7);
						cout << tmp.code << endl;
						ERR_FLAG = true;
					}

			if (lflag != true)
				tmpCode += getSCom(tmpRs) + " " + "0" + intToHex(tmpI);
			else
			{
				for (int p = 0; p < identTable.size(); ++p)
					if (identTable[p].name == tmpL)
						tmpI += stoi(identTable[p].value, nullptr, 16);

				string tmpSm;
				for (int p = 0; p < 8 - intToHex(tmpI).length(); ++p)
					tmpSm += "0";

				tmpSm += intToHex(tmpI);
				tmpCode += getSCom(tmpRs) + " " + tmpSm + " R ";
			}

			if (s[i].Operands[1].OpV[0].lexemType == LexType::BIN_CONST)
			{
				int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text, nullptr, 2);
				tmpCode += " ";
				int ch = 2;

				if (tmpI < 256)
					ch = 2;
				else if (tmpI < 65535)
					if (s[i].Operands[0].OpV[0].lex.text == "DWORD")
						ch = 8;
					else
						ch = 4;
				else
					ch = 8;

				for (int j = intToHex(tmpI).length(); j < ch; ++j)
					tmpCode += "0";
				tmpCode += intToHex(tmpI);
			}
			else if (s[i].Operands[1].OpV[0].lexemType == LexType::DEC_CONST)
			{
				int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text);
				tmpCode += " ";
				int ch = 2;

				if (tmpI < 256)
					ch = 2;
				else if (tmpI < 65535)
					if (s[i].Operands[0].OpV[0].lex.text == "DWORD")
						ch = 8;
					else
						ch = 4;
				else
					ch = 8;

				for (int j = intToHex(tmpI).length(); j < ch; ++j)
					tmpCode += "0";
				tmpCode += intToHex(tmpI);
			}
			else if (s[i].Operands[1].OpV[0].lexemType == LexType::HEX_CONST)
			{
				int tmpI = stoi(s[i].Operands[1].OpV[0].lex.text, nullptr, 16);
				tmpCode += " ";
				int ch = 2;

				if (tmpI < 256)
					ch = 2;
				else if (tmpI < 65535)
					if (s[i].Operands[0].OpV[0].lex.text == "DWORD")
						ch = 8;
					else
						ch = 4;
				else
					ch = 8;

				for (int j = intToHex(tmpI).length(); j < ch; ++j)
					tmpCode += "0";
				tmpCode += intToHex(tmpI);
			}

		}
		else if (s[i].comTok.lex.text == "IF")
		{
			bool labelIsHere = false;

			for (int j = 0; j < equV.size(); ++j)
			{
				if (s[i].Operands[0].OpV[0].lex.text == equV[j].name)
				{
					labelIsHere = true;
					flag = equV[j].flag;
				}
			}

			if (!labelIsHere)
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Wrong identeficator:";
				SetConsoleTextAttribute(hConsole, 7);
				cout << tmp.code << endl;
				ERR_FLAG = true;
			}

			if (!flag)
			{
				while (s[i].comTok.lex.text != "ELSE")
				{
					++i;
				}
			}
			tmpCode = "";
		}
		else if (s[i].comTok.lex.text == "ELSE")
		{
			if (flag)
			{
				while (s[i + 1].comTok.lex.text != "ENDIF")
				{
					++i;
				}
			}
			tmpCode = "";
		}
		else if (s[i].comTok.lex.text == "ENDIF")
		{
			tmpCode = "";
		}

		tmp.com = tmpCode;
		tmpL.push_back(tmp);
		++lIndex;
		++i;
	}
	return tmpL;	
}

string getSCom(string s)
{
	if (s == "EAX+EAX")
		return "00";
	else if (s == "ECX+EAX")
		return "01";
	else if (s == "EDX+EAX")
		return "02";
	else if (s == "EBX+EAX")
		return "03";
	else if (s == "ESI+EAX")
		return "06";
	else if (s == "EDI+EAX")
		return "07";
	else if (s == "EAX+ECX")
		return "08";
	else if (s == "ECX+ECX")
		return "09";
	else if (s == "EDX+ECX")
		return "0A";
	else if (s == "EBX+ECX")
		return "0B";
	else if (s == "ESI+ECX")
		return "0E";
	else if (s == "EDI+ECX")
		return "0F";
	else if (s == "EAX+EDX")
		return "10";
	else if (s == "ECX+EDX")
		return "11";
	else if (s == "EDX+EDX")
		return "12";
	else if (s == "EBX+EDX")
		return "13";
	else if (s == "ESI+EDX")
		return "16";
	else if (s == "EDI+EDX")
		return "17";
	else if (s == "EAX+EBX")
		return "18";
	else if (s == "ECX+EBX")
		return "19";
	else if (s == "EDX+EBX")
		return "1A";
	else if (s == "EBX+EBX")
		return "1B";
	else if (s == "ESI+EBX")
		return "1E";
	else if (s == "EDI+EBX")
		return "1F";
	else if (s == "EAX+ESP")
		return "20";
	else if (s == "ECX+ESP")
		return "21";
	else if (s == "EDX+ESP")
		return "22";
	else if (s == "EBX+ESP")
		return "23";
	else if (s == "ESI+ESP")
		return "26";
	else if (s == "EDI+ESP")
		return "27";
	else if (s == "EAX+EBP")
		return "28";
	else if (s == "ECX+EBP")
		return "29";
	else if (s == "EDX+EBP")
		return "2A";
	else if (s == "EBX+EBP")
		return "2B";
	else if (s == "ESI+EBP")
		return "2E";
	else if (s == "EDI+EBP")
		return "2F";
	else if (s == "EAX+ESI")
		return "30";
	else if (s == "ECX+ESI")
		return "31";
	else if (s == "EDX+ESI")
		return "32";
	else if (s == "EBX+ESI")
		return "33";
	else if (s == "ESI+ESI")
		return "36";
	else if (s == "EDI+ESI")
		return "37";
	else if (s == "EAX+EDI")
		return "38";
	else if (s == "ECX+EDI")
		return "39";
	else if (s == "EDX+EDI")
		return "3A";
	else if (s == "EBX+EDI")
		return "3B";
	else if (s == "ESI+EDI")
		return "3E";
	else if (s == "EDI+EDI")
		return "3F";
}

string stringToHex(const string input)
{
	const char* const alphabet = "0123456789ABCDEF";
	size_t len = input.length();
	string res = "";

	for (int i = 1; i < len - 1; ++i)
	{
		const unsigned char c = input[i];
		res.push_back(alphabet[c >> 4]);
		res.push_back(alphabet[c & 15]);
		res.push_back(' ');
	}

	return res;
}

OpType getOpType(vector<Token> t)
{
	if (t.size() == 1)
	{
		if (t[0].lexemType == LexType::REG8 || t[0].lexemType == LexType::REG32)
			return OpType::reg;
		else if (t[0].lexemType == LexType::BIN_CONST || t[0].lexemType == LexType::DEC_CONST || t[0].lexemType == LexType::HEX_CONST)
			return OpType::imm;
		else if (t[0].lexemType == LexType::USER_IDENT)
			return OpType::imm;
		else
			return OpType::wrong;
	}
	else
		return OpType::mem;
}


string setReversedByteSequence(const string input, short size)
{
	string temp;
	for (int i = 0; i > size*2 - input.length(); ++i)
		temp.push_back('0');
	temp += input;

	string result = "";

	for (int i = temp.size() - 2; i >= 0; i -= 2)
	{
		result.push_back(temp[i]);
		result.push_back(temp[i + 1]);
	}

	return result;
}

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
	else if (t[0].lexemType == LexType::IDENT_TYPE || t[0].lex.text == "[" || t[0].lexemType == LexType::SEG_REG)
	{
		++tmpByte;

		if (t[0].lex.text == "WORD")
			tmpByte += 1;


		for (int i = 0; i < t.size(); ++i)
		{
			if (t[i].lexemType == LexType::USER_IDENT)
			{
				tmpByte += 3;
			}
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

void outputIntBytes(vector<list> s, vector<Token> lex, string fileName)
{
	ofstream file;
	file.open(fileName + ".lst");

	int j = 0;
	for (int i = 0; i < s.size(); ++i)
	{
		int row = lex[j].lex.row;
		if (s[i].com.length() < 21)
		{
			cout << s[i].byte << " " << s[i].com << s[i].code << endl;
			file << s[i].byte << " " << s[i].com << s[i].code << endl;
		}
		else
		{
			string tmpC = s[i].com;
			cout << s[i].byte << " ";
			file << s[i].byte << " ";

			for (int p = 0; p < 21; ++p)
			{
				cout << tmpC[p];
				file << tmpC[p];
			}

			cout << s[i].code << endl;
			file << s[i].code << endl;
			int j = 21;

			while (j < tmpC.length()-21)
			{ 
				cout << "     ";
				file << "     ";
				for (int p = j; p < j + 21; ++p)
				{
					cout << tmpC[p];
					file << tmpC[p];
				}
				cout << endl;
				file << endl;
				j += 21;
			}
			cout << "     ";
			file << "     ";
			for (int p = j; p < tmpC.length() - 1; ++p)
			{
				cout << tmpC[p];
				file << tmpC[p];
			}
			cout << endl;
			file << endl;
		}
	}

	cout << endl << endl;
	file << endl << endl;
	cout << setw(8) << "NAME" << setw(10) << "SIZE" << setw(10) << "LENGTH" << setw(10) << "ALIGN" << setw(20) << "COMB. CLASS" << endl << endl;
	file << setw(8) << "NAME" << setw(10) << "SIZE" << setw(10) << "LENGTH" << setw(10) << "ALIGN" << setw(20) << "COMB. CLASS" << endl << endl;
	
	for (int i = segTable.size()-1; i >= 0 ; --i)
	{
		cout << setw(8) << segTable[i].name << setw(10) << "32 BIT" << setw(10) << segTable[i].length << setw(10) << "PARA" << setw(20) << "NONE" << endl << endl;
		file << setw(8) << segTable[i].name << setw(10) << "32 BIT" << setw(10) << segTable[i].length << setw(10) << "PARA" << setw(20) << "NONE" << endl << endl;
	}

	cout << endl << endl;
	file << endl << endl;
	cout << setw(8) << "NAME" << setw(15) << "TYPE" << setw(15) << "VALUE" << setw(15) << "ATTR" << endl << endl;
	file << setw(8) << "NAME" << setw(15) << "TYPE" << setw(15) << "VALUE" << setw(15) << "ATTR" << endl << endl;

	for (int i = 0; i < identTable.size(); ++i)
	{
		cout << setw(8) << identTable[i].name << setw(15) << identTable[i].type << setw(15) << identTable[i].value << setw(15) << identTable[i].attr << endl << endl;
		file << setw(8) << identTable[i].name << setw(15) << identTable[i].type << setw(15) << identTable[i].value << setw(15) << identTable[i].attr << endl << endl;
	}
	
};