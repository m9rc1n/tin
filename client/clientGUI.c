#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "ncurses-menu.h"
#include "ncurses-readstring.h"
#include "../tin_library/tin_library.h"

char address[20], portString[20], fileName[50], readBufor[512], writeBufor[512];
volatile int serverHandler;
volatile int fileDescriptor; 

void loadConfiguration()
{
	FILE *file;
	char tempLine[20];
	int entry = 1;
	file = fopen("client.conf", "rt");
	while(fgets(tempLine,20,file) != NULL && entry != 3)
	{
		if(entry == 1)
		{
			sscanf(tempLine, "%s", address);
		}
		else if(entry == 2)
		{
			sscanf(tempLine, "%s", portString);
		}
		entry += 1;
	}	
	fclose(file);
}

void saveConfiguration()
{
	FILE *file;
	file = fopen("client.conf", "w+");

	fprintf(file,"%s", address );
	fprintf(file, "\n");
	fprintf(file,"%s", portString ); 

	fclose(file); 	
}

int main(int argc, char *argv[])
{
	//pozycjonowanie menu
	int COORD_Y = 4, COORD_X = 10, WIDTH = 15;


	int mainMenuReturn = 1, configMenuReturn = 1, connectedMenuReturn = 1, fileMenuReturn = 1;
	char mainMenu[][100] = {{"Connect"}, {"Setup"}, {"Quit"},};
	char configMenu[][100] = {{"Address"}, {"Port"}, {"Save config"}, {"Load config"}, {"Return"},};
	char connectedMenu[][100] = {{"Run test"},{"Open file"},{"Disconnect"}}; 
	char fileMenu[][100] = {{"Read file"},{"Write file"},{"Edit file"},{"File info"},{"Return"},};

	setlocale(LC_CTYPE, "");
	
	initscr();
	noecho();
	keypad (stdscr, TRUE);
    meta (stdscr, TRUE);
    nodelay (stdscr, FALSE);
    notimeout (stdscr, TRUE);
    raw();
    curs_set (0);

	//load config after run
	loadConfiguration();

	do
	{
		mainMenuReturn = print_menu(COORD_Y,COORD_X,3,WIDTH,"Menu",mainMenu,mainMenuReturn); // 1, 2 i 4ty parametry mozna zmianiec zeby potestowac wyglad
		
		if(mainMenuReturn == 1) //FILE MENU
		{
			mvprintw(COORD_Y + 3,COORD_X + WIDTH + 3, "Connecting...");
			//laczymy sie z serwerem
			serverHandler = fs_open_server(address, strtol(portString, NULL, 0));
			erase();
			do
			{	//powinno jeszcze byc edytowanie pliku lub edytowawnie przez lseek...
				connectedMenuReturn = print_menu(COORD_Y,COORD_X,3,WIDTH,"File operations",connectedMenu,connectedMenuReturn);
				if(connectedMenuReturn == 1)
				{
					//TODO
					//uruchamiamy testy
				}
				else if(connectedMenuReturn == 2)
				{
					erase();
					do
					{
						fileMenuReturn = print_menu(COORD_Y,COORD_X,5,WIDTH,"File operations",fileMenu,fileMenuReturn);
						if(fileMenuReturn == 1)
						{
							mvreadstr (COORD_Y + 3,COORD_X + WIDTH + 5, fileName, 18, 0);
							fileDescriptor = fs_open(serverHandler, fileName, "r");
							//odczyt
							erase();
							fs_read(serverHandler, fileDescriptor, &readBufor, sizeof(readBufor));
							mvprintw(1,1,readBufor);
							getch();
							fs_close(serverHandler, fileDescriptor);
							erase();
						}
						else if(fileMenuReturn == 2)
						{
							//otwarcie pliku
							mvreadstr (COORD_Y + 5,COORD_X + WIDTH + 5, fileName, 18, 0);
							fileDescriptor = fs_open(serverHandler, fileName, "w+");
							//zapis, fs_write tez cos na razie nie dziala
							erase();
							mvreadstr (COORD_Y - 3,COORD_X - 9, writeBufor, 512, 0);
							erase();
							fs_write(serverHandler, fileDescriptor, writeBufor, sizeof(writeBufor));
							fs_close(serverHandler, fileDescriptor);
						}
						else if(fileMenuReturn == 3)
						{
							//edycja pliku od pewnego miejsca = fs_write + fs_lseek
						}
						else if(fileMenuReturn == 4)
						{
							//informacje o pliku
						}
					} while(fileMenuReturn != 5);
					erase();
					
				}
				else if(connectedMenuReturn == 3)
				{
					//Rozlaczamy sie z serwerem
					fs_close_server(serverHandler);
				}

			} while(connectedMenuReturn != 3);

		}
		else if(mainMenuReturn == 2) //SETTING MENU
		{
			erase();
			do
			{
				//current settings on screen
				mvprintw(COORD_Y - 3,COORD_X,"Current address: ");
				mvprintw(COORD_Y - 3,COORD_X + WIDTH + 3,address);
				mvprintw(COORD_Y - 1,COORD_X,"Current port: ");
				mvprintw(COORD_Y - 1,COORD_X + WIDTH + 3,portString);

				configMenuReturn = print_menu(COORD_Y,COORD_X,5,WIDTH,"Set settings",configMenu,configMenuReturn);
				if(configMenuReturn == 1)
				{
					//address
					mvreadstr (COORD_Y + 3,COORD_X + WIDTH + 3, address, 18, 0);
				}
				else if(configMenuReturn == 2)
				{
					//port
					mvreadstr (COORD_Y + 5,COORD_X + WIDTH + 3, portString, 18, 0);
				}
				else if(configMenuReturn == 3)
				{
					//save
					saveConfiguration();
				}
				else if(configMenuReturn == 4)
				{
					//load
					loadConfiguration();
				}
				erase();
			} while(configMenuReturn != 5);
		}
		
		erase();

	} while(mainMenuReturn != 3);

	endwin();
	return 0;
}
