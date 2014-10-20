#ifndef _REGULAR_EXP
#define _REGULAR_EXP


#include <iostream>
#include <string>
using namespace std;

class RegularExp
{
private:
	static int iRe;
	static string ans;
	static bool isNull;
public:
	static string work(string re, string text, int iText);
	RegularExp();
	~RegularExp();
	static void dealBra();
	static string get(string re, string text);
	static int matchNum(string* ans, string text, int iText, int m, bool isMustMatch);
	/*static int matchWord(string text, int iText, int m, bool isMustMatch);
	static int matchSpace(string text, int iText, int m, bool isMustMatch);
	static int matchSide(string text, int iText, int m, bool isMustMatch);
	static int matchAny(string text, int iText, int m, bool isMustMatch);
	static int matchLineStart(string text, int iText, bool isMustMatch);
	static int matchNotWord(string text, int iText, int m, bool isMustMatch);
	static int matchNotSpace(string text, int iText, int m, bool isMustMatch);
	static int matchNotSide(string text, int iText, int m, bool isMustMatch);
	static int matchNotNum(string text, int iText, int m, bool isMustMatch);
	static int matchLineEnd(string text, int iText, bool isMustMatch);
	static int matchMidBra(string text, int iText, bool isMustMatch);*/
	//static int matchChar(string text, char ch, bool isMustMatch);

	static int* hasMore(string re, string text, int iText);
};
#endif // !_REGULAR_EXP




