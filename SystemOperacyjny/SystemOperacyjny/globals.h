#ifndef globals_h
#define globals_h
#include "pcb.h"
#include "pamiec.h"
#include "REGISTERS.h"

// __declspec(selectany) m�wi kompilatorowi o deklaracji globalnej zmiennej
__declspec(selectany) Pcb *RUNNING = nullptr; 
__declspec(selectany) Pcb *NEXTTRY = nullptr;
__declspec(selectany) memory naszaPamiec;
__declspec(selectany) rejestry rejestr;


#endif