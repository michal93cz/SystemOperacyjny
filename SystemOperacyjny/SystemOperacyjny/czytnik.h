#ifndef czytnik_h
#define czytnik_h

#include <iostream>
#include <string>
#include <fstream>
#include "JOB.h"
#include "semaphore.h"
using namespace std;

class Czyt
{
private:
	SEMAPHORE *USER_SEM;
	JOB job;
public:
	Czyt();
	string Czytaj(string&nazwa, bool dr_nr, string com,int&rozmiar, string&inout); //funkcja czytajaca karte 



};





#endif