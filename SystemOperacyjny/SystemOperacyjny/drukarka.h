#ifndef drukarka_h
#define drukarka_h

#include<iostream>
#include<string>
#include<fstream>
#include "semaphore.h"
#include "pcb.h"

using namespace std;

class Druk
{
private:
	SEMAPHORE *USER_SEM;
	string *tab1, *tab2;
	


public:
	Druk();
	void Drukuj(char zapisywanie[], string IO, string com); //funkcja drukarki do zapisywania do pliku(drukowania)
	void wyswietl();
	void wyczyscdr(string IO);

};





#endif