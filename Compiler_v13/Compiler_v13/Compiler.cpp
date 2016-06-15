#pragma warning(disable: 4018)
#include "SyntaxAnalizer.h"

bool ERR_FLAG = false;

int main()
{
	string name;
	cout << "ENTER NAME OF YOUR FILE(.asm): ";
	cin >> name;
	cout << endl << endl;
	LexicalAnalizer NewLexemTable(name + ".asm");
	vector<Sentence> sen = getSentenceTable(NewLexemTable.getTokens());
	//OutputSentences(sen, name);
	vector<string> code = NewLexemTable.getCode();
	vector<list> intBytes = getIntBytes(code, sen);
	
	if (ERR_FLAG == false)
		outputIntBytes(getComCodes(intBytes, sen), NewLexemTable.getTokens(), name);

	system("PAUSE");
	return 0;
}