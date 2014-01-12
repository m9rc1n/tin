#ifndef SERVER_CONST_H
#define SERVER_CONST_H

#ifdef VDBG
#define VDP0(x) (printf(x))
#define VDP1(x,y) (printf(x,y))
#define VDP2(x,y,z) (printf(x,y,z))
#define VDP3(x,y,z,q) (printf(x,y,z,q))
#else
#define VDP0(x)
#define VDP1(x,y)
#define VDP2(x,y,z)
#define VDP3(x,y,z,q)
#endif

/**
 * Tutaj wrzucamjmy wszystkie makrodefinicje, żeby było potem prościej zmieniać wszelkiego rodzaju
 *  parametry serwera. Ponadto raczej lepiej definiować parametry poprzez idndef-define-endif, bo
 *  wtedy można dać dla testów jakiś mniej sensowny parametr i wygodniej testować (np. timeouty).
 */

/**
 * Wielkosc bufora przesylana miedzy kientem a serwerem
 * @todo Pozniej pojdzie do protokolu albo wymysle cos lepszego
 */
#ifndef MAX_BUF
#define MAX_BUF 256
#endif

/**
 * Wielkość tablicy w hashmapie. Używane w obsłudze sesji.
 * Z definicji hashpamy lepiej podać liczbę pierwszą.
 */
#ifndef SESSION_HASH_LIST_SIZE
#define SESSION_HASH_LIST_SIZE 31
#endif

/**
 * Maksymalna ilość jednocześnie aktywnych sesji. Używane w obsłudze sesji.
 */
#ifndef SESSION_MAX_NUMBER
#define SESSION_MAX_NUMBER 16
#endif

/**
 * Okres wykonywania zombie collection (ubijania starych i nieaktywnych sesji) w sekundach.
 */
#ifndef SESSION_SWEEP_TIME
#define SESSION_SWEEP_TIME 10
#endif

/**
 * Okres od ostatniego requesta po upłynięciu którego sesja jest uznana jako zombie w sekundach.
 */
#ifndef SESSION_ZOMBIE_TIME
#define SESSION_ZOMBIE_TIME 15
#endif

/**
 * Maksymalna ilość plików otwartych w jednej sesji.
 */
#ifndef MAX_FD_PER_SESSION
#define MAX_FD_PER_SESSION 4
#endif

#endif
