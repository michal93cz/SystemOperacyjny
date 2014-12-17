#include "Nadzorca.h"


void Nadzorca::INIT(){
	cout << "-------------------------------\n";
	cout << "Tworzenie procesow systemowych\n";
	cout << "-------------------------------\n";
	for (int i = 0; i < 2; i++)
	{
		pierwszyProces->tworzenieProcesu((char*)tab_sys[i].c_str(), 0);
		//pierwszyProces->uruchomienieProcesu((char*)tab_sys[i].c_str());
		RUNNING = drugiProces;
		NEXTTRY = pierwszyProces;
		drugiProces->tworzenieProcesu((char*)tab_sys[i].c_str(), 0);
		//drugiProces->uruchomienieProcesu((char*)tab_sys[i].c_str());
		RUNNING = pierwszyProces;
		NEXTTRY = drugiProces;
		cout << "-------------------------------\n";
		cout << "Utworzono procesy " << tab_sys[i] << "\n";
		cout << "-------------------------------\n";
	}
	zawiadowca();
}

//kod USERPROG
void Nadzorca::CUSERPROG(){
	string dane;
	int pobrane;
	//Gotowosc do wykonywania procesow
	cout << "Gotowe\n";
	while (1){
		cout << "\n" << "------------------------------------------------\n";
		cout << "Opcje:run,load,view,pam,view_on,usun,znajdz\n";
		cout << "------------------------------------------------\n";
		//Pobranie komendy
		getline(cin, dane);
		if (dane == "run" || dane == ""){
			int i = 0;
			do{
				//Ustawienie odpowiedniej wartosci RUNNING
				string name = RUNNING->getName();
				if (name == "*IBSUP" || name == "Proces_bezczynnosci"
					|| name == "*IN" || name == "*OUT" 
					|| name.find("_OUT", 0) != string::npos 
					|| name.find("_IN", 0) != string::npos
					|| name == "USERPROG")
					zawiadowca();
				i++;
				if ((name != "*IBSUP" && name != "Proces_bezczynnosci"
					&& name != "*IN" && name != "*OUT" && name != "USERPROG" 
					&&name.find("_OUT", 0) != string::npos&& name.find("_IN", 0) != string::npos) || i>10)
					break;

			} while (true);
			//Wykonanie operacji
			if(Wykonaj(RUNNING)==0);
		}
		else if (dane == "load")
		{
			//cout << "\nWybierz czytnik(0,1,2-oba)\n";
			////Wybor czytnika 0,1,2-0ba
			//cin >> pobrane;
			pobrane = 2;
			//Pobranie karty $JOB i wpisanie jej do pamieci
			Zal_JOB(pobrane);
		}
		else if (dane == "view" || dane == "")
		{
			//Wyswietlenie wszystkich procesow w PCB
			RUNNING->wydrukujWszystkieProcesy();

		}
		else if (dane == "pam")
		{
			naszaPamiec.displayLists();
		}
		else if (dane == "view_on" || dane == "")
		{
			//Wyswietlenie procesu aktualnie ustawionego na RUNNING
			cout << "Aktualny RUNNING: " << RUNNING->getName() << "\n";

		}
		else if (dane == "usun")
		{

			cout << "Podaj nazwe procesu: ";
			//Pobranie nazwy procesu
			cin >> dane;
			if (Usuwanie_procesow(dane)) cout << "Blad";
			getchar();
		}
		else if (dane == "znajdz")
		{
			cout << "Podaj nazwe procesu: ";
			//Pobranie nazwy procesu
			cin >> dane;
			Pcb*adr =RUNNING->szukanieProcesu((char*)dane.c_str());
			if ((adr != pierwszyProces&&dane != "*IBSUP") || (adr != drugiProces&&dane != "*IBSUP"))
				cout << "Wynik szukania: " << RUNNING->szukanieProcesu((char*)dane.c_str()) << endl;
			else
				cout << "Nie ma takiego procesu\n";
			getchar();
		}
		else if (dane == "0") break;
	}
}

