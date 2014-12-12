#ifndef pamiec_cpp
#define pamiec_cpp

#include "semaphore.h"
#include "pamiec.h"
#include <iostream>
#include <string>


using namespace std;
using byte = unsigned char;



// dodanie do listy, first - pierwszy bajt, size - wielkosc bloku
	void lista2::add(int first, int size){ 
		lista1 *proces = new lista1;
		proces->first = first;
		proces->size = size;
		if (head){
			proces->next = head->next;
			head->next = proces;
		}
		else{
			proces->next = proces;
		}
		head = proces;
		cout << "Dodano do listy." << endl;
	}

// usuniêcie z listy, brak argumentów
	void lista2::del(){ 
		if (head){
			lista1 *proces = head->next;
			head->first = proces->first;
			head->size = proces->size;
			head->next = proces->next;
			if (proces->next == proces){
				head = NULL;
			}
			delete proces;
		}
		cout << "Usunieto z listy." << endl;
	}

// zliczenie elementów listy FSB lub ZSB
	int lista2::amount(){ 
		lista1 *proces = head;
		byte c = 0;
		do{
			c++;
			proces = proces->next;
		} while (proces != head);
		return c;
	}

	// konstruktor list
	lista2::lista2() { head = NULL; }

	// destruktor list
	lista2::~lista2() { while (head) { del(); } }

// sortowanie listy FSB, nowy - ostatnio dodany element
	void memory::sort(lista1* nowy){ 
		int checkpoint = 0;
		lista1 *iterator = FSB.head;
		do{
			if (nowy->size < iterator->size){
				if (iterator == FSB.head){
					lista1* temp = FSB.head;
					while (temp->next != FSB.head){
						temp = temp->next;
					}
					FSB.head = temp;
					FSB.add(nowy->first, nowy->size);
					temp = FSB.head->next;
					FSB.head = nowy;
					FSB.del();
					FSB.head = temp;
				}
				else{
					int c = iterator->first;
					int b = iterator->size;
					iterator->first = nowy->first;
					iterator->size = nowy->size;
					lista1* temp = FSB.head;
					FSB.head = iterator;
					FSB.add(c, b);
					/////////////
					FSB.head = nowy;
					FSB.del();
					/////////////
					checkpoint = 1;
				}
			}
			iterator = iterator->next;
		} while (checkpoint != 1 && iterator != FSB.head);
		cout << "Posortowano liste wolnej pamieci." << endl;
	}

// scalanie elementow s¹siaduj¹cych z listy FSB, nowy - ostatnio dodany element
	void memory::merge(lista1* nowy){ 
		lista1 *iterator = nowy, *temp = 0, *sort1 = nowy;
		int checkpoint = 0;
		do{ // scalenie z pozniejszym obszarem
			if ((nowy->first + nowy->size) == iterator->first){
				nowy->size = nowy->size + iterator->size;
				if (iterator == FSB.head){
					if (FSB.head->next == nowy){
						checkpoint = 1;
						FSB.del();
						nowy = FSB.head;
						sort1 = nowy;
					}
					else{
						checkpoint = 1;
						FSB.del();
						sort1 = nowy;
					}
				}
				else{
					if (FSB.head == iterator->next){
						FSB.head = iterator;
						if (FSB.head->next == nowy){
							nowy = FSB.head;
						}
						checkpoint = 1;
						FSB.del();
						sort1 = nowy;
					}
					else{
						temp = FSB.head;
						FSB.head = iterator;
						if (FSB.head->next == nowy){
							nowy = FSB.head;
						}
						checkpoint = 1;
						FSB.del();
						FSB.head = temp;
						sort1 = nowy;
					}
				}
			}
			iterator = iterator->next;
		} while (iterator != nowy && checkpoint != 1);
		checkpoint = 0;
		do{ // scalenie z wczesniejszym obszarem
			if ((iterator->first + iterator->size) == nowy->first){
				iterator->size = iterator->size + nowy->size;
				if (nowy == FSB.head){
					if (FSB.head->next == iterator){
						checkpoint = 1;
						FSB.del();
						sort1 = FSB.head;
					}
					else{
						checkpoint = 1;
						FSB.del();
						sort1 = iterator;
					}
				}
				else{
					temp = FSB.head;
					FSB.head = nowy;
					if (FSB.head->next == iterator){
						checkpoint = 1;
						FSB.del();
						sort1 = FSB.head;
						FSB.head = temp;
					}
					else{
						if (FSB.head->next != temp){
							FSB.del();
							FSB.head = temp;
							checkpoint = 1;
							sort1 = iterator;
						}
						else{
							checkpoint = 1;
							FSB.del();
							sort1 = iterator;
							/**/
						}
					}
				}
			}
			iterator = iterator->next;
		} while (iterator != nowy && checkpoint != 1);
		cout << "Scalono (jesli bylo trzeba)." << endl;
		sort(sort1); // sortowanie po scaleniu
	}

	//konstruktor pamieci, utworzenie list i semaforów
	memory::memory(){
		cout << "Utworzono liste wolnej pamieci." << endl;
		FSB.add(1, 255);
		cout << "Utworzono liste zajetej pamieci." << endl;
		zSB.add(0, 1);
		FSBSEM->SET_VALUE(1);
		MEMSEM->SET_VALUE(0);
		cout << "Utworzono semafory FSBSEM i MEMSEM (MEMORY)." << endl;
	}

	//destruktor pamiêci, usuwanie list
	memory::~memory(){
		FSB.~lista2();
		cout << "Usunieto liste wolnej pamieci." << endl;
		zSB.~lista2();
		cout << "Usunieto liste zajetej pamieci." << endl;
	}

