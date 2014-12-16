//POZIOM 3 

#include<iostream>
#include"pcb.h"
#include"semaphore.h"
#include"globals.h"

Pcb* Pcb::firstAllPcb = nullptr;

SEMAPHORE Pcb::message_semaphore_common;

//metody ogólne dla bloku kontrolnego
Pcb::Pcb(char *nazwa, Pcb **pierwszyPcb)
{
	name = nazwa;
	next_pcb_this_group = nullptr;
	last_pcb_this_group = nullptr;
	next_pcb_all = nullptr;
	last_pcb_all = nullptr;
	stopped = true;
	blocked = false;
	first_message = nullptr;
	auto_storage_size = 0;
	auto_storage_adress = 0;
	kwantCzasu = 0;
	firstPcb = pierwszyPcb;
	dolaczenieProcesu(this);
	message_semaphore_receiver.SET_VALUE(0);
	message_semaphore_common.SET_VALUE(1);
}

Pcb::~Pcb()
{
	usuniecieKomunikatow();
	odlaczenieProcesu(this);
}

char* Pcb::getName()
{
	if (this != nullptr)
	{
		return name;
	}
	else
	{
		return nullptr;
	}
}

Pcb* Pcb::getNextThisGroup()
{
	return next_pcb_this_group;
}

void Pcb::setNextThisGroup(Pcb *blok)
{
	next_pcb_this_group = blok;
}

Pcb* Pcb::getNextAll()
{
	return next_pcb_all;
}

void Pcb::setNextAll(Pcb *blok)
{
	next_pcb_all = blok;
}

Pcb* Pcb::getLastThisGroup()
{
	return last_pcb_this_group;
}

void Pcb::setLastThisGroup(Pcb *blok)
{
	last_pcb_this_group = blok;
}

Pcb* Pcb::getLastAll()
{
	return last_pcb_all;
}

void Pcb::setLastAll(Pcb *blok)
{
	last_pcb_all = blok;
}

void Pcb::setStopped()
{
	stopped = true;
}

void Pcb::resetStopped()
{
	stopped = false;
}

bool Pcb::getStopped()
{
	return stopped;
}

void Pcb::setBlocked()
{
	blocked = true;
}

void Pcb::resetBlocked()
{
	blocked = false;
}

bool Pcb::getBlocked()
{
	return blocked;
}

Message* Pcb::getFirstMessage()
{
	return first_message;
}
void Pcb::setFirstMessage(Message *message)
{
	first_message = message;
}

int Pcb::getAutoStorageSize()
{
	return auto_storage_size;
}

void Pcb::setAutoStorageSize(int wielkosc)
{
	auto_storage_size = wielkosc;
}

int Pcb::getAutoStorageAdress()
{
	return auto_storage_adress;
}

void Pcb::setAutoStorageAdress(int adres)
{
	auto_storage_adress = adres;
}

void Pcb::resetKwantCzasu()
{
	kwantCzasu = 0;
}

void Pcb::setKwantCzasu(int value)
{
	kwantCzasu = value;
}
int Pcb::getKwantCzasu()
{
	return kwantCzasu;
}


//metody do zarz¹dzania procesami
Pcb* Pcb::szukanieProcesu(char *nazwa_procesu)
{
	Pcb *wskaznikProcesu = *firstPcb;
	
	if (wskaznikProcesu != nullptr)
	{
		string a=wskaznikProcesu->getName();
		if (a == nazwa_procesu)
		{
			return wskaznikProcesu;
		}
		wskaznikProcesu = wskaznikProcesu->getNextThisGroup();
		bool ster = true;
		while (ster)
		{
			if (wskaznikProcesu != *firstPcb)
			{
				a = wskaznikProcesu->getName();
				if (a != nazwa_procesu)
				{
					wskaznikProcesu = wskaznikProcesu->getNextThisGroup();
				}
				else
				{
					ster = false;
				}
			}
			else
			{
				ster = false;
			}
			
		}
		return wskaznikProcesu;
	}
	else
	{
		return nullptr;
	}
}

void Pcb::tworzenieProcesu(char *nazwa_procesu, int wielkosc_pamieci)
{
	//Przejrzenie kolejki procesów w celu sprawdzenia unikalnoœci nazwy
	Pcb *wskaznikProcesu = szukanieProcesu(nazwa_procesu);

	//Jeœli unikalna przydzielenie odpowiedniego obszaru dla nowego bloku PCB
	if (wskaznikProcesu == *firstPcb)
	{
		std::cout << endl << "Utworzenie procesu " << nazwa_procesu << "." << endl;
		//Zapisanie nazwy, nadanie STOPPED wartoœci 1, BLOCKED wartoœci 0, semaforom wartoœci pocz¹tkowe, wielkoœæ pamiêci
		Pcb *nowyProces = new Pcb(nazwa_procesu, firstPcb);
		nowyProces->setAutoStorageSize(wielkosc_pamieci);
		if (wielkosc_pamieci != 0)
		{
			nowyProces->setAutoStorageAdress(naszaPamiec.allocate(wielkosc_pamieci));
		}
		else
		{
			nowyProces->setAutoStorageAdress(0);
		}
		
		//Do³¹czenie nowego bloku do dwóch list
		//dolaczenieProcesu(nowyProces); - realizowane w konstruktorze Pcb

		
	}
	else
	{
		std::cout << endl << "Nie nastapilo utworzenie procesu " << nazwa_procesu << "." << endl;
		bladPowiadomienie();
	}
}

