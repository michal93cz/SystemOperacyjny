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
		while(nrlini!=3) //wczytywanie danych z karty do bufora
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

int JOB::getSize()
{
	vector <string> s = split(tab[0], ' ');
	int size = atoi(s[1].c_str());
	return size;
}

int JOB::sprawdzIO()
{
	vector <string> k = split(tab[0], ' ');
	int procesIO;
	if (k[2] == "IN" && k[3] == "OUT")
	{
		procesIO = 1; //in i out
	}
	else if (k[2] == "IN" && k[3] == "")
	{
		procesIO = 2; // tylko in
	}
	else procesIO = 3; // tylko out
	
	return procesIO;
}