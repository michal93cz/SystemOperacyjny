#ifndef globals_h
#define globals_h
#include "pcb.h"
#include "pamiec.h"
#include "REGISTERS.h"

// __declspec(selectany) mówi kompilatorowi o deklaracji globalnej zmiennej
__declspec(selectany) Pcb *RUNNING; 
__declspec(selectany) Pcb *NEXTTRY;
__declspec(selectany) memory naszaPamiec;
__declspec(selectany) rejestry rejestr;


#endif