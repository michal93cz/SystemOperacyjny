#include "REGISTERS.h"
#include "ZAWIADOWCA.h"


void rejestry::pcb_time()
{
	RUNNING->setKwantCzasu(RUNNING->getKwantCzasu() +1);
	if (RUNNING->getKwantCzasu() == 5)//gdy kwant czasu osi¹gnie 5 (wykonanych zostanie 5 rozkazow na PCB)
		zawiadowca();//zmiana procesora
}
int rejestry::Przekaz_w_rejestru(int a){
	switch (a){
		case 1:
		{
				  return  rejestr.Get_a();
		}
		case 2:
		{
				  return  rejestr.Get_b();
		}
		case 3:
		{
				  return  rejestr.Get_c();
		}
		case 4:
		{
			return  rejestr.Get_d();
		}
		default:
		{
			return 0;
		}
	}
}
void rejestry::Ustaw_w_rejestru(int a, int b){
	switch (a){
	case 1:
	{
			  return rejestr.Set_a(b);
	}
	case 2:
	{
			  return  rejestr.Set_b(b);
	}
	case 3:
	{
			  return  rejestr.Set_c(b);
	}
	case 4:
		return  rejestr.Set_d(b);

	}
}