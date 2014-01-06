#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED


typedef enum FsCommand
{
	// Polacz sie z serwerem
	OPEN_SERVER = 1,
	// Rozlacz sie z serwerem
	CLOSE_SERVER = 2,
	// Otworz plik
	OPEN = 3,
	// Otworz plik tylko do odczytu
	OPEN_ORDONLY = 31,
	// Otworz plik tylko do zapisu
	OPEN_WRONLY = 32,
	// Otworz plik do zapisu i odczytu
	OPEN_RDWR = 33,
	// Utworz plik */
	OPEN_CREAT = 34,
	// Otworz plik z zapisem na koniec pliku
	OPEN_APPEND = 35,
	// Zapisz do pliku
	WRITE = 4,
	// Odczytaj plik
	READ = 5,
	// Polecenie zmiany miejsca w ktorym
	// zostanie wykonana kolejna operacja
	LSEEK = 6,
	// Zamknij plik
	CLOSE = 7,
	// Przeslanie informacji o pliku
	STAT = 8,
	// Zablokowanie pliku na serwerze
	LOCK = 9,
	// Przeslij ponownie plik
	AGAIN = 10
}
FsCommand;

typedef enum FsAnswer
{
	// Kody informacyjne
	// ===============================================
	// Kontynuacja
	// Serwer czeka na polecenia Klienta
	CONTINUE = 100,
	// -----------------------------------------------
	// Przekroczenie czasu
	// Serwer nie odpowiedzial w zadanym czasie
	CONNECTION_TIMED_OUT = 110,
	// -----------------------------------------------
	// Odmowa polaczenia
	// Serwer odmowil polaczenia z baza
	CONNECTION_REFUSED = 111,

	// Kody powodzenia
	// ===============================================
	// OK
	// Operacja zakonczona powodzeniem
	OK = 200,
	// -----------------------------------------------
	// Utworzony
	// Plik zostal poprawnie utworzony na serwerze
	CREATED = 201,
	// -----------------------------------------------
	// Zapisany
	// Plik zostal poprawnie zapisany na serwerze
	SAVED = 202,
	// -----------------------------------------------
	// Zaakceptowanie
	// Serwer zaakceptowal polaczenie z klientem
	ACCEPTED = 203,
	// -----------------------------------------------
	// Zamkniecie polaczenia
	// Serwer zamknal polaczenie z klientem
	CLOSED = 204,

	// Kody przekierowania
	// ===============================================
	// Niezmodyfikowany
	// Dany plik nie zostal zmodyfikowany na serwerze
	NOT_MODIFIED = 300,

	// Kody bledu aplikacji klienta
	// ===============================================
	// Nieprawidlowe zapytanie
	// żadanie nie może być obslużone przez serwer
	// z powodu blednej skladni zapytania
	BAD_REQUEST = 400,
	// -----------------------------------------------
	// Nie znaleziono
	// serwer nie odnalazl zasobu wedlug podanego wzorca
	NOT_FOUND = 401,
	// -----------------------------------------------
	// Niedozwolone
	// zażadany zasob nie jest w stanie zwrocić
	// odpowiedzi mogacej być obslużonej przez klienta
	// wedlug informacji podanych w zapytaniu
	NOT_ACCETABLE = 402,
	// -----------------------------------------------
	// Blokada na pliku
	// Konflikt z obecnym statusem zasobu na serwerze.
	// Zwracany gdy klient nie może uzyskać dostepu
	// do zablokowanego pliku
	FILE_BLOCKED = 403,
	// -----------------------------------------------
	// Plik istnieje już na serwerze
	// Podany plik znajduje sie już w danym katalogu
	// na serwerze
	ALREADY_EXIST = 404,
	// -----------------------------------------------
	// Jest katalogiem
	// Podany plik jest katalogiem
	IS_DIR = 405,

	// Kody bledow wewnetrznych
	// ===============================================
	// Wewnetrzny blad serwera
	// Serwer napotkal niespodziewane trudnosci
	// ktore uniemożliwily zrealizowanie żadania
	INTERNAL_SERVER_ERROR = 500,
	// ----------------------------------------------
	// Nie zaimplementowano
	// Serwer nie dysponuje funkcjonalnoscia wymagana
	// w zapytaniu ten kod jest zwracany gdy serwer
	// otrzymal nieznany typ zapytania
	NOT_IMPLEMENTED = 501
}
FsAnswer;

typedef struct FsOpenServerC
{
	// adres serwera
	char server_address[16];
}
FsOpenServerC;

typedef struct FsOpenServerS
{
	// uchwyt do serwera
	int server_handler;
}
FsOpenServerS;

typedef struct FsCloseServerC
{
	// uchwyt do serwera
	int server_handler;
}
FsCloseServerC;

// narazie nie usuwam, pozniej tak
typedef struct FsCloseServerS
{
	// komunikat − odpowiedz serwera
	FsAnswer answer;
}
FsCloseServerS;

typedef struct FsRequest
{
	// komenda wysylana do serwera
    FsCommand command;
    
    union RequestData 
    {
        FsOpenServerC open_server_c;
		FsCloseServerC close_server_c;
    } 
	request_data;
} 
FsRequest;

typedef struct FsResponse
{
	// komunikat − odpowiedz serwera
    FsAnswer answer;
    
    union ResponseData
    {
        FsOpenServerS open_server_s;
		FsCloseServerS close_server_s;
    } 
	response_data;
} 
FsResponse;

#endif // PROTOCOL_H_INCLUDED