// przydzielanie pamiêci, size - wymagana wielkoœæ, zwraca adres pierwszego bajtu bloku po zaalokowaniu
	int memory::allocate(int size){ 
		if (FSBSEM->GET_VALUE() != 1){ // semafor zablokowany
			cout << "Brak dostepu do pamieci." << endl;
//			return 255;
		}
		else if (FSBSEM->GET_VALUE() == 1){
			if (MEMSEM->GET_VALUE() != 0){
				cout << "Brak wystarczajaco duzego obszaru pamieci." << endl;
				MEMSEM->P();
			}
			else{
				lista1 *iterator = FSB.head; // pomocnicza do iterowanie po liœcie
				lista1 *temp = iterator; // pomocnicza
				int x = 255; // ostatni bajt w pamiêci
				do{
					iterator = iterator->next;
					if (iterator->size >= size && (iterator->size - size) < x){ // szukanie wystarczaj¹co du¿ego bloku w FSB
						x = iterator->size - size;
						temp = iterator;
					}
				} while (iterator != FSB.head);
				if (x == 0){ // zajêcie pamiêci (dokladnie taki obszar pamieci jak szukamy)
					int pierwszy = temp->first; // pierwszy bajt zajêtego bloku
					zSB.add(pierwszy, size);
					lista1 *temp3 = FSB.head; // pomocnicza
					if (FSB.head == temp){
						FSB.del();
					}
					else{
						if (FSB.head == temp->next){
							FSB.head = temp;
							FSB.del();
						}
						else{
							FSB.head = temp;
							FSB.del();
							FSB.head = temp3;
						}
					}
					cout << "Przydzielono pamiec." << endl;
					return pierwszy;
				}
				else{
					if (x == 255){ // nie mieœci siê
						cout << "Brak odpowiednio duzego wolnego fragmentu pamieci." << endl;
						MEMSEM->P();
						return 255;
					}
					else{ // zajêcie pamiêci (obszar wiêkszy ni¿ szukany)
						zSB.add(temp->first, size);
						int pierwszy = temp->first;
						temp->first = temp->first + size;
						temp->size = temp->size - size;
						FSBSEM->P();
						sort(temp);
						FSBSEM->V();
						cout << "Przydzielono pamiec." << endl;
						return pierwszy;
					}
				}
			}
		}
	}
