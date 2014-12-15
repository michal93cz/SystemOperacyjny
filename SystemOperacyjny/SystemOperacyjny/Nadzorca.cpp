#include "Nadzorca.h"


void Nadzorca::INIT(){
	cout << "-------------------------------\n";
	cout << "Tworzenie procesow systemowych\n";
	cout << "-------------------------------\n";
	for (int i = 0; i < 3; i++)
	{
		pierwszyProces->tworzenieProcesu((char*)tab_sys[i].c_str(), 1);
		//pierwszyProces->uruchomienieProcesu((char*)tab_sys[i].c_str());
		RUNNING = drugiProces;
		NEXTTRY = pierwszyProces;
		drugiProces->tworzenieProcesu((char*)tab_sys[i].c_str(), 1);
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
		cout << "Opcje:run,load,view,pam,view_on,usun,znajdz,druk\n";
		cout << "------------------------------------------------\n";
		//Pobranie komendy
		getline(cin, dane);
		if (dane == "run" || dane == ""){
			int i = 0;
			do{
				//Ustawienie odpowiedniej wartosci RUNNING
				string name = RUNNING->getName();
				if (name == "*IBSUP" || name == "Proces_bezczynnosci"
					|| name == "*IN" || name == "*OUT")
					zawiadowca();
				i++;
				if ((name != "*IBSUP" && name != "Proces_bezczynnosci"
					&& name != "*IN" && name != "*OUT") || i > 1)
					break;

			} while (true);
			//Wykonanie operacji
			Wykonaj(RUNNING);
			rejestr.display_reg();
		}
		else if (dane == "load")
		{
			cout << "\nWybierz czytnik(0,1,2-oba)\n";
			//Wybor czytnika 0,1,2-0ba
			cin >> pobrane;
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
			Usuwanie_procesow(dane);
		}
		else if (dane == "znajdz")
		{
			cout << "Podaj nazwe procesu: ";
			//Pobranie nazwy procesu
			cin >> dane;
			cout << "Wynik szukania: " << RUNNING->szukanieProcesu((char*)dane.c_str()) << endl;
		}
		else if (dane == "druk")
		{
			Drukowanie_komunikatow();
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
	}
	if (dr_nr == 0 || dr_nr == 2)
	{
		if (Tworzenie_wczytywanie_dg(drugiProces) == 1)
		{
			cout << "Blad";
			getchar();
			return;
		}
	}
	zawiadowca();
	naszaPamiec.displayPamiec();
	getchar();
}

bool Nadzorca::Tworzenie_wczytywanie_dg(Pcb*wskaznik)
{
	Czyt*data = new Czyt;
	Interpreter interpreter;
	Pcb*nowy;
	nazwap_procesu = new string;
	*nazwap_procesu = "READ";
	string kod;
	int rozmiar = 1;
	wskaznik->wysylanieKomunikatu("*IN", nazwap_procesu->length(), (char*)nazwap_procesu->c_str());
	nazwap_procesu = Czytanie_komunikatow(kod, rozmiar, wskaznik);
	if (nazwap_procesu == nullptr)	return 1;
	//Utworzenie USERPROG
	wskaznik->tworzenieProcesu("USERPROG", 1);
	//gdy nie ma kodu
	interpreter.interpret_code(kod);
	if (kod == "")	return 1;
	//Tworzenie odpowiednich procesow w odowiedniej grupie
	wskaznik->tworzenieProcesu((char*)nazwap_procesu->c_str(), rozmiar);
	if (IBSUP_ERR()) return 1;
	wskaznik->zatrzymywanieProcesu(wskaznik->getName());
	nowy = wskaznik->szukanieProcesu((char*)nazwap_procesu->c_str());
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
			if ((int)tmp<10) cout << (int)tmp << endl;
			else  cout << (char)tmp << endl;
		}
	}
	//Uruchomienie procesu
	nowy->uruchomienieProcesu(nowy->getName());
	return 0;
}

