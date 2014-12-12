//POZIOM 3

#ifndef pcb_h
#define pcb_h

#include"message.h"
#include"semaphore.h"
#include"REGISTERS.h"

using namespace std;


class Message;

class Pcb
{
	char *name;
	Pcb *next_pcb_this_group;
	Pcb *last_pcb_this_group;
	Pcb *next_pcb_all;
	Pcb *last_pcb_all;
	bool stopped;
	bool blocked;
	Message *first_message;
	int auto_storage_size;
	int kwantCzasu;

public:
	int mem_pointer = 0;
	int auto_storage_adress;
	Pcb(char *,Pcb **);
	~Pcb();
	static SEMAPHORE message_semaphore_common;
	SEMAPHORE message_semaphore_receiver;
	static Pcb *firstAllPcb;
	rejestry MEMORY_SAVE_AREA;

	//metody ogólne
	char *getName();
	Pcb *getNextThisGroup();
	void setNextThisGroup(Pcb *);
	Pcb *getNextAll();
	void setNextAll(Pcb *);
	Pcb *getLastThisGroup();
	void setLastThisGroup(Pcb *);
	Pcb *getLastAll();
	void setLastAll(Pcb *);
	void setStopped();
	void resetStopped();
	bool getStopped();
	void setBlocked();
	void resetBlocked();
	bool getBlocked();
	Message *getFirstMessage();
	void setFirstMessage(Message *);
	int getAutoStorageSize();
	void setAutoStorageSize(int);
	int getAutoStorageAdress();
	void setAutoStorageAdress(int);
	void resetKwantCzasu();
	void setKwantCzasu(int);
	int getKwantCzasu();

	//metody zarz¹dzania procesami
	Pcb **firstPcb;

	Pcb *szukanieProcesu(char *nazwa_procesu);
	void tworzenieProcesu(char *nazwa_procesu, int wielkosc_pamieci);
	void usuniecieProcesu(char *nazwa_procesu);
	void zatrzymanieProcesuPowiadomienie();
	string *czytanieKomunikatu();
	void wysylanieKomunikatu(char *nazwa_odbiorcy, int liczba_znakow, char *tresc_komunikatu);
	void usuniecieKomunikatow();
	void uruchomienieProcesu(char *nazwa_procesu); //adres pamieciowy programu?
	void zatrzymywanieProcesu(char *nazwa_procesu);
	void bladPowiadomienie();
	void dolaczenieProcesu(Pcb *proces);
	void odlaczenieProcesu(Pcb *proces);


	void wydrukujWszystkieProcesy();
};



#endif