void Pcb::usuniecieProcesu(char *nazwa_procesu)
{
	//Wywo³anie XZ(zatrzymywanie procesu)
	zatrzymywanieProcesu(nazwa_procesu);
	Pcb *proces = szukanieProcesu(nazwa_procesu);

	if (proces != *firstPcb)
	{
		//Od³¹czenie bloku PCB od obydwu ³añcuchów
		odlaczenieProcesu(proces);

		//Wywo³anie programu XF w celu zwolnienia wszystkich komunikatów czekaj¹cych, pamiêci roboczej procesu oraz zajmowanej przez blok PCB
		naszaPamiec.release(proces->auto_storage_adress);
		usuniecieKomunikatow();
		delete proces;

		std::cout << endl << "Nastapilo usuniecie procesu " << nazwa_procesu << "." << endl;
	}
	
}

void Pcb::zatrzymanieProcesuPowiadomienie()
{
	//Wys³anie komunikatu o powinnoœci zakoñczenia procesu do procesu nadzorczego za pomoc¹ programu XS
	wysylanieKomunikatu("*IBSUP", 18, "Zatrzymanie procesu");

	//Czytanie i usuniêcie komunikatów procesu
	usuniecieKomunikatow();

	//Blokowanie siê procesu
	setBlocked();

	std::cout << endl << "Nastapilo zatrzymanie procesu " << this->getName() << "." << endl;
}

 string* Pcb::czytanieKomunikatu()
{
	 //Wykonanie operacji P na semaforze MESSAGE_SEMAPHORE_RECEIVER w bloku PCB procesu i MESSAGE_SEMAPHORE_COMMON
	 message_semaphore_receiver.P();
	 message_semaphore_common.P();

	 //Pobranie z listy komunikatów pierwszego komunikatu
	 if (first_message != nullptr)
	 {
		 //Przeniesienie tekstu do obszaru odbiorczego
		 string *message = new string(getFirstMessage()->text);
		 string nadawca = getFirstMessage()->sender->getName();
		 std::cout << this->name << "_> Odebralem: " << *message << ", od: " << nadawca << endl;
		 Message *wskaznikNaWiadomosc = first_message;
		 first_message = first_message->next;

		 //Zwolnienie pamiêci zajmowanej przez komunikat, wyjœcie z programu
		 delete wskaznikNaWiadomosc;

		 return message;
	 }
	 else
	 {
		 return nullptr;
	 }

	 //Odblokowanie listy
	 message_semaphore_common.V();
}

void Pcb::wysylanieKomunikatu(char *nazwa_odbiorcy, int liczba_znakow, char *tresc_komunikatu)
{
	//Wyszukanie bloku PCB na podstawie nazwy procesu odbieraj¹cego
	Pcb *wskaznikProcesu = szukanieProcesu(nazwa_odbiorcy);

	if (wskaznikProcesu != nullptr)
	{
		//Wywo³anie modu³u przydzia³u pamiêci w celu przydzielenia odpowiedniego obszaru pamiêci na komunikat
		//Umieszczenie wskaŸnika nadawcy, liczby znaków, tekstu komunikatu i wyzerowanie wskaŸnika nastêpnego komunikatu w bloku pamiêci komunikatu
		Message *nowyKomunikat = new Message(this, liczba_znakow, tresc_komunikatu);

		//Wykonanie operacji P na semaforze MESSAGE_SEMAPHORE_COMMON w bloku PCB procesu odbiorcy
		wskaznikProcesu->message_semaphore_common.P();

		//Umieszczenie bloku na koñcu listy komunikatów
		nowyKomunikat->umieszczenieNaKoncu(wskaznikProcesu);

		//Wykonanie operacji V na semaforach MESSAGE_SEMAPHORE_COMMON i MESSAGE_SEMAPHORE_RECEIVER
		wskaznikProcesu->message_semaphore_common.V();
		wskaznikProcesu->message_semaphore_receiver.V();

		std::cout << endl << "Nastapilo wyslanie komunikatu do " << nazwa_odbiorcy << "." << endl;
	}
}

void Pcb::usuniecieKomunikatow()
{
	while (first_message != nullptr)
	{
		Message *wskaznikKomunikatu = first_message;
		first_message = first_message->next;
		delete wskaznikKomunikatu;
	}
	std::cout << endl << "Nastapilo usuniecie wszystkich komunikatow procesu " << this->getName() << "." << endl;
}