//Wykonywanie rozkazow
int Nadzorca::Wykonaj(Pcb*proces){
	//gdy proces wychodzi poza wyznaczzona pamiec
	int st = proces->getAutoStorageSize() + proces->auto_storage_adress;
	if (st <= proces->mem_pointer + proces->auto_storage_adress)
	{
		cout << "Koniec pamieci\n"; return 0;
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
	cout << "Proces " << proces->getName() << " wykonuje rozkaz : ID = " << (char)op << " " << abc << endl;

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

	if ((naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48) < 10 && (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48)>0)
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
		if (reg1 == 1)cout << "A\n";
		else if (reg1 == 2) cout << "B\n";
		else if (reg1 == 3) cout << "C\n";
		else if (reg1 == 4) cout << "D\n";
	}	
	}
		break;
	case Interpreter::OpCode::JUMP:
		int_param = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		while ((naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48) < 10 && (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48)>0){
			int_param = int_param * 10;
			int_param += naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		}
		cout << ":" << int_param+proces->auto_storage_adress << endl;
		break;
	case Interpreter::OpCode::JMPZ:
	case Interpreter::OpCode::JPNZ:
		reg2 = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 64;
		int_param = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		while ((naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48) < 10 && (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) - 48)>0){
			int_param = int_param * 10;
			int_param += naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 48;
		}
		cout << ":" << int_param << endl;
		break;
	case Interpreter::OpCode::OUT:
		if (naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer) == '0')
		{
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
		//Wyslanie komunikatu z trescia do druku
		Pcb*wskaznik = RUNNING->szukanieProcesu("*OUT");
		string tmp1 = wskaznik->getName();
		wskaznik = RUNNING;
		RUNNING->uruchomienieProcesu((char*)tmp1.c_str());
		if (raw_param != NULL)
			tmp = raw_param;
		else
			tmp = to_string(przekarz);
		RUNNING->wysylanieKomunikatu("*OUT", tmp.length(), (char*)tmp.c_str());
		RUNNING->zatrzymywanieProcesu((char*)tmp1.c_str());
		RUNNING->uruchomienieProcesu(wskaznik->getName());
		wskaznik->resetBlocked();
		zawiadowca();
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
		proces->mem_pointer = int_param - 1;
		cout << "JUMP " << int_param + proces->auto_storage_adress << endl;
		break;
	case Interpreter::OpCode::JMPZ:
		if (rejestr.Przekaz_w_rejestru(reg2) == 0) {
			proces->mem_pointer = int_param - 1;
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
	string abc = proces->getName();
	proces->zatrzymywanieProcesu((char*)abc.c_str());
	cout << "\n" << "------------------------------------------------\n";
	cout << "BYE\nKoniec procesu\nDrukowanie wynikow\n" << endl;
	Drukowanie_komunikatow();
	cout << "\n" << "------------------------------------------------\n";
	cout << "BYE\nUsuwanie procesu" << endl << abc;
	if (Usuwanie_procesow(abc) != 0) cout << "Blad";
	cout << "Proces usuniety";
	RUNNING->zatrzymywanieProcesu("Proces_bezczynnosci");
	RUNNING->uruchomienieProcesu("*IBSUP");
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
string* Nadzorca::Czytanie_komunikatow(string&rozkazy, int&rozmiar, Pcb*wsk){
	Czyt*data = new Czyt;
	string *message;
	if (RUNNING != wsk->szukanieProcesu("*IBSUP")) wsk->uruchomienieProcesu("*IBSUP");
	if (wsk == pierwszyProces){
		Pcb *wskaznikNaProces = pierwszyProces->szukanieProcesu("*IN");
		message = wskaznikNaProces->czytanieKomunikatu();
		if (message != nullptr)
			rozkazy = data->Czytaj(*message, true, *message,rozmiar);
	}
	if (wsk == drugiProces)
	{
		Pcb *wskaznikNaProces = drugiProces->szukanieProcesu("*IN");
		message = wskaznikNaProces->czytanieKomunikatu();
		if (message != nullptr)
			rozkazy = data->Czytaj(*message, false, *message, rozmiar);

	}
	return message;
}

//Odczytanie komunikatu i wyslanie dancyh do drukarki
void Nadzorca::Drukowanie_komunikatow(){
	Druk*drukarka = new Druk;
	Pcb *wskaznikNaProces = RUNNING->szukanieProcesu("*OUT");
	if (wskaznikNaProces->message_semaphore_receiver.GET_VALUE() < 1) return;
	RUNNING->uruchomienieProcesu("*OUT");
	string *message = wskaznikNaProces->czytanieKomunikatu();
	if (*(RUNNING->firstPcb) == pierwszyProces)
		drukarka->Drukuj((char*)message->c_str(), "drukarka1", "PRIN");
	if (*(RUNNING->firstPcb) == drugiProces)
		drukarka->Drukuj((char*)message->c_str(), "drukarka2", "PRIN");
	RUNNING->zatrzymywanieProcesu("*OUT");
}

//Sprawdza czy nie ma komunikatu bledu
bool Nadzorca::IBSUP_ERR(){
	Druk*drukarka = new Druk;
	Pcb *wskaznikNaProces = RUNNING->szukanieProcesu("*IBSUP");
	string *message;
	message = wskaznikNaProces->czytanieKomunikatu();
	if (message != nullptr){
		if (*message == "ERR")
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
	if (NEXTTRY->getBlocked() == 1 || NEXTTRY->getStopped() == 1) //gdy nastepny blok nie jest w stanie gotowosci
		RUNNING->uruchomienieProcesu("Proces_bezczynnosci");
	RUNNING->zatrzymywanieProcesu((char*)dane.c_str());
	zawiadowca();
	RUNNING->usuniecieProcesu((char*)dane.c_str());
	return 0;
}