#include "MEMORY.h"

bool MEMORY::ustaw_bajt(unsigned short pierwszy_bajt, unsigned short indeks, unsigned char bajt)
{
	
		if (indeks < 256)
		{
			cout << "Wpisano bajt do tablicy pamieci\n";
			tab[pierwszy_bajt + indeks] = bajt;
			count++;
			return true;
		}
		else
		{
			cout<<"Niepowolany doostep do pamieci - przekroczona wartosc indeksu\n";
			return false;
		}

}
bool MEMORY::pobierz_bajt(unsigned short pierwszy_bajt, unsigned short indeks)
{
		if (indeks < 256)
		{
			return tab[pierwszy_bajt + indeks];
		}
		else
		{
			cout<<"Niepowolany doostep do pamieci - przekroczona wartosc indeksu";
			return false;
		}

}
