#include "globals.h"
#include "ZAWIADOWCA.h"
using namespace std;
void ustaw_nexttry();

void zawiadowca()
{
	ustaw_nexttry();
	if (NEXTTRY->getBlocked() == 0)
	{
		if (NEXTTRY->getStopped() == 0)
		{
			cout << "zmiana bloków PCB w zawiadowcy" << endl;
			RUNNING->MEMORY_SAVE_AREA = rejestr;
			RUNNING = NEXTTRY;
			NEXTTRY = NEXTTRY->getNextAll();
			rejestr = RUNNING->MEMORY_SAVE_AREA;
			RUNNING->resetKwantCzasu();
		}
	}
}

void ustaw_nexttry()
{ 
	Pcb *tmp = RUNNING;
	do{

		if (NEXTTRY->getStopped() != 0 || NEXTTRY->getBlocked() != 0)
			NEXTTRY = NEXTTRY->getNextAll();
		else
			break;
		if (tmp == NEXTTRY)
		{
			cout << endl;
			cout << "Sprawdzono wszystkie bloki PCB na liscie - brak nastepnego bloku gotowego do uruchomienia" << endl;
			//RUNNING == NEXTTRY
			cout << "Aktualny RUNNING: " << tmp->getName() << endl;
			cout << "BLOCKED: " << tmp->getBlocked() << "\t STOPPED: " << tmp->getStopped() << endl << endl;
		}
	} while (tmp != NEXTTRY);
	//przy uruchomieniu dwoch programow tkwi w tej petli
}
