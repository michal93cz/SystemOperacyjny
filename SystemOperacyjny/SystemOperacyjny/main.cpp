#include "Nadzorca.h"


int main(){
	std::cout << "\nSymulacja Systemu Operacyjnego IBM 360" << std::endl;
	Nadzorca*IPLRTN=new Nadzorca;
	IPLRTN->INIT();
	IPLRTN->CUSERPROG();
	IPLRTN->FIN();
	return 0;
}