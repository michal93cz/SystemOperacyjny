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
	JOB(string Name, string Data)
	{
		this->Name=Name;
		this->Data=Data;
	}

	void JOB_plik(string plik); //otwieranie karty z pliku
	string getJOB(int i);
	int getSize(int i);
	int sprawdzIO(int i);
	string getData(int i); // funkcja zwracajaca dane z karty
	string getName(int i); // funkcja zwracajaca nazwe
	int nrlini = 0;
	


};





#endif JOB_h