#ifndef run_nexttry_h
#define run_nexttry_h
#include "PCB_class.h"


__declspec(selectany) PCB *RUNNING = new PCB(); //Tells the compiler that the declared global data item
__declspec(selectany) PCB *NEXTTRY = new PCB();

#endif