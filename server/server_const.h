#ifndef SERVER_CONST_H
#define SERVER_CONST_H

/**
 * Tutaj wrzucamjmy wszystkie makrodefinicje, żeby było potem prościej zmieniać wszelkiego rodzaju
 *  parametry serwera. Ponadto raczej lepiej definiować parametry poprzez idndef-define-endif, bo
 *  wtedy można dać dla testów jakiś mniej sensowny parametr i wygodniej testować (np. timeouty).
 */

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


/*
 * Poniższe dwa są tutaj tylko na chwilę, zaraz wylecą.
 */
#define SWEEP_TIME 10
#define ZOMBIFICATION_PERIOD 1800


#endif
