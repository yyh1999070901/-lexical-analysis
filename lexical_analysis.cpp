#include<iostream>
#include<fstream>
#include<vector>
#include<set> 
using namespace std;
vector<string> words;//存储分词后结果
 
//关键字 
set<string> keyword {"auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "enum", "register", 
					 "typedef", "char", "extern", "return", "union", "const", "float", "short", "unsigned", "continue", 
					 "for", "signed", "void", "default", "goto", "sizeof", "volatile", "do", "if", "while", "static"};
//分隔符 
set<char> delimiter {'{', '}', ';', '(', ')', ',', '\'', '\"' ,':', '%', '#'};
//运算符
set<string> operator_set {"=", "+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!=", "++" , "+="};

bool checkBlank(char ch)//判断输入的字符是否为空白符、制表符、换行符 
{
	if(ch==' '||ch=='\t'||ch=='\n')
	{
		return true;
	}
	return false;
}

//判断是否是关键字
bool checkKeyword(string word) 
{
	if(keyword.count(word))
		return true;
	return false;
} 

//判断是否是分隔符 
bool checkDelimiter(string word) 
{
	char w = word[0];
	if(delimiter.count(w))
		return true;
	return false;
}

//判断是否是运算符
bool checkOperator(string word) 
{
	if(operator_set.count(word))
		return true;
	return false;
}

//判断是否是标识符
bool checkIdentifier(string word) 
{
	if(!(isalpha(word[0]) || word[0] == '_'))	// 第一个字符必须是字母或下划线
		return false;
	for(int i = 1; i < word.length(); i++) 
	{
		if(!(isdigit(word[i]) || isalpha(word[i]) || word[i] == '_'))
			return false;
	}
	return true;
} 

//判断是否为常数
bool checkConstant(string word) 
{
	int e_flag = 0, dot_flag = 0, sub_flag = 0; 
	if(!(isdigit(word[0]) || word[0] == '.')) 	//第一个字符只能是.或者数字
		return false;
	if(word[0] == '.')
		dot_flag = 1;
	for(int i = 1; i < word.length(); i++) 
	{
		if(word[i] == 'e' || word[i] == 'E') 
		{	
			//e后要是数字或者-, e前不能是. 
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
		{	//-号后要有数字，前要有e
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

//分割单词 
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
		else if(delimiter.count(str[i])) {	//分隔符 
			string temp = "";
			temp += str[i];
			words.push_back(temp);
			i++;
		}
		else{			//操作符 
			string temp = "";
			while((!isdigit(str[i]) && !isalpha(str[i]) && !delimiter.count(str[i])) && i < str.length()) {
				temp += str[i];
				i++;
			}
			words.push_back(temp);
		}
	} 
}

//写文件 
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
	string str="";//存储分割后的程序 
	while(!infile.eof())
	{
		char ch = infile.get();
		while(!checkBlank(ch)&&!infile.eof())//过滤掉空白符，制表符，换行符 
		{
			//过滤注释 
			if(ch=='/')
			{
				ch=infile.get();
				if(ch=='/')//过滤掉//型的注释
				{
					while(ch!='\n')
					{
						ch=infile.get();
					}
				}
				else if(ch=='*')//过滤掉/*型注释 
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
