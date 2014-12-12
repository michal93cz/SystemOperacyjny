#ifndef semaphore_h
#define semaphore_h
#include <iostream>
#include <queue>
using namespace std;

class Pcb; //definicja klasy PCB- aby unikn�� zap�tlenia i umo�liwi� utworzenie wskaznika do pierwszego procesu pod semaforem
//pola z nag��wka PCB_class s� za��czone do semaphore.cpp wraz z naglowkiem semaphore.h -tam odbywa si� prawid�owe definiowanie
class SEMAPHORE{
public:
	queue<Pcb*> kolejka;
	int value;
	SEMAPHORE() {}
	SEMAPHORE(int v_pocz){//konstruktor nadaj�cy warto�� pocz�tkow� semaforowi
		value = v_pocz;
	}
	~SEMAPHORE() {}
	void SET_PCB(Pcb *r);
	void NEXT_PCB();
	void wirte();
	void SET_VALUE(int v)
	{
		value = v;
	}
	int GET_VALUE()
	{
		return value;
	}

	//funkcje pomocnicze do oblsugi kolejki PCB
	void usun_pierwszy();
	void umiesc_na_koncu();
	void P(); //opuszczenie semafora
	void V(); //podnoszenie semafora
};

#endif