#ifndef PCBclass_h
#define PCBclass_h
#include "semaphore.h"
#include "REGISTERS.h"
#include <string>
using namespace std;



class PCB{
public:
	PCB(){}
	PCB(string name){
		NAME = name;
	}
	PCB(string name, bool blocked, bool stopped)
	{
		NAME = name;
		BLOCKED = blocked;
		STOPPED = stopped;
	}
	~PCB(){
		delete NEXT_PCB_THIS_GROUP, NEXT_PCB_ALL, LAST_PCB_ALL, LAST_PCB_THIS_GROUP, AUTO_STORRAGE_ADDRESS;
	}
	//***********************************
	string NAME;
	
	//***********************************
	PCB *NEXT_PCB_THIS_GROUP = nullptr;
	PCB *LAST_PCB_THIS_GROUP = nullptr;
	PCB *NEXT_PCB_ALL = nullptr;
	PCB *LAST_PCB_ALL = nullptr;
	//***********************************
	bool STOPPED;
	bool BLOCKED;
	int kwant_czasu = 0; //gdy 4 powinna nast¹piæ zmiana na procesu w RUNNING
	//**********************************
	int AUTO_STORRAGE_SIZE;
	int AUTO_STORRAGE_ADDRESS;;
	//**********************************
	static SEMAPHORE MESSAGE_SEMAPHORE_COMMON();
	SEMAPHORE MESSAGE_SEMAPHORE_RECEIVER();

	PCB *NEXT_SEMAPHORE_WAITER = nullptr; // nieuzywane <queue> zainicjaowane w semaforze //wskaŸnik do nastênego procesu czekaj¹cego pod semaforem

	//********************************** 
	rejestry *MEMORY_SAVE_AREA = new rejestry();
	//funkcje do obslugi rejestow;
	void SAVE_rejestr(rejestry *glowny) //zapisanie stanu rejestów do obszaru ochronnego
	{
		MEMORY_SAVE_AREA = glowny;
	} 
	void READ_MEMORY_SAVE_AREA(rejestry *glowny)//przepisanie stanu rejestrów z obszaru ochronnego do rejestru g³ównego
	{
		glowny = MEMORY_SAVE_AREA;
	}
	void Wyswietl_stan_rejestru_PCB()
	{
		MEMORY_SAVE_AREA->display_reg();
	}
	int Przekaz_w_rejestru(int a){
		switch (a){
		case 1:
		{
				  return  MEMORY_SAVE_AREA->Get_a();
		}
		case 2:
		{
				  return  MEMORY_SAVE_AREA->Get_b();
		}
		case 3:
		{
				  return  MEMORY_SAVE_AREA->Get_c();
		}
		case 4:
			return  MEMORY_SAVE_AREA->Get_d();

		}
	}
	void Ustaw_w_rejestru(int a,int b){
		switch (a){
		case 1:
		{
				  return MEMORY_SAVE_AREA->Set_a(b);
		}
		case 2:
		{
				  return  MEMORY_SAVE_AREA->Set_b(b);
		}
		case 3:
		{
				  return  MEMORY_SAVE_AREA->Set_c(b);
		}
		case 4:
			return  MEMORY_SAVE_AREA->Set_d(b);

		}
	}
};

#endif PCB_h