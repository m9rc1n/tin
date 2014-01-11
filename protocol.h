#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#include <sys/types.h>

#define BUF_LEN 200
#define NAME_LEN 32
#define MODE_LEN 3


typedef enum FsCommand
{
	// Polacz sie z serwerem
	OPEN_SERVER = 1,
	// Rozlacz sie z serwerem
	CLOSE_SERVER = 2,
	// Otworz plik
	OPEN = 3,
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
	FSTAT = 8,
	// Zablokowanie pliku na serwerze
	LOCK = 9,
    // Odbierz paczki
    RECEIVE_PACKAGES = 10
	// Przeslij ponownie plik
    // Potencjalnie do usuniecia
	// AGAIN = 10
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
    // -----------------------------------------------
    // Blad przesylania pliku
    // Plik zostal niepoprawnie przeslany
    SENDING_ERROR = 301,

	// Kody bledu aplikacji klienta
	// ===============================================
	// Nieprawidlowe zapytanie
	// Zadanie nie moze byc obsluzone przez serwer
	// z powodu blednej skladni zapytania
	BAD_REQUEST = 400,
	// -----------------------------------------------
	// Nie znaleziono
	// serwer nie odnalazl zasobu wedlug podanego wzorca
	NOT_FOUND = 401,
	// -----------------------------------------------
	// Niedozwolone
	// zazadany zasob nie jest w stanie zwrocic
	// odpowiedzi mogacej byc obsluzonej przez klienta
	// wedlug informacji podanych w zapytaniu
	NOT_ACCETABLE = 402,
	// -----------------------------------------------
	// Blokada na pliku
	// Konflikt z obecnym statusem zasobu na serwerze.
	// Zwracany gdy klient nie moze uzyskac dostepu
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
	// ktore uniemozliwily zrealizowanie zadania
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
    // liczba zwracana przez funkcje
    int status;
}
FsOpenServerS;

typedef struct FsOpenC
{
    // uchwyt do serwera
    int server_handler;
    // sciezka do pliku na serwerze
    char name[NAME_LEN];
    // rozmiar tablicy z nazwa
    size_t name_len;
    // flaga otworzenia pliku
    // O_RDONLY O_WRONLY O_RDWR O_CREAT O_APPEND
    char mode[MODE_LEN];
    // rozmiar tablicy z ustawieniami
    size_t mode_len;
}
FsOpenC;

typedef struct FsOpenS
{
    // unikalny deskryptor pliku dla danego klienta
    int fd;
    // liczba zwracana przez funkcje
    int status;
}
FsOpenS;

typedef struct FsWriteC
{
    // uchwyt do serwera
    int server_handler;
    // unikalny deskryptor pliku dla danego klienta
    int fd;
    // bufor danych do zapisania w pliku
    char buffer[BUF_LEN];
    // ilosc bajtow do zapisania
    size_t buffer_len;
    // identyfikator czesci
    int part_id;
    // ilosc czesci
    int parts_number;
}
FsWriteC;

typedef struct FsWriteS
{
    // liczba zapisanych bajtow
    size_t buffer_len;
    // liczba zwracana przez funkcje
    int status;
}
FsWriteS;

// @todo SPRAWDZIC POPRAWNOSC!
// marcin: przesunalem buffer do danych otrzymywanych z serwera

typedef struct FsReadC
{
    // uchwyt do serwera
    int server_handler;
    // unikalny deskryptor pliku dla danego klienta
    int fd;
    // liczba faktycznie przekazanych bajtow
    size_t buffer_len;
}

FsReadC;

typedef struct FsReadS
{
    // liczba zwracana przez funkcje
    int status;
    // buffor danych do odczytania z pliku
    char buffer[BUF_LEN];
    // ile danych chcemy wczytac
    size_t buffer_len;
    // identyfikator czesci
    int part_id;
    // ilosc czesci
    int parts_number;
}
FsReadS;

typedef struct FsLseekC
{
    // uchwyt do serwera
    int server_handler;
    // unikalny deskryptor pliku dla danego klienta
    int fd;
    // oznacza miejsce od ktorego ma sie rozpoczac
    // najblizsza operacja odczytu/zapisu - liczba bajtow od poczatku
    long offset;
    // whence ma nastepujace znaczenie
    //     0 = liczymy offset od samego poczatku
    //     1 = liczymy offset od aktualnej pozycji
    //     2 = liczymy offset od konca
    unsigned int whence;
}
FsLseekC;

typedef struct FsLseekS
{
    // liczba zwracana przez funkcje
    int status;
}
FsLseekS;

typedef struct FsCloseC
{
    // uchwyt do serwera
    int server_handler;
    // unikalny deskryptor pliku dla danego klienta
    int fd;
}
FsCloseC;

typedef struct FsCloseS
{
    // liczba zwracana przez funkcje
    int status;
}
FsCloseS;

typedef struct FsFstat
{
    // protection
    mode_t mode;
    // total size, in bytes
    off_t size;
    // time of last access
    time_t atime;
    // time of modification
    time_t mtime;
    // time of last status change
    time_t ctime;
}
FsFstat;

typedef struct FsFstatC
{
    // uchwyt do serwera
    int server_handler;
    // unikalny deskryptor pliku dla danego klienta
    int fd;
    FsFstat stat;
    // kolejka blokad
    // int* queueLock;
    // aktualny stan
    int actualState;
}
FsFstatC;

typedef struct FsFstatS
{
    // liczba zwracana przez funkcje
    int status;
}
FsFstatS;

typedef struct FsLockC
{
    // uchwyt do serwera
    int server_handler;
    // unikalny deskryptor pliku dla danego klienta
    int fd;
    // ustawienia blokady
    //     mode = 0, blokada READ
    //     mode = 1, blokada WRITE
    //     mode = 2, zdejmnij poprzednia blokade
    int mode;
}
FsLockC;

typedef struct FsLockS
{
    // liczba zwracana przez funkcje
    int status;
}
FsLockS;

typedef struct FsCloseServerC
{
	// uchwyt do serwera
	int server_handler;
}
FsCloseServerC;

typedef struct FsCloseServerS
{
    // liczba zwracana przez funkcje
    int status;
}
FsCloseServerS;

typedef struct FsRequest
{
	// komenda wysylana do serwera
    FsCommand command;

    union RequestData
    {
        FsOpenServerC open_server;
		FsCloseServerC close_server;
        FsOpenC open;
        FsWriteC write;
        FsReadC read;
        FsLseekC lseek;
        FsCloseC close;
        FsFstatC fstat;
        FsLockC lock;
    }
	data;
}
FsRequest;

typedef struct FsResponse
{
	// komunikat - odpowiedz serwera
    FsAnswer answer;

    union ResponseData
    {
        FsOpenServerS open_server;
		FsCloseServerS close_server;
        FsOpenS open;
        FsWriteS write;
        FsReadS read;
        FsLseekS lseek;
        FsCloseS close;
        FsFstatS fstat;
        FsLockS lock;
    }
	data;
}
FsResponse;

#endif // PROTOCOL_H_INCLUDED
