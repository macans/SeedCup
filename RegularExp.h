#ifndef _REGULAR_EXP
#define _REGULAR_EXP
#define null  ""

#include <iostream>
#include <string>
#include <math.h>
using namespace std;

class RegularExp
{
private:
	static string re;
	static bool isNull;
	static bool isFirstCmd;
public:
	static string work(string text, int iText, int iCmd);
	RegularExp();
	~RegularExp();
	static void dealBra();
	static string get(string re, string text);
	static int match(char ch, string* ans, string text, int iText, int m, bool isMustMatch);
	static int matchNum(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum);
	static int matchWord(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum);
	static int matchSpace(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum);
	//static int matchSide(string text, int iText, int m, bool isMustMatch);
//	static int matchAny(string text, int iText, int m, bool isMustMatch);
	//static int matchLineStart(string text, int iText, bool isMustMatch);
	//static int matchLineEnd(string text, int iText, bool isMustMatch);
	//static int matchMidBra(string text, int iText, bool isMustMatch);
	static bool matchChar(string* ans, string text, char ch, int iText);

	static int* hasMore(string text, int iText, int iRe);
};
#endif // !_REGULAR_EXP




