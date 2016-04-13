#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <set>
#include <vector>
#include <fstream>

using namespace std;

bool directiveCheck(string s);
bool dataTypeCheck(string s);
bool reg32Check(string s);
bool reg8Check(string s);
bool segmentRegCheck(string s);
bool commandCheck(string s);
bool identCheck(string s);
bool operatorCheck(string s);
bool alphabetCheck(char ch);
bool identCharCheck(char ch);
bool numCheck(char ch);
bool singleCheck(char ch);
bool dividerCheck(char ch);
bool quoteCheck(char ch);
bool commentCheck(char ch);