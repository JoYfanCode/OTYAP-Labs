#define _CRT_SECURE_NO_WARNINGS // очень важная строчка!
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
/*
Состояния автомата
*/
enum AState { S, A, E, F };
/*
Контейнер лексемы
*/
struct Lex
{
	bool valid;
	char* str;
};
/*
Включает ли в себя слово этот символ
*/
bool HasWordChar(char* word, int length, char c)
{
	for (int i = 0; i < length; i++)
		if (word[i] == c)
				return true;

	return false;
}
/*
Функция лексического анализа
*/
vector<Lex> LexAnalysis(char* str)
{
	vector<Lex> result;
	int position = 0; // текущая позиция в строке
	AState state = AState::S; // текущее состояние
	Lex lexema; // текущая лексема
	int firstPos = 0; // позиция начала лексемы
	char* word = new char[100];

	while (str[position] != '\0')
	{
		char currentChar = str[position];
		// Инициализация лексемы при обнаружении непробельного символа
		if (state == AState::S && currentChar != ' ')
		{
			firstPos = position;
			lexema.valid = true;
			word = new char[100];
		}

		// переход по матрице состояний
		if (currentChar == ' ') {
			if (state == AState::S) state = AState::F;
			if (state == AState::A) state = AState::F;
			if (state == AState::E) state = AState::F;
		}
		else if (HasWordChar(word, position - firstPos + 1, currentChar)) {
			if (state == AState::S) state = AState::E;
			if (state == AState::A) state = AState::E;
			if (state == AState::E) state = AState::E;
		}
		else if (HasWordChar(word, position - firstPos + 1, currentChar) == false) {
			if (state == AState::S) state = AState::A;
			if (state == AState::A) state = AState::A;
			if (state == AState::E) state = AState::E;
		}

		// Определение соответствия лексемы заданию
		if (state == AState::E)
			lexema.valid = false;
		// Запись текущей лексемы в выходной список и инициализация новой лексемы
		if (state == AState::F)
		{
			int length = position - firstPos;
			lexema.str = new char[length + 1];
			// Вычленение подстроки и запись в лексему
			strncpy(&lexema.str[0], &str[0] + firstPos, length);
			// Постановка финализирующего 0
			lexema.str[length] = '\0';
			// Запись лексемы в список
			result.push_back(lexema);
			state = AState::S;
		}

		word[position - firstPos] = str[position++];
	}
	return result;
}
int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	char text[100];
	in.getline(text, 100, '\0');
	vector<Lex> result = LexAnalysis(text);

	for (int i = 0; i < (int)result.size(); i++) {
		if (result[i].valid)
			out << result[i].str << " ";
	}

	in.close();
	out.close();

	return 0;
}