#include<iostream>
#include<fstream>
#include<vector>
#include<set> 
using namespace std;
vector<string> words;//�洢�ִʺ���
 
//�ؼ��� 
set<string> keyword {"auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "enum", "register", 
					 "typedef", "char", "extern", "return", "union", "const", "float", "short", "unsigned", "continue", 
					 "for", "signed", "void", "default", "goto", "sizeof", "volatile", "do", "if", "while", "static"};
//�ָ��� 
set<char> delimiter {'{', '}', ';', '(', ')', ',', '\'', '\"' ,':', '%', '#'};
//�����
set<string> operator_set {"=", "+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!=", "++" , "+="};

bool checkBlank(char ch)//�ж�������ַ��Ƿ�Ϊ�հ׷����Ʊ�������з� 
{
	if(ch==' '||ch=='\t'||ch=='\n')
	{
		return true;
	}
	return false;
}

//�ж��Ƿ��ǹؼ���
bool checkKeyword(string word) 
{
	if(keyword.count(word))
		return true;
	return false;
} 

//�ж��Ƿ��Ƿָ��� 
bool checkDelimiter(string word) 
{
	char w = word[0];
	if(delimiter.count(w))
		return true;
	return false;
}

//�ж��Ƿ��������
bool checkOperator(string word) 
{
	if(operator_set.count(word))
		return true;
	return false;
}

//�ж��Ƿ��Ǳ�ʶ��
bool checkIdentifier(string word) 
{
	if(!(isalpha(word[0]) || word[0] == '_'))	// ��һ���ַ���������ĸ���»���
		return false;
	for(int i = 1; i < word.length(); i++) 
	{
		if(!(isdigit(word[i]) || isalpha(word[i]) || word[i] == '_'))
			return false;
	}
	return true;
} 

//�ж��Ƿ�Ϊ����
bool checkConstant(string word) 
{
	int e_flag = 0, dot_flag = 0, sub_flag = 0; 
	if(!(isdigit(word[0]) || word[0] == '.')) 	//��һ���ַ�ֻ����.��������
		return false;
	if(word[0] == '.')
		dot_flag = 1;
	for(int i = 1; i < word.length(); i++) 
	{
		if(word[i] == 'e' || word[i] == 'E') 
		{	
			//e��Ҫ�����ֻ���-, eǰ������. 
			if(e_flag)
				return false;
			e_flag = 1;
			if(!(word[i + 1] == '-' || isdigit(word[i + 1]))) 
			{
				return false;
			}
			if(word[i - 1] == '.')
				return false;
		}
		else if(word[i] == '-') 
		{	//-�ź�Ҫ�����֣�ǰҪ��e
			if(sub_flag)
				return false;
			sub_flag = 1; 
			if(!isdigit(word[i + 1])) 
				return false;	
			if(word[i - 1] != 'e' && word[i - 1] != 'E')
				return false;	
		}
		else if(!((isdigit(word[i]) || word[i] == '.')))
			return false;
		if(word[i] == '.') 
		{
			if(dot_flag)
				return false;
			dot_flag = 1;	
		}
	}
	return true;
}  

//�ָ�� 
void split_word(string str) {
	for(int i = 0; i < str.length(); ) {		
		if((isdigit(str[i]) || isalpha(str[i]) || str[i] == '.') && i < str.length()) {		
			string temp = "";
			while((isdigit(str[i]) || isalpha(str[i]) || str[i] == '.' || str[i] == '-') && i < str.length()) {
				temp += str[i];
				i++;
			} 
			words.push_back(temp);
		}
		else if(delimiter.count(str[i])) {	//�ָ��� 
			string temp = "";
			temp += str[i];
			words.push_back(temp);
			i++;
		}
		else{			//������ 
			string temp = "";
			while((!isdigit(str[i]) && !isalpha(str[i]) && !delimiter.count(str[i])) && i < str.length()) {
				temp += str[i];
				i++;
			}
			words.push_back(temp);
		}
	} 
}

//д�ļ� 
void writeFile(fstream &outfile) {
    for(int i = 0; i < words.size(); i++) {
    	if(checkKeyword(words[i]))		
    		outfile << "1, " << words[i] << "\n";
    	else if(checkIdentifier(words[i])) 
    		outfile << "2, " << words[i] << "\n";
		else if(checkConstant(words[i])) 
			outfile << "3, " << words[i] << "\n";
    	else if(checkOperator(words[i]))
    		outfile << "4, " << words[i] << "\n";
    	else if(checkDelimiter(words[i]))
    		outfile << "5, " << words[i] << "\n";
    	else
    		outfile <<"Error, " << words[i] << "\n";
	}
} 

int main()
{
	ifstream infile;
	fstream outfile;
	infile.open("./test.c",ios::in);
	outfile.open("./result(c++1).txt",ios::out);
	string str="";//�洢�ָ��ĳ��� 
	while(!infile.eof())
	{
		char ch = infile.get();
		while(!checkBlank(ch)&&!infile.eof())//���˵��հ׷����Ʊ�������з� 
		{
			//����ע�� 
			if(ch=='/')
			{
				ch=infile.get();
				if(ch=='/')//���˵�//�͵�ע��
				{
					while(ch!='\n')
					{
						ch=infile.get();
					}
				}
				else if(ch=='*')//���˵�/*��ע�� 
				{
					char pre=' ';
					while(true)
					{
						ch=infile.get();
						if(pre=='*'&&ch=='/')
						{
							break;
						}
						pre=ch;
					}
				} 
				break; 
			}
			else
			{
				str+=ch;
				ch=infile.get();
			}
		}
		if(str!="")
		{
			split_word(str);	
		} 
		str="";
	}
	writeFile(outfile);
    infile.close();   
	outfile.close();    
	return 0;	
} 
