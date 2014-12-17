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
		if (USER_SEM->GET_VALUE() != 0)
		{
			cout << "Urzadzenie: Czytnik jest zajete";
		}
		else
		{
			if (com != "READ")
			{
				cout << "Odebrano zly komunikat";
			}
			else
			{
				job.JOB_plik("plik1.job");
				USER_SEM->P();
				for (int i = 0; i < job.nrlini; i++)
				{
					buf = job.getJOB(i);
					tmp = split(buf, ' ');
					if (tmp[0] != "$JOB")
					{
						cout << "Nieprawidlowa karta";
						bufor.push_back(Bufor());

					}
					else
					{

						cout << "Rozpoczecie czytania:" << endl;

						bufor.push_back(Bufor());
						bufor[i].rozkazy = job.getData(i);
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
		if (USER_SEM->GET_VALUE() != 0)
		{
			cout << "Urzadzenie: Czytnik jest zajete";
		}
		else
		{
			if (com != "READ")
			{
				cout << "Odebrano zly komunikat";
			}
			else
			{
				job.JOB_plik("plik2.job");
				USER_SEM->P();
				for (int i = 0; i < job.nrlini; i++)
				{
					buf = job.getJOB(i);
					tmp = split(buf, ' ');
					if (tmp[0] != "$JOB")
					{
						cout << "Nieprawidlowa karta";

					}
					else
					{

						cout << "Rozpoczecie czytania:" << endl;

						bufor.push_back(Bufor());
						bufor[i].rozkazy = job.getData(i);
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