
#include "RegularExp.h"

int RegularExp::iRe = 0;
bool RegularExp::isNull = false;
string RegularExp::ans = "";

RegularExp::RegularExp(){

}
RegularExp::~RegularExp(){

}
//�����̣��ݹ����
string RegularExp::work(string re, string text, int iText){
	string ans = "";
	string str = "";
	string strp = "";
	int lenRe = re.length();
	int lenText = text.length();
	bool flag = false, isMustMatch = false;
	int* side;
	while (iRe < lenRe){
		char ch = re[iRe];
		switch (ch){
		case 'd':
			//if (!flag) matchChar(text, ch, true);
			flag = false;
			side = hasMore(re, text, iText);
			isMustMatch = side[2];

			if (side[0] == 1 && 1 == side[1]){
				iText = matchNum(&ans, text, iText, 1, isMustMatch, true);
				if (iText == -1) return "";
			}
			else{
				int maxLen = 0;
				int t = iRe;
				int lastiText = iText;
				int left = side[0];
				int right = side[1];
				strp = ans;
				for (int i = left; i <= right; i++){
					str = strp;
					iText = matchNum(&str, text, lastiText, i, isMustMatch, true);
					if (iText == -1) continue;
					str.append(work(re, text.substr(iText, text.length() - iText + 1), 0));
					iRe = t;
					if (str.length() > maxLen ){
						ans = str;
						maxLen = str.length();
					}
					
				}
				if (ans.length() == strp.length()) return "";
				return ans;
			}
			break;
		 //case 'w':
			////if (!flag) matchChar(text, ch, true);
			//matchWord(text, iText, 1, true);
			//break;
		/*case 's':
			if (!flag) matchChar(text, ch, true);
			matchSpace(text, iText, 1, true);
			break;
		case 'b':
			if (!flag) matchChar(text, ch, true);
			matchSide(text, iText, 1, true);
			break;
		case '.':
			if (!flag) matchChar(text, ch, true);
			matchAny(text, iText, 1, true);
			break;
		case '^':
			matchLineStart(text, iText, true);
			break;
		case 'W':
			if (!flag) matchChar(text, ch, true);
			matchNotWord(text, iText, 1, true);
			break;
		case 'S':
			if (!flag) matchChar(text, ch, true);
			if (!flag) matchChar(text, ch, true);
			matchNotSpace(text, iText, 1, true);
			break;
		case 'B':
			if (!flag) matchChar(text, ch, true);
			matchNotSide(text, iText, 1, true);
			break;
		case 'D':
			if (!flag) matchChar(text, ch, true);
			matchNotNum(text, iText, 1, true);
			break;
		case '$':
			matchLineEnd(text, iText, true);
			break;
		case '[':
			matchMidBra(text, iText, true);
			break;*/
		case '\\':
			flag = true;
			break;
		/*default:
			matchChar(text, iText, true);
			break;*/
		}
		++iRe;
	}
	return ans;
}

//��������
void RegularExp::dealBra(){
	//right[i]��ʾi�����ŵ�λ�ã����iλ�ò��������ţ���right[i] = -1;
}



string RegularExp::get(string re, string text)
{	
 	dealBra();
	return work(re, text, 0);
}

int RegularExp::matchNum(string* ans, string text, int iText, int m, bool isMustMatch, bool isNum)
{
	string str;
	if (0 == m){
		return iText;
	}
	bool flag = false;
	int last = text.length() - m + 1;
	while (iText < last){
		
		for (int i = iText; i < iText + m; i++){
			if (text[i] >= '0' && text[i] <= '9' && isNum) continue;
			flag = true;
			break;
		}
		if (!flag){
			str = text.substr(iText, m);
			ans->append(str);
			return iText + m;
		}
		flag = false;
		++iText;
	}
	if (isMustMatch){
		RegularExp::isNull = true;
	}
	return -1;	
}

int* RegularExp::hasMore(string re, string text, int iText)
{
		int a[3];
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
		return a;
}

int RegularExp::matchWord(string* ans, string text, int iText, int m, bool isMustMatch)
{
	string str;
	if (0 == m){
		return iText;
	}
	bool flag = false; 
	while (iText < text.length() - m + 1){
		for (int i = iText; i < iText + m; i++){
			if ((text[i] >= '0' && text[i] <= '9') || (text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) continue;
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
	}
	if (isMustMatch){
		RegularExp::isNull = true;
	}
	return -1;
}

int RegularExp::matchSpace(string* ans, string text, int iText, int m, bool isMustMatch)
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

int RegularExp::matchNotNum(string* ans, string text, int iText, int m, bool isMustMatch)
{
	string str;
	if (0 == m){
		return iText;
	}
	bool flag = false;
	while (iText < text.length() - m + 1){
		for (int i = iText; i < iText + m; i++){
			if (!(text[i] >= '0' && text[i] <= '9')) continue;
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
	}
	if (isMustMatch){
		RegularExp::isNull = true;
	}
	return -1;
}




