#ifndef proces_h
#define proces_h
#include "global.h"
#include "PCB_class.h"
#include "RUNNING_NEXTTRY.h"

class Proces
{
public:

	Proces(){ cout << "Utworzenie obiektu Proces\n"; }
	~Proces(){}
	unsigned short pierwszy_bajt_pamieci=0;
	unsigned short mem_pointer=0;
	Proces*zpro(string nazwa);
	Proces*Running=nullptr;
	string nazwa;
	int t_obslugi=0;
	void CProces(int rozmiar, const char nazwa[8]);

};


#endif