//Ladowanie karty $JOB
void Nadzorca::Zal_JOB(int dr_nr){
	//Wyslanie komunikatu i wczytanie danych z odpowiedniego czytnika
	if (dr_nr == 1 || dr_nr == 2){
		if (Tworzenie_wczytywanie_dg(pierwszyProces) == 1)
		{
			cout << "Blad";
			getchar();
			return;
		}
		else
			naszaPamiec.displayPamiec();
	}
	if (dr_nr == 0 || dr_nr == 2)
	{
		if (Tworzenie_wczytywanie_dg(drugiProces) == 1)
		{
			cout << "Blad";
			getchar();
			return;
		}
		else
			naszaPamiec.displayPamiec();
	}
	zawiadowca();
}

bool Nadzorca::Tworzenie_wczytywanie_dg(Pcb*wskaznik)
{
	Interpreter interpreter;
	Pcb*nowy;
	string komunikat="READ";
	vector<Bufor>*programy;
	string kod;
	int z_in_out=0;
	int rozmiar = 0;
	wskaznik->wysylanieKomunikatu("*IN", komunikat.length(), (char*)komunikat.c_str());
	programy = Czytanie_karty(wskaznik);
	//Uruchomienie procesów
	for (int i = 0; programy->size() > i; i++){
		//if ( == nullptr)	return 1;
		z_in_out=programy->at(i).inout;
		kod=programy->at(i).rozkazy;
		rozmiar = programy->at(i).size;
		if (kod == "")	return 1;
		if (z_in_out == 3) {
			wskaznik->wysylanieKomunikatu("*OUT", kod.length(), (char*)kod.c_str());
			Drukowanie_komunikatow(wskaznik);
			return 0;
		}
		//Utworzenie USERPROG
		if (wskaznik->szukanieProcesu("USERPROG") == wskaznik)
		{
			wskaznik->tworzenieProcesu("USERPROG", 0);
			if (IBSUP_ERR(wskaznik)) return 1;
		}
		char*nazwap_procesu = (char*)programy->at(i).nazwa.c_str();
		interpreter.interpret_code(kod);
		//Tworzenie odpowiednich procesow w odowiedniej grupie
		wskaznik->tworzenieProcesu(nazwap_procesu, rozmiar);
		if (IBSUP_ERR(wskaznik)) return 1;
		if (z_in_out == 2){
			wskaznik->wysylanieKomunikatu(nazwap_procesu, 7, "Czytaj");
		}
		if (z_in_out == 1){
			wskaznik->wysylanieKomunikatu(nazwap_procesu, 7, "D_C");
		}
		nazwa_in = new string;
		nazwa_in->append(nazwap_procesu);
		nazwa_in->append("_IN");
		nazwa_out = new string;
		nazwa_out->append(nazwap_procesu);
		nazwa_out->append("_OUT");
		wskaznik->tworzenieProcesu((char*)nazwa_in->c_str(), 0);
		if (IBSUP_ERR(wskaznik)) return 1;
		wskaznik->tworzenieProcesu((char*)nazwa_out->c_str(), 0);
		if (IBSUP_ERR(wskaznik)) return 1;
		//wskaznik->zatrzymywanieProcesu(wskaznik->getName());
		nowy = wskaznik->szukanieProcesu(nazwap_procesu);
		//Wpisywabnie kodu programu do pamieci
		int j = nowy->auto_storage_adress;//adres poczatku programu
		char tmp;//wartosc wpisywanego akurat bajta
		for (int i = 0; (i) < rozmiar; i++){
			tmp = interpreter.buffer[i];
			if (!naszaPamiec.setByte(j, i, tmp)){
				cout << "Pisanie do bajtu " << j + i << " \tnie powiodlo sie" << endl;
				return 1;
			}
			else {
				cout << "Pisanie do bajtu:" << j + i << " \t:";
				if ((int)tmp < 10) cout << (int)tmp << endl;
				else  cout << (char)tmp << endl;
			}
		}
		getchar();
	}
	wskaznik->zatrzymywanieProcesu("*IBSUP");
	wskaznik->uruchomienieProcesu("USERPROG");
	for (int i = 0; programy->size() > i; i++)
		wskaznik->uruchomienieProcesu((char*)programy->at(i).nazwa.c_str());
	return 0;
}

