#ifndef _REGULAR_EXP
#define _REGULAR_EXP
#define null  ""

#include <iostream>
#include <string>
#include <math.h>
using namespace std;

struct RESULT
{
	bool isMatch;
	string ans;
};
class RegularExp
{
private:
	static string re;
	static bool isNull;
	static int iBra;
	static bool isFirstCmd;
	static bool isFinish;
	static int nextBra[1025];
	static int nextOr[1025];
	static string brakets[102];
	static bool isEnd[105];
public:
	static RESULT work(string text, int iRe, int iNextBra, int iNextOr, int iCmd);
	RegularExp();
	~RegularExp();
	static void dealBra();
	static string get(string re, string text);
	static bool match(char ch, string* ans, string text, int iText, int m, bool isMustMatch);
	static bool matchNum(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum);
	static bool matchWord(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum);
	static bool matchSpace(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum);
	static bool matchSide(string text, int iText, bool isMustMatch, bool isSide);
	static bool matchBra(string*ans, string text, int iText, int m);
	static int* matchMidBra(int iRe);
	static bool matchChar(string* ans, string text, char ch, int iText, int m);
	static int* hasMore(string text, int iText, int iRe);
};
#endif // !_REGULAR_EXP




