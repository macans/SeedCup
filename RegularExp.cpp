
#include "RegularExp.h"
int RegularExp::iBra = 0;
string RegularExp::re = null;
bool RegularExp::isEnd[105] = { false };
bool RegularExp::isNull = false;
bool RegularExp::isFirstCmd = false;
bool RegularExp::isFinish = false;
int RegularExp::nextBra[1025] = { 0};
int RegularExp::nextOr[1025] = { 0 };
string RegularExp::brakets[102] = { null };

RegularExp::RegularExp(){

}
RegularExp::~RegularExp(){

}
//主过程，递归调用
//work(text, iRe, iNextOr, iCmd)
RESULT RegularExp::work(string text, int iRe, int iNextBra, int iNextOr, int iCmd)
{
	RESULT result, strp, str;
	result.ans = null;
	result.isMatch = false;
	if (iRe >= re.length()){
		isFinish = true;
		result.ans = null;
		result.isMatch = true;
		return result;
	}
	int  r_iRe, r_iText, left, right, end_right, i, j;
	int lenText = text.length(), iText = 0, iReNext;
	bool flag = false, isMustMatch = false;
	int ever_iEnd;
	int* side;
	char ch = re[iRe];

	if (ch == '\\') {
		++iRe;
		flag = true;
		ch = re[iRe];
	}
	if (iRe == iNextOr && iRe != 0){
		iNextOr = nextOr[iRe] + 1;
		if (iNextOr == 1) iNextOr = iNextBra + 1;
		result = work(text, iNextOr, iNextBra, iNextOr, iCmd + 1);
		return result;
	}
	if (iRe == iNextBra){
		for (i = iBra; i > 0; i--){
			if (!isEnd[i]){
				isEnd[i] = true;
				ever_iEnd = i;
				break;
			}
		}

		side = hasMore(text, 0, iRe);
		if (side[0] == 1 && side[1] == 1){
			result = work(text, iRe + 1, nextBra[iRe + 1], nextOr[iRe + 1], iCmd + 1);
			isEnd[ever_iEnd] = false;
			return result;
		}
		else{
			left = side[0];
			right = side[1];
			iReNext = side[3];
			for (i = left; i <= right; i++){
				//if (iCmd == 1 && isFinish) return result;
				if (!matchBra(&str.ans, text, iText, ever_iEnd)) break;
				strp = work(text.substr(iText + brakets[ever_iEnd].length(), text.length() - iText - brakets[ever_iEnd].length() + 1),
					iReNext, nextBra[iReNext], nextOr[iReNext], iCmd + 1);
				if (!strp.isMatch) continue;
				if (str.ans.length() + strp.ans.length() > result.ans.length()){
					result.isMatch = true;
					result.ans = str.ans;
					result.ans.append(strp.ans);
				}
				iText = iText + brakets[ever_iEnd].length();
			}
			return result;
		}
	}
	if (ch == '('){
		++iBra;
		if (iCmd != 1) ++iCmd;
		str = work(text, iRe + 1, nextBra[iRe], nextOr[iRe], iCmd);
		--iBra; 
		if (!str.isMatch) result.isMatch = false;
		if (str.ans > result.ans){
			result = str;
		}
		return result;
	}
	if (iCmd == 1) {
		end_right = lenText;
	}
	else{
		end_right = 1;
	}
	if (!flag && ch!= '$' && ch != '^' && ch != '['){
		side = hasMore(text, 0, iRe);
		if (side[0] == 1 && 1 == side[1]){
			for (i = 0; i < end_right; i++){
				if (iCmd == 1 && isFinish) return result;
				str = result;
				//if (iCmd == 1) isFirstCmd = true;
				if (!matchChar(&str.ans, text, ch, i, 1)) continue;
				strp= work(text.substr(i + 1, lenText - i), iRe + 1, iNextBra, iNextOr, iCmd + 1);
				if (!strp.isMatch) continue;
				result.isMatch = true;
				//if (iCmd == 1) isFirstCmd = false;
				//if (strp.length() == 0) continue;
				str.ans.append(strp.ans);
				if (str.ans.length() > result.ans.length()){
					result.ans= str.ans;
				}
			}
			brakets[iBra] = result.ans;
		}
		else{
			left = side[0];
			right = side[1];
			iReNext = side[3];
			for (i = left; i <= right; i++){
				for (j = 0; j < end_right; j++){
					if (iCmd == 1 && isFinish) return result;
					str.ans = null;
					//if (iCmd == 1) isFirstCmd = true;
					if (!matchChar(&str.ans, text, ch, j, i)) continue;
					strp = work(text.substr(i + j), iReNext, iNextBra, iNextBra, iCmd + 1);
					if (!strp.isMatch) continue;
					result.isMatch = true;
					//if (iCmd == 1) isFirstCmd = false;
					str.ans.append(strp.ans);
					if (str.ans.length() > result.ans.length()){
						result = str;
					}
				}
			}
		}
		
		return result;
	}
	if (ch == '['){
		int* midBra = matchMidBra(iRe);
		iRe = nextBra[iRe];
		side = hasMore(text, 0, iRe);
		left = side[0];
		right = side[1];
		iReNext = side[3];
		bool canMatch = true;
		for (i = left; i <= right; i++){
			for (j = 0; j < i; j++){
				for (int k = 2; k < midBra[0] + 2; k++){
					if (midBra[k] != text[j]) continue;
					flag = true;
					break;
				}
				if (!flag){
					canMatch = false;
					break;
				}
			}
			if (!canMatch) break;
			str.ans = text.substr(0, i);
			strp = work(text.substr(i + 1), iReNext, iNextBra, iNextOr, iCmd + 1);
			if (!strp.isMatch) continue;
			str.ans.append(strp.ans);
			if (str.ans.length() > result.ans.length()){
				result.isMatch = true;
				result.ans = str.ans;
			}
		}
		return result;
	}
	//处理单词的开头和结尾，行开头
	if (ch == 'b' || ch == 'B' || ch == '^'|| ch == '$'){
		switch (ch)
		{
		case 'b':
			for (i = 0; i < end_right; i++){
				if (iCmd == 1 && isFinish) return result;
				if (!matchSide(text, iText + i, true, true)) continue;
				str = work(text.substr(i, lenText - i + 1), iRe + 1, iNextBra, iNextOr, iCmd + 1);
				if (!str.isMatch) continue;
				result.isMatch = true;
				if (str.ans.length() > result.ans.length() ){
					result =str;
				}
			}
			break;
		case 'B':
			for (i = 0; i < end_right; i++){
				if (iCmd == 1 && isFinish) return result;
				iText = matchSide(text, iText + i, true, false);
				if (iText == -1) continue;
				str = work(text.substr(i, lenText - i + 1), iRe + 1, iNextBra, iNextOr, iCmd + 1);
				if (!str.isMatch) continue;
				result.isMatch = true;
				if (str.ans.length() > result.ans.length()){
					result = str;
				}
			}
			break;
		case '^':
			if (iCmd != 1){
				return result;
			}
			str = work(text,  iRe + 1, iNextBra, iNextOr, iCmd + 1);
			if (!str.isMatch) break;
			result.isMatch = true;
			if (str.ans.length() > result.ans.length()){
				result = str;
			}
			break;
		case '$':
			if (lenText != 0){
				result.isMatch = false;
				break;
			}
			result = work(null, iRe + 1, iNextBra, iNextOr, iCmd + 1);
			break;
		}
		return result;
	}

	if (ch >= '0' && ch <= '9'){
		int m = ch - 48;
		++iRe;
		while (re[iRe] >= '0' && re[iRe] <= '9'){
			m = m * 10 + re[iRe] - 48;
			++iRe;
		}
		--iRe;
		side = hasMore(text, 0, iRe);
		if (side[0] == 1 && side[1] == 1){
			if (matchBra(&str.ans, text, 0, m + 1)){
				strp = work(text.substr(brakets[m + 1].length(), text.length() - brakets[m + 1].length() + 1), iRe + 1, iNextBra, iNextOr, iCmd + 1);
				if (strp.isMatch){
					result.isMatch = true;
					str.ans.append(strp.ans);
					result.ans = str.ans;
					result.isMatch = true;
				}
				else{
					result.isMatch = false;
				}

			}
			else{
				result.isMatch = false;
			}
			return result;
		}
		else{
			left = side[0];
			right = side[1];
			iReNext = side[3];
			for (i = left; i <= right; i++){
				if (!matchBra(&str.ans, text, iText, m + 1)) break;
				strp = work(text.substr(iText + brakets[m + 1].length(), text.length() - iText - brakets[m + 1].length() + 1),
					iReNext, iNextBra, iNextOr, iCmd + 1);
				if (!strp.isMatch) continue;
				if (strp.ans.length() + str.ans.length() > result.ans.length()){
					result.ans = str.ans;
					result.ans.append(strp.ans);
					result.isMatch = true;
				}
				iText += brakets[m + 1].length();
			}
			return result;
		}
	}
		
	side = hasMore(text, 0, iRe);
	isMustMatch = side[2];
	if (side[0] == 1 && 1 == side[1]){
		for (i = 0; i < end_right; i++){
			if (iCmd == 1 && isFinish) return result;
			str.ans = null;
			if (iCmd == 1) isFirstCmd = true;
			if (!match(ch, &str.ans, text, i, 1, isMustMatch)) continue;
			for (int k = 0; k <= iBra; k++){
				if (isEnd[k]) continue;
				brakets[iBra] += str.ans;
			}
			strp = work(text.substr(i + 1, lenText - i), iRe + 1,iNextBra, iNextOr, iCmd + 1);
			if (!strp.isMatch) continue;
			result.isMatch = true;
			if (iCmd == 1) isFirstCmd = false;
			//if (str.length() == 0) continue;
			str.ans.append(strp.ans);
			if (str.ans.length() > result.ans.length()){
				result = str;
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
				if (iCmd == 1 && isFinish) return result;
				str.ans = null;
				//if (iCmd == 1) isFirstCmd = true;
				if (!match(ch, &str.ans, text, j, i, isMustMatch)) continue;
				int ever_iBra = iBra;
				for (int k = 1; k <= iBra; k++){
					if (isEnd[k]) continue;
					brakets[k] += str.ans;
				}
				strp = work(text.substr(j + i, lenText - j), iReNext, iNextBra, iNextOr, iCmd + 1);
				for (int k = 1; k <= ever_iBra; k++){
					if (isEnd[k]) continue;
					brakets[k] = brakets[k].substr(0, brakets[k].length() - str.ans.length());
				}
				if (!strp.isMatch) continue;
				result.isMatch = true;
				//if (iCmd == 1) isFirstCmd = false;
				//if (strp.length() == 0 && iReNext < re.length()) continue;
				str.ans.append(strp.ans);
				if ( str.ans.length() > result.ans.length()){
					result = str;
				}
			}
		}
	}
	return result;
}

