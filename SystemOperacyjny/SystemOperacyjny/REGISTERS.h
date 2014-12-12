#ifndef REGISTERS_h
#define REGISTERS_h
#include <iostream>
//#include "globals.h"

using namespace std;

class rejestry{
private:
	int licznik_rozkazow = 0;
public:
	rejestry(){}
	~rejestry(){}

	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;

	void pcb_time(); //odmierz ilosc wykonanych rozkazow na pcb
	int VALUE_L_ROZKAZOW()
	{
		return licznik_rozkazow;
	}

	//funkcje do ustawiania wartosci rejestrow
	void Set_a(int v)
	{
		a = v;
		licznik_rozkazow++;
		pcb_time();
	}
	void Set_b(int v)
	{
		b = v;
		licznik_rozkazow++;
		pcb_time();
	}
	void Set_c(int v)
	{
		c = v;
		licznik_rozkazow++;
		pcb_time();
	}
	void Set_d(int v)
	{
		d = v;
		licznik_rozkazow++;
	}

	//funkcje do pobierania wartosci rejestrow
	int Get_a()
	{
		return a;
	}
	int Get_b()
	{
				
		return b;
	}
	int Get_c()
	{
		return c;
	}
	int Get_d()
	{
		return d;
	}

	void display_reg()
	{
		cout << "Licznik r. : " << licznik_rozkazow << endl;
		
		cout << "Rejestr A :" << a << endl;
		cout << "Rejestr B :" << b << endl;
		cout << "Rejestr C :" << c << endl;
		cout << "Rejestr D :" << d << endl;
	}
	
	int Przekaz_w_rejestru(int a);
	void Ustaw_w_rejestru(int a, int b);
};



#endif REGISTERS_h