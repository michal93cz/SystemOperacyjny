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

	void JOB_plik(string plik); //otwieranie karty z pliku
	string getJOB(int i);	//funkcja zwracajaca pierwsza linijke karty
	int getSize(int i);		//funkcja zwracajaca pamiec
	int sprawdzIO(int i);	//funkcja sprawdzajaca wystapienie odnosnikow in i out
	string getData(int i); // funkcja zwracajaca dane z karty
	string getName(int i); // funkcja zwracajaca nazwe
	int nrlini = 0;
	


};





#endif JOB_h