// zwalanianie pamiêci, first - pierwszy bajt zwalnianego bloku
	void memory::release(int first){ 

		if (FSBSEM->GET_VALUE() != 1){ //semafor zablokowany
			cout << "Brak dostepu do pamieci." << endl;
		}
		else if (FSBSEM->GET_VALUE() == 1){
			FSBSEM->P();
			while (zSB.head->first != first){
				zSB.head = zSB.head->next; //przeszukiwanie listy zajêtej pamieci ¿eby znalezc t¹ ktor¹ chcê zwolniæ
			}
			int size = zSB.head->size;
			zSB.del();
			FSB.add(first, size);
			merge(FSB.head); // próba scalenie po zwolnieniu
			FSBSEM->V();
			if (MEMSEM->GET_VALUE() < 0){ // sekwencja operacji V, tyle razy ile procesow oczekuje pod semaforem
				for (int i = 0; i > MEMSEM->GET_VALUE(); i--){
					MEMSEM->V();
				}
			}
			cout << "Zwolniono pamiec." << endl;
		}
	}

// ustawienie bajtu (wpisanie do tablicy pamiêci), first - pierwszy bajt, id - nr bajtu programu (przesuwanie, ¿eby wpisywaæ dane do kolejnych bajtów, nie tylko do pierwszego), byte - wpisywana wartoœæ
	bool memory::setByte(int first, int id, byte byte){ 
		lista1 *iterator = zSB.head; // pomocnicza do iterowanie po liœcie
		int checkpoint = 0;
		do{
			iterator = iterator->next;
			if (iterator->first == first){
				checkpoint = 1;
			}
		} while (iterator != zSB.head && checkpoint != 1);
		if (checkpoint == 1){
			pamiec[id + first] = byte; // wpisanie do odpowiedniego bajtu (id+first) wartosci byte
			return true;
		}
		else{
			cout << "Blad." << endl;
			return false;
		}
	}

// pobranie bajtu (czytanie), first - pierwszy bajt, id - nr bajtu programu (przesuwanie, ¿eby nie odczytywaæ tylko pierwszego bajtu)
	byte memory::getByte(int first, int id){ 
		lista1 *iterator = zSB.head;
		int checkpoint = 0;
		do{
			iterator = iterator->next;
			if (iterator->first == first){
				checkpoint = 1;
			}
		} while (iterator != zSB.head && checkpoint != 1);
		if (checkpoint == 1){
			return pamiec[first + id];
		}
		else return 255;
	}

	void memory::displayPamiec(){ // wyœwietlenie tablicy pamieci
		cout << endl;
		cout << "________________________________________________________________________________" << endl;
		cout << "Tablica pamieci:" << endl;
		/*{
			cout << "WYPELNIONA PRZYKLADOWYMI DANYMI!!!" << endl;
			for (int i = 0; i < 256; i++){
				pamiec[i] = i;
			}
		}*/
		cout << "________________________________________________________________________________" << endl;
		for (int i = 0; i < 256; i++){
			//cout << (int)pamiec[i] << "  ";
			if ((int)pamiec[i]<=10) cout << (int)pamiec[i] << " ";
			else  cout << (char)pamiec[i] << " ";
		}
		cout << endl;
		cout << "________________________________________________________________________________" << endl;
	}

	void memory::displayLists(){ // wyœwietlenie list pamiêci
		//
		zSB.head = zSB.head->next;
		//
		lista1 *wolna = FSB.head;
		lista1 *zajeta = zSB.head;
		cout << "________________________________________________________________________________" << endl;
		cout << "Lista wolnej pamieci: " << FSB.amount() << " blokow." << endl;
		cout << "________________________________________________________________________________" << endl;
		do{
			cout << "Pierwszy bajt: " << wolna->first << " wielkosc: " << wolna->size << " ostatni bajt: " << wolna->first + wolna->size - 1 << endl;
			wolna = wolna->next;
		} while (wolna != FSB.head); // poniewa¿ lista cykliczna
		cout << endl;
		cout << "________________________________________________________________________________" << endl;
		cout << "Lista zajetej pamieci: " << zSB.amount() << " blokow." << endl;
		cout << "________________________________________________________________________________" << endl;
		do{
			cout << "Pierwszy bajt: " << zajeta->first << " wielkosc: " << zajeta->size << " ostatni bajt: " << zajeta->first + zajeta->size - 1 << endl;
			zajeta = zajeta->next;
		} while (zajeta != zSB.head); // poniewa¿ lista cykliczna
		cout << "________________________________________________________________________________" << endl;
		cout << endl;
	}




// V --> +1
// P --> -1

#endif