//处理括号
void RegularExp::dealBra(){
	//right[i]表示i右括号的位置，如果i位置不是左括号，则right[i] = -1;
	int stackBra[512];
	int stackOr[512] = { 0 };
	int s_n = 0;
	int o_n = 0;
	for (int i = 0; i < re.length(); i++){
		switch (re[i]){
		case '(':
			stackBra[s_n] = i;
			stackOr[o_n] = i;
			s_n++;
			o_n++;
			break;
		case '[':
			stackBra[s_n] = i;
			s_n++;
			break;
		case '|':
			stackOr[o_n] = i;
			o_n++;
			break;
		case ']':
			nextBra[stackBra[s_n - 1]] = i;
			nextBra[i] = i;
			s_n--;
			break;
		case ')':
			nextBra[stackBra[s_n - 1]] = i;
			nextBra[i] = i;
			stackOr[o_n] = i;
			o_n++;
			s_n--;
			break;
		}

	}
	for (int i = 0; stackOr[i] != 0 || i == 0; i++){
		if (re[stackOr[i]] == '(' && re[stackOr[i + 1]] == '|'){
			nextOr[i] = stackOr[i + 1];
		}
		else{
			if (re[stackOr[i]] == '|' && re[stackOr[i + 1]] == '|')
				nextOr[i] = stackOr[i + 1];
		}

	}
}

