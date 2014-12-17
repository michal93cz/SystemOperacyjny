#ifndef JOB_h
#define JOB_h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"split.h"

using namespace std;

class JOB
{
private:
	string Job;
	string Name;
	string Data;

	string *tab;

public:
	JOB();
	JOB(string Name, string Data)
	{
		this->Name=Name;
		this->Data=Data;
	}

	void JOB_plik(string plik); //otwieranie karty z pliku
	string getJOB();
	int getSize();
	int sprawdzIO();
	string getData(); // funkcja zwracajaca dane z karty
	string getName(); // funkcja zwracajaca nazwe
	


};





#endif JOB_h