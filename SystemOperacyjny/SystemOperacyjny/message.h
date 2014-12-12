//POZIOM 3

#ifndef message_h
#define message_h

#include "pcb.h"
#include <string>

using namespace std;

class Pcb;

class Message
{
public:
	Pcb *sender;
	Message *next;
	int size;
	string text;

	Message(Pcb *, int, string);
	void umieszczenieNaKoncu(Pcb *);
	string pobraniePierwszego(Pcb *);
};


#endif