string RegularExp::get(string s, string text)
{	
	re = "(" + s + ")";
 	dealBra();
	RESULT result = work(text, 0, nextBra[0], nextOr[0], 0);
	if (result.isMatch){
		return result.ans;
	}
	return null;
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

bool RegularExp::matchNum(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum)
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

bool RegularExp::matchWord(string* ans, string text, int iText, int m, bool isMustMatch, bool isWord)
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

bool RegularExp::matchSpace(string* ans, string text, int iText, int m, bool isMustMatch, bool isSpace)
{
	string str;
	bool flag = false;
	for (int i = iText; i < iText + m; i++){
		if ((text[i] == ' ') == (isSpace)) continue;
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

bool RegularExp::matchChar(string* ans, string text, char ch, int iText, int m)
{
	int i;
	bool flag = false;
	if (ch == '.'){
		for (i = 0; i < m; i++){
			ans->push_back(text[i]);
		}
		return true;
	}
	for (i = iText; i < iText + m; i++){
		if (text[i] == ch) continue;
		flag = true;
		break;
	}
	if (!flag){
		for (i = 0; i < m; i++){
			ans->push_back(ch);
		}
		return true;
	}
	
	return false;
}

bool RegularExp::match(char ch, string* ans, string text, int iText, int m, bool isMustMatch)
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
	case 'S':
		return matchSpace(ans, text, iText, m, isMustMatch, false);
		break;
	case 'W':
		return matchWord(ans, text, iText, m, isMustMatch, false);
		break;
	case 'D':
		return matchNum(ans, text, iText, m, isMustMatch, false);
		break;
	}
}

bool RegularExp::matchBra(string* ans, string text,int iText, int m){
	int i;
	bool flag = false;
	int len = brakets[m].length();
	if (text.length() < len) return false;
	for (i = 0; i < len; i++){
		if (brakets[m][i] == text[i + iText]) continue;
		flag = true;
		break;
	}
	if (!flag){
		ans->append(brakets[m]);
		return true;
	}
	return false;
}

bool RegularExp::matchSide(string text, int iText, bool isMustMatch, bool isSide)
{
	if (((text[iText] >= '0' && text[iText] <= '9') || (text[iText] >= 'a' && text[iText] <= 'z') || (text[iText] >= 'A' && text[iText] <= 'Z')) == isSide) {
		--iText;
		if (!((text[iText] >= '0' && text[iText] <= '9') || (text[iText] >= 'a' && text[iText] <= 'z') || (text[iText] >= 'A' && text[iText] <= 'Z'))){
			return true;
		}
		iText += 2;
		if (!((text[iText] >= '0' && text[iText] <= '9') || (text[iText] >= 'a' && text[iText] <= 'z') || (text[iText] >= 'A' && text[iText] <= 'Z'))){
			return true;
		}
	}
	return false;
}

int* RegularExp::matchMidBra(int iRe)
{
	int a[1025];
	int k = 2;
	int ch;
	if (re[iRe + 1] == '^')
	{
		a[1] = 1;
	}
	else a[1] = 0;
	for (int i = iRe + 1; i < nextBra[iRe]; i++)
	{
		if (re[i] != '^'&&re[i] != '-')
		{
			ch = re[i];
			a[k] = ch;
			k++;
		}
		else if (re[i] == '-')
		{
			while (ch++ != re[i + 1])
			{
				a[k] = ch;
				k++;
			}
		}
	}
	a[0] = k - 2;
	return a;
}




