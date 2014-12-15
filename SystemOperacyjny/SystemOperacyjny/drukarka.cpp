#include "drukarka.h"

Druk::Druk()
{
	USER_SEM = new SEMAPHORE(0);  //tworzenie semafora USER_SEMAPHORE
}

void Druk::Drukuj(char zapisywanie[], string IO, string com)
{
	if (com != "PRIN"){
		cout << "Otrzymano zly komunikat";
	}
	else{
		if (IO == "drukarka1")
		{

			if (USER_SEM->GET_VALUE() != 0)
			{
				cout << "Urzadzenie: Drukarka jest zajete" << endl;
			}
			else
			{
				USER_SEM->P();
				fstream wyniki;
				if (wyniki.is_open())
				{
					wyniki.close();
				}
				cout << "Drukujê";
				wyniki.open("wyniki1.txt", ios::app | ios::ate);
				if (wyniki.is_open())
				{
					wyniki << zapisywanie << endl;
				}
				else
				{
					cout << "Blad, nie otworzono pliku.";
				}

				//	USER_SEM->V();
			}
		}
		else
		{
			if (USER_SEM->GET_VALUE() != 0)
			{
				cout << "Urzadzenie: Drukarka jest zajete" << endl;
			}
			else
			{
				USER_SEM->P();
				fstream wyniki;
				if (wyniki.is_open())
				{
					wyniki.close();
				}
				cout << "Drukuje";
				wyniki.open("wyniki2.txt", ios::app | ios::ate);
				if (wyniki.is_open())
				{
					wyniki << zapisywanie << endl;
				}
				else
				{
					cout << "Blad, nie otworzono pliku.";
				}

				//USER_SEM->V();

			}
		}
	}
}


void Druk::wyswietl()
{
	int rozmiar = 10;
	tab1 = new string[rozmiar];
	tab2 = new string[rozmiar];
	cout << "Ostatnie " << rozmiar << " drukowanych danych" << endl;
	fstream bazadanych("wyniki1.txt", ios::in);

	if(bazadanych.is_open())
	{
		int nrlini = 0;
		string buf;

		while(nrlini < rozmiar)
		{
			if(bazadanych.eof()) break;

			getline(bazadanych, buf);
			tab1[nrlini] = buf;
			nrlini++;

		}

		for (int i=0; i<rozmiar;i++)
		{
			cout << "Drukarka1:" << endl;
			cout<<i+1 << ": ";
			cout<< tab1[i] << endl;
		}
	}
	else
	{
		cout << "Blad: Nie otworzono pliku wynikowego";
	}
	bazadanych.close();
	fstream bazadanych2("wyniki2.txt", ios::in);

	if (bazadanych2.is_open())
	{
		int nrlini = 0;
		string buf;

		while (nrlini < rozmiar)
		{
			if (bazadanych2.eof()) break;

			getline(bazadanych2, buf);
			tab2[nrlini] = buf;
			nrlini++;

		}

		for (int i = 0; i<rozmiar; i++)
		{
			cout << "Drukarka2:" << endl;
			cout << i + 1 << ": ";
			cout << tab2[i] << endl;
		}
	}
	else
	{
		cout << "Blad: Nie otworzono pliku wynikowego";
	}
	bazadanych.close();
}


void Druk::wyczyscdr(string IO)
{
	fstream bazadanych;
	string device = IO;
	string plik;
	if (device == "drukarka1")
	{
		plik = "wyniki1.txt";
		if (bazadanych.is_open())
		{
			bazadanych.close();
		}
		bazadanych.open(plik, ios::out | ios::trunc);

		cout << "Czyszczenie pliku zakonczone" << endl;
		bazadanych.close();
	}
	else if (device == "drukarka2")
	{
		plik = "wyniki2.txt";

		if (bazadanych.is_open())
		{
			bazadanych.close();
		}
		bazadanych.open(plik, ios::out | ios::trunc);

		cout << "Czyszczenie pliku zakonczone" << endl;
		bazadanych.close();
	}
	else { cout << "Nie znaleziono urzadzenia" << endl; }

}