void Pcb::uruchomienieProcesu(char *nazwa_procesu) //adres pamieciowy programu?
{
	//Wyszukanie bloku PCB procesu na podstawie nazwy
	Pcb *wskaznikProcesu = szukanieProcesu(nazwa_procesu);

	//Wpisanie do obszarow ochronnych stan rejestrów
	//realizowane przez zawiadowce

	//Wyzerowanie bitu STOPPED w bloku PCB
	wskaznikProcesu->resetStopped();
	std::cout << endl << "Nastapilo uruchomienie procesu " << wskaznikProcesu->getName() << "." << endl;
}

void Pcb::zatrzymywanieProcesu(char *nazwa_procesu)
{
	//Wyszukanie bloku PCB procesu na podstawie nazwy
	Pcb *wskaznikProcesu = szukanieProcesu(nazwa_procesu);

	if (wskaznikProcesu != nullptr)
	{
		//Nadanie bitowi STOPPED wartoœæ 1
		wskaznikProcesu->setStopped();
		std::cout << endl << "Nastapilo zatrzymanie procesu " << wskaznikProcesu->getName() << "." << endl;
	}
}

void Pcb::bladPowiadomienie()
{
	wysylanieKomunikatu("*IBSUP", 13, "Wystapil blad");
	std::cout << endl << "Nastapilo wyslanie komunikatu z bledem przez " << this->getName() << "." << endl;
}

void Pcb::dolaczenieProcesu(Pcb *proces)
{
	Pcb *wskaznikProcesu;
	if (firstAllPcb)
	{
		/*while (wskaznikProcesu->getNextAll() != firstAllPcb)
		{
			wskaznikProcesu = wskaznikProcesu->getNextAll();
		}
		wskaznikProcesu->setNextAll(proces);
		proces->setLastAll(wskaznikProcesu);
		proces->setNextAll(firstAllPcb);*/

		if (RUNNING)
		{
			wskaznikProcesu = RUNNING->getNextAll();
			wskaznikProcesu->setLastAll(proces);
			proces->setNextAll(wskaznikProcesu);
			RUNNING->setNextAll(proces);
			proces->setLastAll(RUNNING);
		}

	}
	else
	{
		firstAllPcb = proces;
		proces->setNextAll(proces);
		proces->setLastAll(proces);
	}

	
	if (*firstPcb)
	{
		/*while (wskaznikProcesu->getNextThisGroup() != *firstPcb)
		{
			wskaznikProcesu = wskaznikProcesu->getNextThisGroup();
		}
		wskaznikProcesu->setNextThisGroup(proces);
		proces->setLastThisGroup(wskaznikProcesu);
		proces->setNextThisGroup(*firstPcb);*/

		wskaznikProcesu = (*firstPcb)->getNextThisGroup();
		wskaznikProcesu->setLastThisGroup(proces);
		proces->setNextThisGroup((*firstPcb)->getNextThisGroup());
		(*firstPcb)->setNextThisGroup(proces);
		proces->setLastThisGroup(*firstPcb);
	}
	else
	{
		*firstPcb = proces;
		proces->setNextThisGroup(proces);
		proces->setLastThisGroup(proces);
	}

	std::cout << endl << "Nastapilo dolaczenie procesu " << proces->getName() << " do dwoch list." << endl;
}

void Pcb::odlaczenieProcesu(Pcb *proces)
{
	Pcb *wskaznikNext;
	Pcb *wskaznikLast;

	wskaznikNext = proces->getNextAll();
	wskaznikLast = proces->getLastAll();

	wskaznikLast->setNextAll(wskaznikNext);
	wskaznikNext->setLastAll(wskaznikLast);


	wskaznikNext = proces->getNextThisGroup();
	wskaznikLast = proces->getLastThisGroup();

	wskaznikNext->setLastThisGroup(wskaznikLast);
	wskaznikLast->setNextThisGroup(wskaznikNext);

	std::cout << endl << "Nastapilo odlaczenie procesu " << proces->getName() << " od dwoch list." << endl;
}

void Pcb::wydrukujWszystkieProcesy()
{
	std::cout << endl << "Wskaznik na pierwszy proces: " << firstAllPcb << endl;
	std::cout << "Przegladanie calej grupy procesow:" << endl;
	Pcb *wskaznikProcesu = firstAllPcb;
	if (wskaznikProcesu != nullptr)
	{
		do
		{
			std::cout << wskaznikProcesu->getName() << " " << wskaznikProcesu->blocked << " " << wskaznikProcesu->stopped << endl;
			std::cout << "Next: " << wskaznikProcesu->getNextAll() << endl;
			wskaznikProcesu = wskaznikProcesu->getNextAll();

		} while (wskaznikProcesu != firstAllPcb);
	}
}