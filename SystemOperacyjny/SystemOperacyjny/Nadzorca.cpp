#include "Nadzorca.h"


void Nadzorca::INIT(){
	cout << "-------------------------------\n";
	cout << "Utworzenie procesow systemowych\n";
	cout << "-------------------------------\n";
	for (int i = 0; i < 3; i++)
	{
		pierwszyProces->tworzenieProcesu((char*)tab_sys[i].c_str(), 1);
		drugiProces->tworzenieProcesu((char*)tab_sys[i].c_str(), 1);
		cout << "Utworzono proces" << tab_sys[i]<<"\n";
	}

	cout << "Wlaczenie zawiadowcy\n";
	zawiadowca();
}

//kod USERPROG
void Nadzorca::CUSERPROG(){
	string dane;
	bool pobrane;
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
			cout << "\nWybierz czytnik(1,0)\n";
			//Wybor czytnika 0,1
			cin >> pobrane;
			//Pobranie karty $JOB i wpisanie jej do pamieci
			if (Zal_JOB(pobrane) == 1) getchar();
			
		}
		else if (dane == "view")
		{
			//Wyswietlenie wszystkich procesow w PCB
			RUNNING->wydrukujWszystkieProcesy();

		}
		else if (dane == "pam")
		{
			naszaPamiec.displayLists();
		}
		else if(dane == "view_on")
		{
			//Wyswietlenie wszystkich procesow w PCB
			cout<<RUNNING->getName();

		}
		else if (dane == "view_on")
		{
			//Wyswietlenie procesu aktualnie ustawionego na RUNNING
			cout << "Aktualny RUNNING: " << RUNNING->getName() << "\n";

		}
		else if (dane == "usun")
		{
			cout << "Podaj nazwe procesu: ";
			//Pobranie nazwy procesu
			cin >> dane;
			if (NEXTTRY->getBlocked() == 1 || NEXTTRY->getStopped() == 1) //gdy nastepny blok nie jest w stanie gotowosci
				RUNNING->uruchomienieProcesu("Proces_bezczynnosci");
			RUNNING->zatrzymywanieProcesu((char*)dane.c_str());
			zawiadowca();
			RUNNING->usuniecieProcesu((char*)dane.c_str());
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
int Nadzorca::Zal_JOB(bool dr_nr){
	Czyt*data=new Czyt;
	Interpreter interpreter;
	Pcb*nowy;
	nazwap_procesu = new string;
	*nazwap_procesu = "READ";
	string kod;
	int rozmiar=1;
	//Wyslanie komunikatu i wczytanie danych z odpowiedniego czytnika
	if (dr_nr == true){
		Pcb*wskaznik = pierwszyProces->szukanieProcesu("*IN");
		string tmp1 = wskaznik->getName();
		RUNNING = *(pierwszyProces->firstPcb);
		pierwszyProces->uruchomienieProcesu((char*)tmp1.c_str());
		pierwszyProces->wysylanieKomunikatu("*IN", nazwap_procesu->length(), (char*)nazwap_procesu->c_str());
		nazwap_procesu = Czytanie_komunikatow(kod, rozmiar);
		pierwszyProces->zatrzymywanieProcesu((char*)tmp1.c_str());
		//Utworzenie USERPROG
		pierwszyProces->tworzenieProcesu("USERPROG",1);
		zawiadowca();
	}
	else if (dr_nr == false)
	{
		Pcb*wskaznik = drugiProces->szukanieProcesu("*IN");
		string tmp1 = wskaznik->getName();
		RUNNING = *(drugiProces->firstPcb);
		drugiProces->uruchomienieProcesu((char*)tmp1.c_str());
		drugiProces->wysylanieKomunikatu("*IN", nazwap_procesu->length(), (char*)nazwap_procesu->c_str());
		nazwap_procesu = Czytanie_komunikatow(kod, rozmiar);
		drugiProces->zatrzymywanieProcesu((char*)tmp1.c_str());
		//Utworzenie USERPROG
		drugiProces->tworzenieProcesu("USERPROG",1);
		zawiadowca();
	}
	//gdy nie ma kodu
	interpreter.interpret_code(kod);
	if (kod == "")
	{
		cout << "Nie ma programu";
		return 1;
	}
	rozmiar = interpreter.total_length;
	//Tworzenie odpowiednich procesow w odowiedniej grupie
	if (dr_nr == 1){
		pierwszyProces->tworzenieProcesu((char*)nazwap_procesu->c_str(), rozmiar);
		if (IBSUP_ERR()) return 1;
		nowy = pierwszyProces->szukanieProcesu((char*)nazwap_procesu->c_str());
	}
	else if (dr_nr == 0)
	{
		drugiProces->tworzenieProcesu((char*)nazwap_procesu->c_str(), rozmiar);
		if (IBSUP_ERR()) return 1;
		nowy = drugiProces->szukanieProcesu((char*)nazwap_procesu->c_str());
	}
	//Uruchomienie procesu
	string a = RUNNING->getName();
	if (a =="Proces_bezczynnosci"){
		RUNNING->setStopped();
		nowy->uruchomienieProcesu(nowy->getName());
	}
	else  nowy->uruchomienieProcesu(nowy->getName());
	//Wpisywabnie kodu programu do pamieci
	int j = nowy->auto_storage_adress;//adres poczatku programu
	char tmp;//wartosc wpisywanego akurat bajta
	for (int i = 0; (i) < rozmiar; i++){
		tmp= interpreter.buffer[i];
		if (!naszaPamiec.setByte(j, i, tmp)){
			cout << "Pisanie do bajtu " << j+i << " \tnie powiodlo sie" << endl;
			return 1;
		}
		else {
			cout << "Pisanie do bajtu:" << j+i << " \t:";
			if ((int)tmp<10) cout << (int)tmp << endl;
			else  cout << (char)tmp << endl;
		}
	}
	zawiadowca();
	naszaPamiec.displayPamiec();
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
	case 'P':abc = "JMPZ"; break;
	case 'Z':abc = "JPNZ"; break;
	case 'O':abc = "OUT"; break;
	case 'E':abc = "BYE"; break;
	}
	cout << "Proces " << proces->getName() << " wykonuje rozkaz : ID = " << (char)op << " "<<abc<<endl;
	
	//do odczytania parametru
	char* raw_param = NULL;
	//wartosc parametru-int_param,len-do ustalania dl przy skokach i out, reg-nr rejestru
	int reg1, len,int_param;
	
	//przelaczenie na odpowiednia operacje
	switch (op) {
	case Interpreter::OpCode::SET:
	case Interpreter::OpCode::ADD:
	case Interpreter::OpCode::SUB:
	case Interpreter::OpCode::MUL:
	case Interpreter::OpCode::DIV:
	{
		reg1 = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) - 64;
		cout << " "; 
		if (reg1 == 1)cout << "A";
		else if(reg1 == 2) cout << "B";
		else if (reg1 == 3) cout << "C";
		else if (reg1 == 4) cout << "D";
		
		int_param = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++) ;
		cout << ":" << int_param << endl;
		}
		break;
	case Interpreter::OpCode::JUMP:
	case Interpreter::OpCode::JMPZ:
	case Interpreter::OpCode::JPNZ:
		int_param = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++);
		break;
	case Interpreter::OpCode::OUT:
		len = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++);
		raw_param = new char[len+1];
		for (int i = 0; i < len+1; i++)
		{
			raw_param[i] = naszaPamiec.getByte(proces->auto_storage_adress, proces->mem_pointer++);
		}
		//Wyslanie komunikatu z trescia do druku
		Pcb*wskaznik = RUNNING->szukanieProcesu("*OUT");
		string tmp1=wskaznik->getName();
		wskaznik = RUNNING;
		RUNNING->uruchomienieProcesu((char*)tmp1.c_str());
		string tmp = raw_param;
		RUNNING->wysylanieKomunikatu("*OUT", tmp.length(), raw_param);
		RUNNING->zatrzymywanieProcesu((char*)tmp1.c_str());
		RUNNING->uruchomienieProcesu(wskaznik->getName());
		wskaznik->resetBlocked();
		zawiadowca();
		break;
	}
	Pcb *wskaznikProcesu = proces;
	switch (op) {
	case Interpreter::OpCode::SET:
		rejestr.Ustaw_w_rejestru(reg1, int_param);
		break;
	case Interpreter::OpCode::ADD:
		rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) + int_param);
		break;
	case Interpreter::OpCode::SUB:
		rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) - int_param);
		break;
	case Interpreter::OpCode::MUL:
		rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) * int_param);
		break;
	case Interpreter::OpCode::DIV:
		rejestr.Ustaw_w_rejestru(reg1, rejestr.Przekaz_w_rejestru(reg1) / int_param);
		break;
	case Interpreter::OpCode::JUMP:
		proces->mem_pointer = int_param - 1;
		cout << "JUMP " << int_param + proces->auto_storage_adress << endl;
		break;
	case Interpreter::OpCode::JMPZ:
		if (rejestr.Przekaz_w_rejestru(1) == 0) {
			proces->mem_pointer = int_param - 1;
			cout << "JMPZ " << int_param + proces->auto_storage_adress << endl;
		}
		else {
			cout << "JMPZ " << int_param + proces->auto_storage_adress << " (ignoring)" << endl;
		}
		break;
	case Interpreter::OpCode::JPNZ:
		if (rejestr.Przekaz_w_rejestru(1) != 0) {
			proces->mem_pointer = int_param - 1;
			cout << "JPNZ " << int_param+proces->auto_storage_adress << endl;
		}
		else {
			cout << "JPNZ " << int_param + proces->auto_storage_adress << " (zignorowany)" << endl;
		}
		break;
	case Interpreter::OpCode::OUT:
		cout << "OUT: " << raw_param << endl;
		break;
	case Interpreter::OpCode::BYE:
		cout << "\n" << "------------------------------------------------\n";
		cout << "BYE\nKoniec procesu\nDrukowanie wynikow\n" << endl;
		Drukowanie_komunikatow();
		cout << "\n" << "------------------------------------------------\n";
		cout << "BYE\nUsuwanie procesu" << endl<<proces->getName();
		if (NEXTTRY->getBlocked() == 1 || NEXTTRY->getStopped() == 1) //gdy nastepny blok nie jest w stanie gotowosci
			RUNNING->uruchomienieProcesu("Proces_bezczynnosci");
		RUNNING->zatrzymywanieProcesu(proces->getName());
		zawiadowca();
		RUNNING->usuniecieProcesu(proces->getName());
		cout << "BYE\Proces usuniety";
		zawiadowca();
		break;
	default:
		cout << "Nieprawidlowy rozkaz" << endl;
		return 1;
		if (raw_param){
			delete [] raw_param;
		}
	}
	return 0;
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
string* Nadzorca::Czytanie_komunikatow(string&rozkazy, int rozmiar){
	Czyt*data = new Czyt;
	string *message;	
	zawiadowca();
	if (*(RUNNING->firstPcb) == pierwszyProces){
		Pcb *wskaznikNaProces = pierwszyProces->szukanieProcesu("*IN");
		message = wskaznikNaProces->czytanieKomunikatu();
		rozkazy = data->Czytaj(*message, true, *message);
	}
	if (*(RUNNING->firstPcb) == drugiProces)
	{
		Pcb *wskaznikNaProces = drugiProces->szukanieProcesu("*IN");
		message = wskaznikNaProces->czytanieKomunikatu();
		rozkazy = data->Czytaj(*message, false, *message);
	}
	return message;
}

//Odczytanie komunikatu i wyslanie dancyh do drukarki
void Nadzorca::Drukowanie_komunikatow(){
	Druk*drukarka = new Druk;
	Pcb *wskaznikNaProces = RUNNING->szukanieProcesu("*OUT");
	string *message = wskaznikNaProces->czytanieKomunikatu();
	if (*(RUNNING->firstPcb) == pierwszyProces)
	drukarka->Drukuj((char*)message->c_str(), "drukarka1","PRIN");
	if (*(RUNNING->firstPcb) == drugiProces)
	drukarka->Drukuj((char*)message->c_str(), "drukarka2","PRIN");
}

//Sprawdza czy nie ma komunikatu bledu
bool Nadzorca::IBSUP_ERR(){
	Druk*drukarka = new Druk;
	Pcb *wskaznikNaProces = RUNNING->szukanieProcesu("*IBSUP");
	string *message;
	message = wskaznikNaProces->czytanieKomunikatu();
	if (message!=nullptr){
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