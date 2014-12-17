#include "czytnik.h"

Czyt::Czyt()
{
	USER_SEM = new SEMAPHORE(0);	//tworzenie semafora USER_SEMAPHORE
}
//funkcja czytania karty zwracajaca rozkazy na tej karcie
vector <Bufor> Czyt::Czytaj(bool dr_nr, string com)
{
	JOB job;
	string buf;
	vector <string> tmp;
	vector <Bufor> bufor;
	if (dr_nr == 1)
	{
		job.JOB_plik("plik1.job");
		buf = job.getJOB(0);
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
					for (int i = 0; i < 1; i++){
						bufor.push_back(Bufor());
						bufor[i].rozkazy = job.getData(i);
						//cout << rozkazy1 << endl;
						bufor[i].nazwa = job.getName(i);
						bufor[i].size = job.getSize(i);
						bufor[i].inout = job.sprawdzIO(i);
					}

				}
				USER_SEM->V();
			}
		}
	}
	else if (dr_nr == 0)
	{

		job.JOB_plik("plik2.job");
		buf = job.getJOB(0);
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
				cout << "Rozpoczecie czytania:" << endl;
				for (int i = 0; i < 1; i++){
					bufor.push_back(Bufor());
					bufor[i].rozkazy = job.getData(i);
					//cout << rozkazy1 << endl;
					bufor[i].nazwa = job.getName(i);
					bufor[i].size = job.getSize(i);
					bufor[i].inout = job.sprawdzIO(i);
				}
				
			}
			}
			USER_SEM->V();
		}
	}
	return bufor;
}

