#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool isCorrectWord(char* word, int length) // do word haven't the same letters
{
	for (int i = 0; i < length; i++)
		for (int j = i + 1; j < length; j++)
			if (word[i] == word[j])
				return false;

	return true;
}

vector<char*> processText(char* text)
{
	vector<char*> list;
	char* word = new char[100];
	int cur = 0;

	for (int i = 0; text[i]; i++)
	{
		if (text[i] == ' ' || text[i] == '\t' || text[i] == '\n') // end word
		{
			if (isCorrectWord(word, cur)) {
				word[cur] = '\0';
				list.push_back(word);
				word = new char[100];
			}
			cur = 0;
		}
		else // next letter of word
		{
			word[cur++] = text[i];
		}
	}

	if (cur > 0) 
	{  // check last word
		if (isCorrectWord(word, cur)) 
		{
			word[cur] = '\0';
			list.push_back(word);
			word = new char[100];
		}
	}

	return list;
}


int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	char text[100];
	in.getline(text, 100, '\0');

	vector<char*> list = processText(text);

	for (int i = 0; i < (int)list.size(); i++)
		out << list[i] << " ";

	in.close();
	out.close();

	return 0;
}