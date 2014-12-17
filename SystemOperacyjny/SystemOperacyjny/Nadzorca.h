#pragma once
#ifndef nadzorca_h
#define nadzorca_h

#include "pcb.h"
#include "pamiec.h"
#include "globals.h"
#include "ZAWIADOWCA.h"
#include "czytnik.h"
#include "drukarka.h"
#include "interpreter.h"

using namespace std;

class Nadzorca
{
private:
	//zawieraz procesy systemowe i p_bezczynn
	string tab_sys[2];
	//ladowanie karty $JOB
	void Zal_JOB(int dr_nr);
	Pcb *pierwszyProces = nullptr;
	Pcb *drugiProces = nullptr;
	Pcb *firstGroupOne = nullptr;
	Pcb *firstGroupTwo = nullptr;
	string* Czytanie_karty(string&rozkazy, int&rozmiar, Pcb*wsk,int in_out);
	void Drukowanie_komunikatow();
	string*nazwap_procesu;
	bool IBSUP_ERR();
	bool Tworzenie_wczytywanie_dg(Pcb*wskaznik);
	bool Usuwanie_procesow(string dane);
	void FIN_procesu(Pcb*proces);
	void Przekazywanie_komunikatow(char*proces);
	string*nazwa_in;
	string*nazwa_out;
public:
	Nadzorca(){
		tab_sys[0] = "*IN";
		tab_sys[1] = "*OUT";
		pierwszyProces = new Pcb("*IBSUP", &firstGroupOne);
		pierwszyProces->uruchomienieProcesu("*IBSUP");
		RUNNING = pierwszyProces;
		drugiProces = new Pcb("*IBSUP", &firstGroupTwo);
		drugiProces->uruchomienieProcesu("*IBSUP");
		NEXTTRY = drugiProces;
	}
	~Nadzorca(){
		delete nazwap_procesu;
		delete nazwa_in;
		delete nazwa_out;
		delete pierwszyProces;
		delete drugiProces;
	}
	void INIT();
	void FIN();
	void CUSERPROG();
	int Wykonaj(Pcb*proces);
};


#endif