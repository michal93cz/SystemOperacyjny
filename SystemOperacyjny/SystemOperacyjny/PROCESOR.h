#ifndef PROCESOR_h
#define PROCESOR_h
#include <iostream>
#include "globals.h"
#include "REGISTERS.h"

class PROCESOR{
public:
	PROCESOR(){}
	~PROCESOR(){}
	static void wypisz_stan_rejestow()
	{
		rejestr.display_reg();
	}
};

#endif PROCESOR_h