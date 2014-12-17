#include "czytnik.h"

Czyt::Czyt()
{
	USER_SEM = new SEMAPHORE(0);	//tworzenie semafora USER_SEMAPHORE
}
//funkcja czytania karty zwracajaca rozkazy na tej karcie
string Czyt::Czytaj(string&nazwa, bool dr_nr, string com,int&rozmiar, int&inout)
{
	JOB job;
	string rozkazy;
	string buf;
	vector <string> tmp;
	if (dr_nr == 1)
	{
		job.JOB_plik("plik1.job");
		buf = job.getJOB();
		tmp = split(buf, ' ');
		if (tmp[0] != "$JOB"){
			cout << "Karta jest nieprawidlowa";
		}
		else{
			if (com != "READ"){
				cout << "Odebrano zly komunikat";
			}
			else{
				string rozkazy1;
				if (USER_SEM->GET_VALUE() != 0)
				{
					cout << "Urzadzenie: Czytnik jest zajete";
				}
				else
				{
					USER_SEM->P();
					cout << "Rozpoczecie czytania:" <<endl;
					rozkazy1 = job.getData();
					cout << rozkazy1 << endl;
					nazwa = job.getName();
					rozmiar = job.getSize();
					inout = job.sprawdzIO();
					

				}
				USER_SEM->V();
				rozkazy = rozkazy1;
			}
		}
	}
	else if (dr_nr == 0)
	{

		job.JOB_plik("plik2.job");
		buf = job.getJOB();
		tmp = split(buf, ' ');
		if (tmp[0] != "$JOB"){
			cout << "Karta jest nieprawidlowa";
		}
		else{

			string rozkazy2;
			if (USER_SEM->GET_VALUE() != 0)
			{
				cout << "Urzadzenie: Czytnik jest zajete";
			}
			else
			{
				if (com != "READ"){
					cout << "Odebrano zly komunikat";
				}
				else{
				USER_SEM->P();
				cout << "Rozpoczecie czytania:"<<endl;
				rozkazy2 = job.getData();
				cout << rozkazy2 << endl;
				nazwa = job.getName();
				rozmiar = job.getSize();
				
			}
			}
			USER_SEM->V();
			rozkazy = rozkazy2;
		}
	}
	return rozkazy;
}

