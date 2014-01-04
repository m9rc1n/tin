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
	// Serwer nie odpowiedział w zadanym czasie
	CONNECTION_TIMED_OUT = 110,
	// -----------------------------------------------
	// Odmowa połączenia
	// Serwer odmówił połączenia z bazą
	CONNECTION_REFUSED = 111,

	// Kody powodzenia
	// ===============================================
	// OK
	// Operacja zakończona powodzeniem
	OK = 200,
	// -----------------------------------------------
	// Utworzony
	// Plik został poprawnie utworzony na serwerze
	CREATED = 201,
	// -----------------------------------------------
	// Zapisany
	// Plik został poprawnie zapisany na serwerze
	SAVED = 202,
	// -----------------------------------------------
	// Zaakceptowanie
	// Serwer zaakceptował połączenie z klientem 
	ACCEPTED = 203,
	// -----------------------------------------------
	// Zamknięcie połączenia
	// Serwer zamknął połączenie z klientem
	CLOSED = 204,

	// Kody przekierowania
	// ===============================================
	// Niezmodyfikowany
	// Dany plik nie został zmodyfikowany na serwerze
	NOT_MODIFIED = 300,

	// Kody błędu aplikacji klienta
	// ===============================================
	// Nieprawidłowe zapytanie 
	// żądanie nie może być obsłużone przez serwer 
	// z powodu błędnej składni zapytania
	BAD_REQUEST = 400,
	// -----------------------------------------------
	// Nie znaleziono
	// serwer nie odnalazł zasobu według podanego wzorca
	NOT_FOUND = 401,
	// -----------------------------------------------
	// Niedozwolone
	// zażądany zasób nie jest w stanie zwrócić 
	// odpowiedzi mogącej być obsłużonej przez klienta 
	// według informacji podanych w zapytaniu
	NOT_ACCETABLE = 402,
	// -----------------------------------------------
	// Blokada na pliku
	// Konflikt z obecnym statusem zasobu na serwerze. 
	// Zwracany gdy klient nie może uzyskać dostępu 
	// do zablokowanego pliku 
	FILE_BLOCKED = 403,
	// -----------------------------------------------
	// Plik istnieje już na serwerze
	// Podany plik znajduje się już w danym katalogu 
	// na serwerze
	ALREADY_EXIST = 404,
	// -----------------------------------------------
	// Jest katalogiem
	// Podany plik jest katalogiem
	IS_DIR = 405,

	// Kody błędów wewnętrznych
	// ===============================================
	// Wewnętrzny błąd serwera
	// Serwer napotkał niespodziewane trudności
	// które uniemożliwiły zrealizowanie żądania
	INTERNAL_SERVER_ERROR = 500,
	// ----------------------------------------------
	// Nie zaimplementowano
	// Serwer nie dysponuje funkcjonalnością wymaganą 
	// w zapytaniu ten kod jest zwracany gdy serwer 
	// otrzymał nieznany typ zapytania
	NOT_IMPLEMENTED = 501
}
FsAnswer;

typedef struct FsOpenServerC
{
	/* adres serwera */
	char server_address[16];
} 
FsOpenServerC;

typedef struct FsOpenServerS
{
	// komunikat − odpowiedz serwera
	FsAnswer answer;
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

typedef struct FsCloseServerS
{
	// komunikat − odpowiedz serwera
	FsAnswer answer;
} 
FsCloseServerS;

#endif // PROTOCOL_H_INCLUDED