//Wykonywanie rozkazow
int Nadzorca::Wykonaj(Pcb*proces){
	//gdy proces wychodzi poza wyznaczzona pamiec
	int st = proces->getAutoStorageSize() + proces->auto_storage_adress;
	if (st <= proces->mem_pointer + proces->auto_storage_adress)
	{
		cout << "Koniec pamieci\n"; return 1;
	}
	//wczytanie nr operacji
	Interpreter::OpCode op = (Interpreter::OpCode)naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++);
	char a = (char)op;

	string abc;
	switch (a){
	case 'S':abc = "SET"; break;
	case 'A':abc = "ADD"; break;
	case 'U':abc = "SUB"; break;
	case 'M':abc = "MUL"; break;
	case 'D':abc = "DIV"; break;
	case 'J':abc = "JUMP"; break;
	case 'N':abc = "JMPZ"; break;
	case 'Z':abc = "JPNZ"; break;
	case 'O':abc = "OUT"; break;
	case 'E':abc = "BYE"; break;
	}
	cout << "Proces " << proces->getName() << " wykonuje rozkaz = " << abc << endl;

	//do odczytania parametru(test albo wartosc rejestru)
	char* raw_param = NULL;
	int przekarz;
	string tmp;
	//wartosc parametru-int_param,len-do ustalania dl przy skokach i out, reg-nr rejestru
	int reg1,reg2, len, int_param;
	bool reg_to_reg=0;
	//przelaczenie na odpowiednia operacje
	switch (op) {
	case Interpreter::OpCode::SET:
	case Interpreter::OpCode::ADD:
	case Interpreter::OpCode::SUB:
	case Interpreter::OpCode::MUL:
	case Interpreter::OpCode::DIV:
	{
	reg1 = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 64;
	if (reg1<0 && reg1>4) return 1;
	cout << " ";
	if (reg1 == 1)cout << "A";
	else if (reg1 == 2) cout << "B";
	else if (reg1 == 3) cout << "C";
	else if (reg1 == 4) cout << "D";

	if ((naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48) < 10 && (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48)>-1)
	{
		int_param = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		while ((naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48) < 10 && (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48)>-1){
			int_param = int_param * 10;
			int_param += naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		}
		cout << ":" << int_param << endl;
	}
	else
	{
		cout << ":";
		reg_to_reg = 1;
		reg2 = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 64;
		if (reg2 == 1)cout << "A\n";
		else if (reg2 == 2) cout << "B\n";
		else if (reg2 == 3) cout << "C\n";
		else if (reg2 == 4) cout << "D\n";
	}	
	}
		break;
	case Interpreter::OpCode::JUMP:
		int_param = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		while ((naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48) < 10 && (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48)>-1){
			int_param = int_param * 10;
			int_param += naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		}
		int_param++;
		cout << int_param+proces->auto_storage_adress << endl;
		break;
	case Interpreter::OpCode::JMPZ:
	case Interpreter::OpCode::JPNZ:
		reg2 = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 64;
		int_param = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		while ((naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48) < 10 && (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48)>-1){
			int_param = int_param * 10;
			int_param += naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		}
		cout << int_param + proces->auto_storage_adress << endl;
		break;
	case Interpreter::OpCode::OUT:
		if (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) == '0')
		{
			proces->mem_pointer++;
			len = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++);
			raw_param = new char[len + 1];
			for (int i = 0; i < len + 1; i++)
			{
				raw_param[i] = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++);
			}
		}
		else{
			reg1 = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 64;
			przekarz=rejestr.Przekaz_w_rejestru(reg1);
		}
		
		if (raw_param != NULL)
			tmp = raw_param;
		else
			tmp = to_string(przekarz);
		string tmp2 = RUNNING->getName();
		tmp2.append("_OUT");
		//Wyslanie komunikatu z trescia do druku
		proces->wysylanieKomunikatu((char*)tmp2.c_str(), tmp.length(), (char*)tmp.c_str());
		Przekazywanie_komunikatow((char*)tmp2.c_str());
		break;
	}
	Pcb *wskaznikProcesu = proces;
	switch (op) {
	case Interpreter::OpCode::SET:
		if (reg_to_reg==1) rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg2));
		else rejestr.Ustaw_w_rejestru(reg1, int_param);
		break;
	case Interpreter::OpCode::ADD:
		if (reg_to_reg == 1) rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg2) + rejestr.Przekaz_w_rejestru(reg1));
		else rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) + int_param);
		break;
	case Interpreter::OpCode::SUB:
		if (reg_to_reg == 1) rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg2) - rejestr.Przekaz_w_rejestru(reg1));
		else rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) - int_param);
		break;
	case Interpreter::OpCode::MUL:
		if (reg_to_reg == 1) rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg2) * rejestr.Przekaz_w_rejestru(reg1));
		else rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) * int_param);
		break;
	case Interpreter::OpCode::DIV:
		if (reg_to_reg == 1) rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg2) / rejestr.Przekaz_w_rejestru(reg1));
		else rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) / int_param);
		break;
	case Interpreter::OpCode::JUMP:
		proces->mem_pointer = int_param;
		cout << "JUMP " << int_param + proces->auto_storage_adress << endl;
		break;
	case Interpreter::OpCode::JMPZ:
		if (rejestr.Przekaz_w_rejestru(reg2) == 0) {
			proces->mem_pointer = int_param;
			cout << "JMPZ " << int_param + proces->auto_storage_adress << endl;
		}
		else {
			cout << "JMPZ " << int_param + proces->auto_storage_adress << " (zignorowany)" << endl;
		}
		break;
	case Interpreter::OpCode::JPNZ:
		if (rejestr.Przekaz_w_rejestru(reg2) != 0) {
			proces->mem_pointer = int_param;
			cout << "JPNZ " << int_param + proces->auto_storage_adress << endl;
		}
		else {
			cout << "JPNZ " << int_param + proces->auto_storage_adress << " (zignorowany)" << endl;
		}
		break;
	case Interpreter::OpCode::OUT:
		cout << "OUT: " << tmp << endl;
		break;
	case Interpreter::OpCode::BYE:
		cout << " BYE";
		FIN_procesu(proces);
		break;
	default:
		cout << "Nieprawidlowy rozkaz" << endl;
		return 1;
		if (raw_param){
			delete[] raw_param;
		}
	}
	return 0;
}

