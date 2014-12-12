#pragma once
#include "global.h"
class MEMORY
{
public:
	int count;
	MEMORY(){
		cout << "Utworzenie obiektu MEMORY\n";
		count = 0;
	}
	~MEMORY(){}
	char tab[256];
	int pierwszy_bajt_pamieci_2 = 1;
	bool ustaw_bajt(unsigned short pierwszy_bajt, unsigned short indeks, unsigned char bajt);
	bool pobierz_bajt(unsigned short pierwszy_bajt, unsigned short indeks);

};

