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
		s = "\\d+\\w*";
		text = "abdc134t r12356r";
		string ans = RegularExp::get(s, text);
		cout << ans << endl;
		return 0;
	}
}

// .	�����ַ�
// \w	��ĸ������
// \s	�հ׷�,���з�
// \d	����
// \b	���ʿ�ʼ�����
// ^	ƥ���п�ʼ
// $	ƥ���н���
// [x]	ƥ��x�ַ�
// \W	������ĸ������	
// \S	���ǿհ׷�
// \D	������
// \B	���ǵ��ʿ�ʼ�����
// [^x] ��x�������ַ�
