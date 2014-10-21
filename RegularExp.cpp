
#include "RegularExp.h"

string RegularExp::re = null;
bool RegularExp::isNull = false;
bool RegularExp::isFirstCmd = false;

RegularExp::RegularExp(){

}
RegularExp::~RegularExp(){

}
//主过程，递归调用
string RegularExp::work(string text, int iRe, int iCmd)
{
	if (iRe == re.length()) return null;
	string ans = null;
	string str = null;
	string strp = null;
	int maxLen, r_iRe, r_iText, left, right, end_right, i, j;
	int lenText = text.length(), iText, iReNext;
	bool flag = false, isMustMatch = false;
	int* side;
	
	char ch = re[iRe];
	if (ch == '\\') {
		++iRe;
		flag = true;
		ch = re[iRe];
	}
	/*if (ch == '('){
		work(iRe, next[iRe]
		}*/
	if (iCmd == 1) {
		end_right = lenText;
	}
	else{
		end_right = 1;
	}
	if (!flag) {
		for (i = 0; i < end_right; i++){
			str = null;
			if (iCmd == 1) isFirstCmd = true;
			if (!matchChar(&str, text, ch, i)) continue;
			strp = work(text.substr(i + 1, lenText - i), iRe + 1, iCmd + 1);
			if (iCmd == 1) isFirstCmd = false;
			if (strp.length() == 0) continue;
			str.append(strp);
			if (str.length() > ans.length()){
				ans = str;
			}
		}
		return ans;
	}
	side = hasMore(text, 0, iRe);
	isMustMatch = side[2];
	if (side[0] == 1 && 1 == side[1]){
		for (i = 0; i < end_right; i++){
			str = null;
			if (iCmd == 1) isFirstCmd = true;
			if (!match(ch, &str, text, i, 1, isMustMatch)) continue;
			strp = work(text.substr(i + 1, lenText - i), iRe + 1 , iCmd + 1);
			if (iCmd == 1) isFirstCmd = false;
			if (strp.length() == 0) continue;
			str.append(strp);
			if (str.length() > ans.length()){
				ans = str;
			}
		}
	}
	else{
		left = side[0];
		right = side[1];
		isMustMatch = side[2];
		iReNext = side[3];
		for (i = left; i <= right; i++){
			for (j = 0; j < end_right; j++){
				str = null;
				if (iCmd == 1) isFirstCmd = true;
				if (!match(ch, &str, text, j, i, isMustMatch)) continue;
				strp = work(text.substr(j + i, lenText - j), iReNext, iCmd + 1);
				if (iCmd == 1) isFirstCmd = false;
				if (strp.length() == 0 && iReNext < re.length()) continue;
				str.append(strp);
				if (str.length() > ans.length()){
					ans = str;
				}
			}
		}
		++iRe;
	}
	return ans;
}

//处理括号
void RegularExp::dealBra(){
	//right[i]表示i右括号的位置，如果i位置不是左括号，则right[i] = -1;
}

string RegularExp::get(string s, string text)
{	
 	dealBra();
	re = s;
	return work(text, 0, 1);
}

int* RegularExp::hasMore(string text, int iText, int iRe)
{
	int a[4];
	int i;
	int n;
	iRe++;
	a[0] = 0;
	a[1] = text.length() - iText + 1;
	a[2] = false;
	char ch = re[iRe];
	switch (ch){
	case '*':
		a[0] = 0;
		++iRe;
		break;
	case '+':
		a[0] = 1;
		++iRe;
		break;
	case '?':
		++iRe;
		a[0] = 0;
		a[1] = 1;
		break;
	case '{':
		for (i = 0; re[iRe + i] != ',' && re[iRe + i] != '}'; i++);
		int tem[10];
		for (int j = 0; j < i - 1; j++){
			a[0] += (re[iRe + i - 1 - j] - 48) * pow(10, j);
		}
		if (re[iRe + i] == '}'){
			iRe += i + 1;
			a[1] = a[0];
			a[2] = true;
		}
		else{
			if (re[iRe + i + 1] == '}')
				iRe += i + 2;
			else{
				a[1] = 0;
				for (n = 0; re[iRe + i + n] != '}'; n++);
				for (int j = 0; j < n - 1; j++){
					a[1] += (re[iRe + i + n - 1 - j] - 48)* pow(10, j);
				}
				iRe += i + n + 1;
			}
		}
		break;
	default:
		a[0] = 1;
		a[1] = 1;
		a[2] = true;
		break;
	}
	a[3] = iRe;
	return a;
}

int RegularExp::matchNum(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum)
{
	string str;
	bool flag = false;
	for (int i = iText; i < iText + m; i++){
		if ((text[i] >= '0' && text[i] <= '9') == (isNum)) continue;
		flag = true;
		break;
	}
	if (!flag){
		str = text.substr(iText, m);
		ans->append(str);
		return true;
	}
	return false;
}

int RegularExp::matchWord(string* ans, string text, int iText, int m, bool isMustMatch, bool isWord)
{
	
	string str;
	bool flag = false;
	for (int i = iText; i < iText + m; i++){
		if (((text[i] >= '0' && text[i] <= '9') || (text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) == isWord) continue;
		flag = true;
		break;
	}
	if (!flag){
		str = text.substr(iText, m);
		ans->append(str);
		return true;
	}
	return false;
}

int RegularExp::matchSpace(string* ans, string text, int iText, int m, bool isMustMatch, bool isSpace)
{
	string str;
	if (0 == m){
		return iText;
	}
	bool flag = false;
	while (iText < text.length() - m + 1){
		for (int i = iText; i < iText + m; i++){
			if (text[i] == ' ') continue;
			flag = true;
			break;
		}
		if (!flag){
			str = text.substr(iText, m);
			ans->append(str);
			return ++iText;
		}
		flag = false;
		++iText;
		if (isMustMatch){
			RegularExp::isNull = true;
		}
		return -1;
	}
}

bool RegularExp::matchChar(string* ans, string text,  char ch, int iText)
{
	if (text[iText] == ch){
		ans->push_back(ch);
		return true;
	}
	return false;
}

int RegularExp::match(char ch, string* ans, string text, int iText, int m, bool isMustMatch)
{
	switch (ch)
	{
	case 'd':
		return matchNum(ans, text, iText, m, isMustMatch, true);
		break;
	case 'w':
		return matchWord(ans, text, iText, m, isMustMatch, true);
		break;
	case 's':
		return matchSpace(ans, text, iText, m, isMustMatch, true);
		break;
		/*case 'b':
			return matchSide(ans, text, iText, 1, isMustMatch, true);
			break;
			case '.':
			return matchAny(ans, text, iText, 1, isMustMatch, true);
			break;

			case 'W':
			return matchWord(ans, text, iText, 1, isMustMatch, false);
			break;
			case 'S':
			return matchSpace(ans, text, iText, 1, isMustMatch, false);
			break;
			case 'B':
			return matchSide(ans, text, iText, 1, isMustMatch, true);
			break;
			case 'D':
			return matchSide(ans, text, iText, 1, isMustMatch, true);
			break;
			case '$':
			return matchSide(ans, text, iText, 1, isMustMatch, true);
			break;
			case '[':
			return matchSide(ans, text, iText, 1, isMustMatch, true);
			break;*/
	default:
		break;
	}
}