void Nadzorca::FIN_procesu(Pcb*proces){
	Pcb*proces2;
	string abc = proces->getName();
	string*message=new string;
	message->append("D_C");
	cout << "\n" << "------------------------------------------------\n";
	cout << "BYE\nKoniec procesu\n";
	string*tma=proces->czytanieKomunikatu();
	if (*tma == *message){
		cout << "Drukowanie komunikatow\n";
		proces2 = proces->szukanieProcesu("*OUT");
		int licz = proces2->message_semaphore_receiver.GET_VALUE();
		for (int i = 0; i < licz; i++)
			Drukowanie_komunikatow(proces);
	}
	cout << "\n" << "------------------------------------------------\n";
	cout << "BYE\nUsuwanie procesu" << endl << abc;
	if (Usuwanie_procesow(abc) != 0) cout << "Blad";
	cout << "Proces usuniety\n";
	delete message;
	if (RUNNING->liczenieProcesu() == 4)
	{
		RUNNING->usuniecieProcesu("USERPROG");
		RUNNING->uruchomienieProcesu("*IBSUB");
	}
	zawiadowca();
}

//Zamykanie systemu
void Nadzorca::FIN(){
	Pcb *wskaznikProcesu = RUNNING;
	string a = wskaznikProcesu->getName();
	do
	{
		a = wskaznikProcesu->getName();
		if (wskaznikProcesu != nullptr)
		{
			if (a == "USERPROG")break;
			wskaznikProcesu = wskaznikProcesu->getLastAll();
			wskaznikProcesu->usuniecieProcesu((char*)a.c_str());
			cout << "Usunieto proces " << a << "\n";

		}
	} while (1);

	pierwszyProces->usuniecieProcesu("USERPROG");
	cout << "Usuwanie procesow systemowych\n";
	for (int i = 0; i < 3; i++)
	{
		pierwszyProces->usuniecieProcesu((char*)tab_sys[i].c_str());
		drugiProces->usuniecieProcesu((char*)tab_sys[i].c_str());
		cout << "Usunieto proces" << tab_sys[i] << "\n";
	}
	RUNNING->wydrukujWszystkieProcesy();
} 

