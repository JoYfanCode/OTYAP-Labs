#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;
/*
Состояния автомата
*/
enum AState 
{ 
	S, 
	A_word, F_word, 
	A_const, F_const, 
	A_arithmetic, F_arithmetic,
	A_equal, F_equal,
	A_compare, B_compare, F_compare,
	A_wrong, F_wrong
};

enum Types { kw, id, eq, vl, ao, wl, co };
/*
Контейнер лексемы
*/
struct Lex
{
	Types type;
	char* str;
};


bool isKeyword(const char* const str)
{
	if (!strcmp(str, "for"))
		return true;
	if (!strcmp(str, "to"))
		return true;
	if (!strcmp(str, "next"))
		return true;

	return false;
}

bool isArithmetic(char c)
{
	if (c == '+' || c == '-' || c == '/' || c == '*')
		return true;
	else
		return false;
}

bool isIndentation(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return true;
	else
		return false;
}
/*
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
	bool next = true;

	while (str[position] != '\0')
	{
		
		// Инициализация лексемы при обнаружении непробельного символа
		if (state == AState::S)
		{
			firstPos = position;
			word = new char[100];
			next = true;
		}

		char currentChar = str[position];
		word[position - firstPos] = str[position];

		// Проход по автомату

		// Start
		if (state == AState::S) {
			if (isdigit(currentChar))
				state = AState::A_const;
			else if (isalpha(currentChar))
				state = AState::A_word;
			else if (isArithmetic(currentChar))
				state = AState::A_arithmetic;
			else if (currentChar == '=')
				state = AState::A_equal;
			else if (currentChar == '>' || currentChar == '<')
				state = AState::A_compare;
			else
				state = AState::A_wrong;
		}

		// Words
		else if (state == AState::A_word) {
			if (isalnum(currentChar)) {
				state = AState::A_word;
			}
			else if (isIndentation(currentChar)) {
				state = AState::F_word;
			}
			else if (isArithmetic(currentChar)) {
				next = false;
				state = AState::F_word;
			}
			else if (currentChar == '=') {
				next = false;
				state = AState::F_word;
			}
			else if (currentChar == '>' || currentChar == '<'){
				next = false;
				state = AState::F_word;
			}
			else {
				state = AState::A_wrong;
			}
		}
		// Consts
		else if (state == AState::A_const) {
			if (isdigit(currentChar)) {
				state = AState::A_const;
			}
			else if (isIndentation(currentChar)) {
				lexema.type = Types::vl;
				state = AState::F_const;
			}
			else if (isArithmetic(currentChar)) {
				next = false;
				lexema.type = Types::vl;
				state = AState::F_const;
			}
			else if (currentChar == '=') {
				next = false;
				lexema.type = Types::vl;
				state = AState::F_const;
			}
			else if (currentChar == '>' || currentChar == '<'){
				next = false;
				lexema.type = Types::vl;
				state = AState::F_const;
			}
			else {
				state = AState::A_wrong;
			}
		}
		// Arithmetic
		else if (state == AState::A_arithmetic) {
			if (isIndentation(currentChar)) {
				lexema.type = Types::ao;
				state = AState::F_arithmetic;
			}
			else if (isalnum(currentChar)) {
				next = false;
				lexema.type = Types::ao;
				state = AState::F_arithmetic;
			}
			else {
				state = AState::A_wrong;
			}
		}
		// Equal
		else if (state == AState::A_equal) {
			if (isIndentation(currentChar)) {
				lexema.type = Types::eq;
				state = AState::F_equal;
			}
			else if (isalnum(currentChar)) {
				next = false;
				lexema.type = Types::eq;
				state = AState::F_equal;
			}
			else {
				state = AState::A_wrong;
			}
		}
		// Compare
		else if (state == AState::A_compare) {
			if (currentChar == '<' || currentChar == '>' || currentChar == '=') {
				state == AState::B_compare;
			}
			else if (isIndentation(currentChar)) {
				lexema.type = Types::co;
				state = AState::F_compare;
			}
			else if (isalnum(currentChar)) {
				next = false;
				lexema.type = Types::co;
				state = AState::F_compare;
			}
			else {
				state = AState::A_wrong;
			}
		}
		else if (state == AState::B_compare) {
			if (isIndentation(currentChar)) {
				lexema.type = Types::co;
				state = AState::F_compare;
			}
			else if (isalnum(currentChar)) {
				next = false;
				lexema.type = Types::co;
				state = AState::F_compare;
			}
			else {
				state = AState::A_wrong;
			}
		}
		// Wrongs
		else if (state == AState::A_wrong)
		{
			if (isIndentation(currentChar)) {
				lexema.type = Types::wl;
				state = AState::F_wrong;
			}
		}

		// Запись текущей лексемы в выходной список и инициализация новой лексемы
		if (state == AState::F_word || state == AState::F_const || state == AState::F_arithmetic || state == AState::F_equal || state == AState::F_compare || state == AState::F_wrong)
		{
			int length = position - firstPos;

			lexema.str = new char[length + 1];
			// Вычленение подстроки и запись в лексему
			strncpy(&lexema.str[0], &str[0] + firstPos, length);
			// Постановка финализирующего 0
			lexema.str[length] = '\0';

			if (state == AState::F_word) {
				if (isKeyword(lexema.str)) lexema.type = Types::kw;
				else lexema.type = Types::id;
			}
			// Запись лексемы в список
			result.push_back(lexema);

			state = AState::S;
		}

		if (next) position++;
	}

	return result;
}
int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	char text[100];
	in.getline(text, 100, '\0');

	int length = 0;
	for (int i = 0; text[i] != '\0'; i++)
		length++;

	text[length] = ' ';
	text[length + 1] = '\0';

	vector<Lex> result = LexAnalysis(text);

	for (int i = 0; i < (int)result.size(); i++) {
		out << result[i].str;

		if (result[i].type == Types::kw)
			out << "[kw] ";
		if (result[i].type == Types::id)
			out << "[id] ";
		if (result[i].type == Types::vl)
			out << "[vl] ";
		if (result[i].type == Types::ao)
			out << "[ao] ";
		if (result[i].type == Types::eq)
			out << "[eq] ";
		if (result[i].type == Types::co)
			out << "[co] ";
		if (result[i].type == Types::wl)
			out << "[wl] ";
	}
	out << endl;
	for (int i = 0; i < (int)result.size(); i++) {
		if (result[i].type == Types::id)
			out << result[i].str << " ";
	}
	out << endl;
	for (int i = 0; i < (int)result.size(); i++) {
		if (result[i].type == Types::vl)
			out << result[i].str << " ";
	}

	in.close();
	out.close();

	return 0;
}