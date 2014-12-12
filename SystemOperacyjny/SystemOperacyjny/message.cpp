//POZIOM 3

#include "message.h"

Message::Message(Pcb *wskaznikProcesu, int rozmiar, string tekst)
{
	size = rozmiar;
	text = tekst;
	sender = wskaznikProcesu;
	next = nullptr;
}

void Message::umieszczenieNaKoncu(Pcb *wskaznikProcesu)
{
	Message *wskaznikWiadomosci = wskaznikProcesu->getFirstMessage();
	if (wskaznikWiadomosci != nullptr)
	{
		while (wskaznikWiadomosci->next != nullptr)
		{
			wskaznikWiadomosci = wskaznikWiadomosci->next;
		}
		wskaznikWiadomosci->next = this;
	}
	else
	{
		wskaznikProcesu->setFirstMessage(this);
	}
}

string Message::pobraniePierwszego(Pcb *wskaznikProcesu)
{
	wskaznikProcesu->setFirstMessage(wskaznikProcesu->getFirstMessage()->next);
	return wskaznikProcesu->getFirstMessage()->text;
}