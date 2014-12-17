#ifndef JOB_h
#define JOB_h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"split.h"
#include "Bufor.h"

using namespace std;

class JOB
{
private:
	string Job;
	string Name;
	string Data;

	vector<Bufor> Buf;

public:
	JOB();

	void JOB_plik(int&licznik,string plik); //otwieranie karty z pliku
	string getJOB(int i);
	int getSize(int i);
	int sprawdzIO(int i);
	string getData(int i); // funkcja zwracajaca dane z karty
	string getName(int i); // funkcja zwracajaca nazwe
	


};





#endif JOB_h