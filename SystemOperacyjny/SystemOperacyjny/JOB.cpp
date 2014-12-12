#include "JOB.h"

JOB::JOB()
{
	Job = "null";
	Name = "null";
	Data = "null";
	
}
//otwieranie karty job z pliku
void JOB::JOB_plik(string plik)
{
	cout << "Orwieranie karty JOB" << endl;
	int rozmiar = 3;
	fstream bazadanych(plik,ios::in);

	if(bazadanych.is_open())
	{
		tab = new string[rozmiar];
		int nrlini = 0;
		string buf;

		cout << "Wczytywanie pliku do bufora.";
		while(bazadanych.eof()!=1) //wczytywanie danych z karty do bufora
		{
			getline(bazadanych, buf);
			tab[nrlini]=buf;
			nrlini++;
		}
		cout << "Wczytywanie zakonczone.";
	}
	else
	{
		cout << "Blad: Nie otworzono pliku";
	}
	bazadanych.close();
}
//funkcja zwracajaca nazwe procesu
string JOB::getName()
{
	Name = tab[1];
	return Name;
}
//funkcja zwracajaca dane z karty
string JOB::getData()
{
	Data = tab[2];
	return Data;
}

string JOB::getJOB()
{
	Job = tab[0];
	return Job;

}

