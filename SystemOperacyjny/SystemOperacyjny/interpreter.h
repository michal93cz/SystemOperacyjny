#include "global.h"
#include <cstdlib>
#include <vector>
#include "split.h"
#include <algorithm>

class Interpreter {
public:
	//kody operacji
	enum class OpCode : unsigned char {
		SET = 'S',
		ADD = 'A',
		SUB = 'U',
		MUL = 'M',
		DIV = 'D',
		JUMP = 'J',
		JMPZ = 'N',
		JPNZ = 'Z',
		OUT = 'O',
		BYE = 'B',
	};
	int op_count;//liczba operacji
	int total_length;//dlugosc calego kodu
	char* buffer;//przechowuje kod
	static bool poprawny_rejestr(string name);
	void interpret_code(string blob);
};