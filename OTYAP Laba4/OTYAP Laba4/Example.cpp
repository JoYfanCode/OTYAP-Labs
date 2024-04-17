#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

// ���� ������
enum ELexType { lWh, lDo, lEnd, lLo, lCo, lId, lVl, lAo, lEq };

// ��������� ������������� ������
char TYPE_NAMES[9][3] = { "wh", "do", "en", "lo", "co", "id", "vl", "ao", "eq" };

// ������ ������
struct Lex {
	ELexType type;
	int index;
	int pos;
	Lex* next;
};

// ������ ������
Lex* P = NULL;
// ����� ������� �������
int CURPOS = 0;
// ������ ������, ������� ��������� (��� ������ ������)
Lex* EX = NULL;
// ���� ��� ������
ofstream FOUT;

// ����� ������
void Error() {
	FOUT << CURPOS << ' ';
	while (EX) {
		FOUT << TYPE_NAMES[EX->type] << ' ';
		EX = EX->next;
	}
}

// ��������������� ������� ������ ���������
void Next() {
	P = P->next;
	CURPOS++;
}

// �������� ��������
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

// �������� �������������� ��������
bool ArithExpr() {
	if (!Operand()) return false;
	while (P && P->type == lAo) {
		Next();
		if (!Operand()) return false;
	}
	return true;
}



// �������� ��������� ���������
bool CompExpr() {
	if (!Operand()) return false;
	if (P && P->type == lCo) {
		Next();
		if (!Operand()) return false;
	}
	return true;
}

// �������� �������
bool Condition() {
	if (!CompExpr()) return false;
	while (P && P->type == lLo) {
		Next();
		if (!CompExpr()) return false;
	}
	return true;
}

// �������� ��������������� ���������
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

// �������������� ������ �����������
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
