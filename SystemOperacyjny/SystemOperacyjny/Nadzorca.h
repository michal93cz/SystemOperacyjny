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
	string tab_sys[3];
	//ladowanie karty $JOB
	int Zal_JOB(bool dr_nr);
	Pcb *pierwszyProces = nullptr;
	Pcb *drugiProces = nullptr;
	Pcb *firstGroupOne = nullptr;
	Pcb *firstGroupTwo = nullptr;
	string* Czytanie_komunikatow(string&rozkazy, int rozmiar);
	void Drukowanie_komunikatow();
	string*nazwap_procesu;
	bool IBSUP_ERR();
public:
	Nadzorca(){
		tab_sys[0] = "*IN";
		tab_sys[1] = "*OUT";
		tab_sys[2] = "Proces_bezczynnosci";
	pierwszyProces = new Pcb("*IBSUP", &firstGroupOne);
	RUNNING = pierwszyProces;
	drugiProces = new Pcb("*IBSUP", &firstGroupTwo);
	NEXTTRY = drugiProces;
	}
	~Nadzorca(){
		delete nazwap_procesu;
		delete pierwszyProces;
		delete drugiProces;
	}
	void INIT();
	void FIN();
	void CUSERPROG();
	int Wykonaj(Pcb*proces);
};


#endif