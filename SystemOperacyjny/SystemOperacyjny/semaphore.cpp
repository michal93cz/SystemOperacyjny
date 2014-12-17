#include "semaphore.h"
#include "XPER.h"
#include "globals.h"
#include "REGISTERS.h"

//funkcje pomocnicze dla procedur P i V
void SEMAPHORE::umiesc_na_koncu() //umieszczenie blocku PCB z running na koncu kolejki
{
	kolejka.push(RUNNING);
	std::cout << "dodano do kolejki: " << RUNNING->getName() << std::endl;
	cout << "Ilosc procesow pod semaforem: " << kolejka.size() << endl;
}
void block(SEMAPHORE *sem)
{
	if (RUNNING->getBlocked() != 1) //nie mozna dodac prcesu do kolejki 2 razy, jezeli byl juz wczesniej zablokowany
	{
		RUNNING->setBlocked(); //zablokowanie procesu
		sem->umiesc_na_koncu();
	}
}
void SEMAPHORE::usun_pierwszy() //usuwa pierwszy blok PCB spod semafora
{
	Pcb *tmp2 = kolejka.front();
	kolejka.pop();
	NEXTTRY = tmp2;
	std::cout << "usunieto z kolejki: " << tmp2->getName() << std::endl;
	cout << "Ilosc procesow pod semaforem: " << kolejka.size() << endl;
	tmp2->resetBlocked();
}
void wake_up(SEMAPHORE *sem)
{
	sem->usun_pierwszy();
}

//opracje na semaforach
void SEMAPHORE::V() //podnoszenie semafora
{
	this->SET_VALUE(this->GET_VALUE() + 1); //value++
	if (this->GET_VALUE() <= 0)
	{
		wake_up(this);
	}
}
void SEMAPHORE::P() //opuszczenie semafora
{
	this->SET_VALUE(this->GET_VALUE() - 1); // value -= 1;
	if (this->GET_VALUE() < 0)
	{
		block(this);
		XPER(); //wywo³anie programu zwiadowcy
	}
}