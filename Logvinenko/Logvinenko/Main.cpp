#pragma warning(disable: 4018)

#include "Lexical_analizer.h"
#include "Sentence.h"

int main()
{
	string name = "TEST";
	vector<string> code;
	vector<Lexem> lexems;
	vector<Token> tokens;

	ifstream sourceFile(name+".asm");

	if (sourceFile.is_open())
	{
		string tmp;
		while (getline(sourceFile, tmp))
			code.push_back(tmp);

		sourceFile.close();

		bool flag = false;
		for (auto &iter : code)
		{
			int i = 0;
			while (i < iter.length())
			{
				if (isQuote(iter[i]) && !flag)
					flag = true;
				else if (isQuote(iter[i]) && flag)
					flag = false;

				if (!flag)
					iter[i] = toupper(iter[i]);
				++i;
			}
			flag = false;
		}

		GenerateLexemVector(code, lexems);
		AnalizeLexems(lexems, tokens);
		OutputTokens(tokens, name);
		vector<Sentence> sen = getSentenceTable(tokens);
		OutputSentences(sen, name);
	}
	else
	{
		setlocale(LC_CTYPE, "rus");
		cout << "ÔÀÉË ÍÅ ÇÍÀÉÄÅÍÎ!";
		exit(1);
	}
	return 0;
}