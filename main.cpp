#include <fstream>
#include "RegularExp.h"

int main(){
	ifstream in, out;
	in.open("regex.txt", ios::in);
	string s;
	int i = 0;
	while (getline(in, s)){
		++i;
		string suf;
		char t[256];
		sprintf_s(t, "%d", i);
		suf = t;
		string file = "text";
		file += suf;
		file += ".txt";
		ifstream infile(file, ios::in);
		string text;
		getline(infile, text);
		s = "((\\d+)(\\s+)(\\d+))\\s+(\\4\\2)";
		text = "abc23  456 45623cdfad23 dfe";
		//s = "\\b\\w";
		//text = " abc1d";
		string ans = RegularExp::get(s, text);
		cout << ans << endl;
		return 0;
	}
}

// .	任意字符
// \w	字母或数字
// \s	空白符,换行符
// \d	数字
// \b	单词开始或结束
// ^	匹配行开始
// $	匹配行结束
// [x]	匹配x字符
// \W	不是字母或数字	
// \S	不是空白符
// \D	非数字
// \B	不是单词开始或结束
// [^x] 除x外任意字符
