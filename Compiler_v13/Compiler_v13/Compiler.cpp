#pragma warning(disable: 4018)
#include "SyntaxAnalizer.h"

int main()
{
	string name = "KR";
	LexicalAnalizer NewLexemTable(name + ".asm");
	vector<Sentence> sen = getSentenceTable(NewLexemTable.getTokens());
	OutputSentences(sen, name);
	vector<string> code = NewLexemTable.getCode();
	vector<string> intBytes = getIntBytes(code, sen);
	outputIntBytes(intBytes, NewLexemTable.getTokens());

	return 0;
}