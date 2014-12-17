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
				cout << "Drukuje: ";
				wyniki.open("wyniki1.txt", ios::app | ios::ate);
				if (wyniki.is_open())
				{
					wyniki << zapisywanie << endl;
					cout << zapisywanie << endl;
				}
				else
				{
					cout << "Blad, nie otworzono pliku.";
				}

				USER_SEM->V();
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
				cout << "Drukuje: ";
				wyniki.open("wyniki2.txt", ios::app | ios::ate);
				if (wyniki.is_open())
				{
					wyniki << zapisywanie << endl;
					cout << zapisywanie << endl;
				}
				else
				{
					cout << "Blad, nie otworzono pliku.";
				}

				USER_SEM->V();

			}
		}
	}
}

