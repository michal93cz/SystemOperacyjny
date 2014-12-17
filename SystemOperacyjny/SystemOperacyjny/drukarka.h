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
	

public:
	Druk();
	void Drukuj(char zapisywanie[], string IO, string com); //funkcja drukarki do zapisywania do pliku(drukowania)

};





#endif