#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

// Типы лексем
enum ELexType { lWh, lDo, lEnd, lLo, lCo, lId, lVl, lAo, lEq };

// Строковые представления лексем
char TYPE_NAMES[9][3] = { "wh", "do", "en", "lo", "co", "id", "vl", "ao", "eq" };

// Вектор лексем
struct Lex {
	ELexType type;
	int index;
	int pos;
	Lex* next;
};

// Вектор лексем
Lex* P = NULL;
// Номер текущей лексемы
int CURPOS = 0;
// Вектор лексем, которые ожидались (для вывода ошибки)
Lex* EX = NULL;
// Файл для вывода
ofstream FOUT;

// Вывод ошибки
void Error() {
	FOUT << CURPOS << ' ';
	while (EX) {
		FOUT << TYPE_NAMES[EX->type] << ' ';
		EX = EX->next;
	}
}

// Вспомогательная функция сдвига указателя
void Next() {
	P = P->next;
	CURPOS++;
}

// Проверка операнда
bool Operand() {
	if (!P || (P->type != lId && P->type != lVl)) {
		EX = new Lex({ lId });
		EX->next = new Lex({ lVl });
		Error();
		return false;
	}
	Next();
	return true;
}

// Проверка арифметической операции
bool ArithExpr() {
	if (!Operand()) return false;
	while (P && P->type == lAo) {
		Next();
		if (!Operand()) return false;
	}
	return true;
}



// Проверка выражения сравнения
bool CompExpr() {
	if (!Operand()) return false;
	if (P && P->type == lCo) {
		Next();
		if (!Operand()) return false;
	}
	return true;
}

// Проверка условия
bool Condition() {
	if (!CompExpr()) return false;
	while (P && P->type == lLo) {
		Next();
		if (!CompExpr()) return false;
	}
	return true;
}

// Проверка арифметического выражения
bool Statement() {
	if (!P || P->type != lId) {
		EX = new Lex({ lId });
		Error();
		return false;
	}
	Next();
	if (!P || P->type != lEq) {
		EX = new Lex({ lEq });
		Error();
		return false;
	}
	Next();
	if (!ArithExpr()) return false;
	return true;
}

// Синтаксический анализ конструкции
bool WhileStatement() {
	if (!P || P->type != lWh) {
		EX = new Lex({ lWh });
		Error();
		return false;
	}
	Next();
	if (!Condition()) return false;
	if (!P || P->type != lDo) {
		EX = new Lex({ lDo });
		Error();
		return false;
	}
	Next();
	if (!Statement()) return false;
	if (!P || P->type != lEnd) {
		EX = new Lex({ lEnd });
		Error();
		return false;
	}
	Next();
	if (P) {
		Error();
		return false;
	}
	return true;
}