//Odczytanie komunikatu i pobranie dancyh z czytnika
vector<Bufor>* Nadzorca::Czytanie_karty(Pcb*wsk){
	Czyt*data = new Czyt; 
	string *message;
	vector<Bufor>*bufor = new vector<Bufor>();
	if (wsk == pierwszyProces){
		message = Czytanie_kom(wsk,true);
		if (message != nullptr)
			*bufor = data->Czytaj(true, *message);
	}
	else if (wsk == drugiProces)
	{
		message = Czytanie_kom(wsk,true);
		if (message != nullptr)
			*bufor = data->Czytaj(false, *message);

	}
	do{
		zawiadowca();
	} while (RUNNING != wsk->szukanieProcesu("IBSUB"));
	delete data;
	return bufor;
}

//Odczytanie komunikatu i wyslanie dancyh do drukarki
void Nadzorca::Drukowanie_komunikatow(Pcb*pr){
	Druk*drukarka = new Druk;
	Pcb *wsk = RUNNING->szukanieProcesu("*OUT");
	if (wsk->message_semaphore_receiver.GET_VALUE() < 1) return;
	if (wsk != RUNNING){
		wsk->uruchomienieProcesu("*OUT");
		do{			
			zawiadowca();
		} while (wsk != RUNNING);
	}
	string *message = wsk->czytanieKomunikatu();
	if (*(RUNNING->firstPcb) == pierwszyProces)
		drukarka->Drukuj((char*)message->c_str(), "drukarka1", "PRIN");
	else if (*(RUNNING->firstPcb) == drugiProces)
		drukarka->Drukuj((char*)message->c_str(), "drukarka2", "PRIN");
	wsk->zatrzymywanieProcesu("*OUT");
	do{
		zawiadowca();
	} while (RUNNING != pr->szukanieProcesu(pr->getName()));
}

void Nadzorca::Przekazywanie_komunikatow(char*proces){
	Pcb *wskaznikNaProces = RUNNING->szukanieProcesu(proces);
	wskaznikNaProces->uruchomienieProcesu(proces);
	if (wskaznikNaProces != RUNNING){
		do{
			zawiadowca();
		} while (wskaznikNaProces != RUNNING);
	}
	string *message = wskaznikNaProces->czytanieKomunikatu();
	message->append(" ");
	message->append(wskaznikNaProces->getName());
	RUNNING->wysylanieKomunikatu("*OUT", message->length(), (char*)message->c_str());
	wskaznikNaProces->zatrzymywanieProcesu(proces);
		zawiadowca();
}

//Sprawdza czy nie ma komunikatu bledu
bool Nadzorca::IBSUP_ERR(Pcb *wskaznik){
	Druk*drukarka = new Druk;
	string *message;
	message = wskaznik->czytanieKomunikatu();
	if (message != nullptr){
		if (*message == "Wystapil blad")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;

}

bool Nadzorca::Usuwanie_procesow(string dane){
	if (dane != "*IBSUP" || dane != "Proces_bezczynnosci"
		|| dane != "*IN" || dane != "*OUT"){
		nazwa_in = new string;
		nazwa_out = new string;
		nazwa_in->append(dane);
		nazwa_in->append("_IN");
		nazwa_out->append(dane);
		nazwa_out->append("_OUT");
		if (RUNNING->szukanieProcesu((char*)dane.c_str()) == RUNNING->szukanieProcesu("*IBSUP")) return 1;
		if (RUNNING == RUNNING->szukanieProcesu((char*)dane.c_str())) RUNNING = RUNNING->szukanieProcesu("*IBSUP");
		RUNNING->usuniecieProcesu((char*)dane.c_str());
		cout << "-------------------------------\n";
		RUNNING->usuniecieProcesu((char*)nazwa_in->c_str());
		cout << "-------------------------------\n";
		RUNNING->usuniecieProcesu((char*)nazwa_out->c_str());
		cout << "-------------------------------\n";
		return 0;
	}
	else
	{
		cout << "Nastapila proba usuniecia procesu systemowego";
		return 1;
	}
}

string*Nadzorca::Czytanie_kom(Pcb*wsk,bool IB_on){
	string*message;
	wsk = wsk->szukanieProcesu("*IN");
	if (wsk != RUNNING){
		wsk->uruchomienieProcesu("*IN");
		do{			
			zawiadowca();
		} while (wsk != RUNNING);
	}
		message = wsk->czytanieKomunikatu();
		wsk->zatrzymywanieProcesu("*IN");
		if (IB_on=true)
		do{
			zawiadowca();
		} while (RUNNING != wsk->szukanieProcesu("IBSUB"));
return message;
}