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
	fstream bazadanych(plik,ios::in);

	if(bazadanych.is_open())
	{
	
		int nrlini = 0;
		string buf;

		cout << "Wczytywanie pliku do bufora.";
		while(!bazadanych.eof()) //wczytywanie danych z karty do bufora
		{
			Buf.push_back(Bufor());
			getline(bazadanych, buf);
			Buf[nrlini].dane=buf;
			getline(bazadanych, buf);
			Buf[nrlini].nazwa = buf;
			getline(bazadanych, buf);
			Buf[nrlini].rozkazy = buf;
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
string JOB::getName(int i)
{
	Name = Buf[i].nazwa;
	return Name;
}
//funkcja zwracajaca dane z karty
string JOB::getData(int i)
{
	Data = Buf[i].rozkazy;
	return Data;
}

string JOB::getJOB(int i)
{
	Job = Buf[i].dane;
	return Job;

}

int JOB::getSize(int i)
{
	vector <string> s = split(Buf[i].dane, ' ');
	int size = atoi(s[1].c_str());
	return size;
}

int JOB::sprawdzIO(int i)
{
	vector <string> k = split(Buf[i].dane, ' ');
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
