#pragma warning(disable: 4018)
#include "SyntaxAnalizer.h"

int main()
{
	string name;
	cout << "ENTER NAME OF YOUR FILE(.asm): ";
	cin >> name;
	cout << endl << endl;
	LexicalAnalizer NewLexemTable(name + ".asm");
	vector<Sentence> sen = getSentenceTable(NewLexemTable.getTokens());
	vector<string> code = NewLexemTable.getCode();
	vector<list> intBytes = getIntBytes(code, sen);
	outputIntBytes(getComCodes(intBytes, sen), NewLexemTable.getTokens(), name);

	system("PAUSE");
	return 0;
}