#ifndef pamiec_h
#define pamiec_h

#include "semaphore.h"

#include <iostream>
#include <string>


using namespace std;
using byte = unsigned char;


class lista1{
public:
	lista1(){}
	~lista1(){}
	lista1 *next = nullptr;
	int first;
	int size;
};

class lista2{
public:
	lista1 *head = nullptr;
//	void display();
	void add(int first, int size); // dodanie do listy FSB lub ZSB
	void del(); // usuniêcie z listy FSB lub ZSB
	int amount(); // zliczenie elementów listy FSB lub ZSB
	lista2();
	~lista2();
};

class memory{
public:
	byte pamiec[256];
	SEMAPHORE *MEMSEM = new SEMAPHORE(0); // semafor MEMORY
	SEMAPHORE *FSBSEM = new SEMAPHORE(1);  // semafor FSBSEM
	lista2 FSB; // lista FSB
	lista2 zSB; // lista ZSB
	void sort(lista1* nowy); // sortowanie listy FSB
	void merge(lista1* nowy); // scalanie elementow s¹siaduj¹cych z listy FSB
	memory();
	~memory();
	int allocate(int size); // przydzielanie pamiêci
	void release(int first); // zwalanianie pamiêci
	bool setByte(int first, int id, byte byte); // ustawienie bajtu
	byte getByte(int first, int id); // pobranie bajtu
	void displayPamiec(); // wyœwietlenie tablicy pamiêci
	void displayLists(); // wyœwietlenie list pamiêci
};



// V --> +1
// P --> -1

#endif