#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "ncurses-menu.h"
#include "ncurses-readstring.h"
#include "../tin_library/tin_library.h"

#define READ_LEN 512

char address[20], portString[20], fileName[50], readBufor[512], writeBufor[512], size[15], mode[15], atime[15], cctime[15], mtime[15], offset[15];
volatile int serverHandler;
volatile int fileDescriptor;
struct stat* buf;

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


	int mainMenuReturn = 1, configMenuReturn = 1, connectedMenuReturn = 1, fileMenuReturn = 1, testMenuReturn = 1;
	char mainMenu[][100] = {{"Connect"}, {"Setup"}, {"Quit"},};
	char configMenu[][100] = {{"Address"}, {"Port"}, {"Save config"}, {"Load config"}, {"Return"},};
	char connectedMenu[][100] = {{"Run test"},{"Create file"},{"Open file"},{"Disconnect"}};
	char fileMenu[][100] = {{"Read file"},{"Read file from"},{"Write file"},{"Edit file"}, {"Edit file from"},{"File info"},{"Return"},};
	char testMenu[][100] = {{"Test Open"},{"Test Read"},{"Test Write"},{"Simple test"},{"Basic session"},{"Session timeout"},{"Return"},};

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

			if(serverHandler != -1)
{
			erase();
			do
			{
				connectedMenuReturn = print_menu(COORD_Y,COORD_X,4,WIDTH,"File operations",connectedMenu,connectedMenuReturn);
				if(connectedMenuReturn == 1)
				{
					erase();
					do
					{
						testMenuReturn = print_menu(COORD_Y,COORD_X,7,WIDTH,"Tests Menu",testMenu,testMenuReturn);
						int errorCode = 0;
						if(testMenuReturn == 1)
						{
							erase();
							int pid = fork();
							char *arguments[4] = {"./file_opener.out", address, portString, NULL};
							if(pid == 0)
							{
								execv("../tests/bin/file_opener.out", arguments);
								wait(&errorCode);
							}
							sleep(2);
							if(errorCode == 0){mvprintw(20,1, "Test success");}else{mvprintw(20,1, "Test failed");}
							getch();
						}
						else if (testMenuReturn == 2)
						{
							int pid = fork();
							char *arguments[4] = {"./file_reader.out", address, portString, NULL};
							if(pid == 0)
							{
								execv("../tests/bin/file_reader.out", arguments);
								wait(&errorCode);
							}
							sleep(2);
							if(errorCode == 0){mvprintw(20,1, "Test success");}else{mvprintw(20,1, "Test failed");}
							getch();
						}
						else if (testMenuReturn == 3)
						{
							int pid = fork();
							char *arguments[4] = {"./file_writer.out", address, portString, NULL};
							if(pid == 0)
							{
								execv("../tests/bin/file_writer.out", arguments);
								wait(&errorCode);
							}
							sleep(2);
							if(errorCode == 0){mvprintw(20,1, "Test success");}else{mvprintw(20,1, "Test failed");}
							getch();
						}
						else if (testMenuReturn == 4)
						{
							int pid = fork();
							char *arguments[4] = {"./file_simple.out", address, portString, NULL};
							if(pid == 0)
							{
								execv("../tests/bin/file_simple.out", arguments);
								wait(&errorCode);
							}
							sleep(2);
							if(errorCode == 0){mvprintw(20,1, "Test success");}else{mvprintw(20,1, "Test failed");}
							getch();
						}
						else if (testMenuReturn == 5)
						{
							int pid = fork();
							char *arguments[5] = {"./session_basic.out", address, portString, "40", NULL};
							if(pid == 0)
							{
								execv("../tests/bin/session_basic.out", arguments);
								wait(&errorCode);
							}
							sleep(2);
							if(errorCode == 0){mvprintw(20,1, "Test success");}else{mvprintw(20,1, "Test failed");}
							getch();
						}
						else if (testMenuReturn == 6)
						{
							int pid = fork();
							char *arguments[4] = {"./session_timeouter.out", address, portString, NULL};
							if(pid == 0)
							{
								execv("../tests/bin/session_timeouter.out", arguments);
								wait(&errorCode);
							}
							sleep(2);
							if(errorCode == 0){mvprintw(20,1, "Test success");}else{mvprintw(20,1, "Test failed");}
							getch();
						}
						erase();
					} while(testMenuReturn != 7);
					erase();
				}
				else if(connectedMenuReturn == 2)
				{
					mvreadstr (COORD_Y + 5,COORD_X + WIDTH + 5, fileName, 18, 0);
					fileDescriptor = fs_open(serverHandler, fileName, O_CREAT);
					fs_close(serverHandler, fileDescriptor);
					erase();
				}
				else if(connectedMenuReturn == 3)
				{
					erase();
					do
					{
						fileMenuReturn = print_menu(COORD_Y,COORD_X,7,WIDTH,"File operations",fileMenu,fileMenuReturn);
						if(fileMenuReturn == 1)
						{
							mvreadstr(COORD_Y + 3,COORD_X + WIDTH + 5, fileName, 18, 0);
							fileDescriptor = fs_open(serverHandler, fileName, O_RDONLY);
							//odczyt
							erase();

                            char* buf = (char*) calloc (sizeof(char), READ_LEN);

							fs_read(serverHandler, fileDescriptor, buf, READ_LEN);
							if( buf!= NULL) mvprintw(1,1, buf);

							getch();
							fs_close(serverHandler, fileDescriptor);
							//sprintf(cctime, "%d", (int)cc );	mvprintw(22,1, cctime); getch();

                            free(buf);

							erase();
						}
						else if(fileMenuReturn == 3)
						{
							//otwarcie pliku
							mvreadstr (COORD_Y + 7,COORD_X + WIDTH + 5, fileName, 18, 0);
							fileDescriptor = fs_open(serverHandler, fileName, O_WRONLY);
							//zapis
							erase();
							mvreadstr (COORD_Y - 3,COORD_X - 9, writeBufor, 512, 0);
							char  ch = writeBufor[0];
							int i = 0;
							while(ch != ';')
							{
								i += 1;
								ch = writeBufor[i];
							}
							erase();
							fs_write(serverHandler, fileDescriptor, writeBufor, i);
							fs_close(serverHandler, fileDescriptor);
						}
						else if(fileMenuReturn == 4)
						{
							mvreadstr(COORD_Y + 9,COORD_X + WIDTH + 5, fileName, 18, 0);
							fileDescriptor = fs_open(serverHandler, fileName, O_WRONLY);
							fs_lseek(serverHandler, fileDescriptor, 0, SEEK_END);
							erase();
							mvreadstr(COORD_Y - 3,COORD_X - 9, writeBufor, 512, 0);
							char  ch = writeBufor[0];
							int i = 0;
							while(ch != ';')
							{
								i += 1;
								ch = writeBufor[i];
							}
							erase();
							fs_write(serverHandler, fileDescriptor, writeBufor, i);
							fs_close(serverHandler, fileDescriptor);
						}
						else if(fileMenuReturn == 6)
						{
							mvreadstr (COORD_Y + 13,COORD_X + WIDTH + 5, fileName, 18, 0);
							fileDescriptor = fs_open(serverHandler, fileName, O_RDONLY);
							erase();
							buf = (struct stat*) calloc(sizeof(struct stat), 1);
							//informacje o pliku
							fs_fstat(serverHandler, fileDescriptor, buf);

							sprintf(size, "%d", (int)buf->st_size );
							sprintf(mode, "%d", (int)buf->st_mode );
							sprintf(atime, "%d", (int)buf->st_atime );
							sprintf(cctime, "%d", (int)buf->st_ctime );
							sprintf(mtime, "%d", (int)buf->st_mtime );

							mvprintw(1,1, "File size: ");
							mvprintw(1,20, size);

							mvprintw(2,1, "File mode: ");
							mvprintw(2,20, mode);

							mvprintw(3,1, "Access time: ");
							mvprintw(3,20, atime);

							mvprintw(4,1, "Change time: ");
							mvprintw(5,20, cctime);

							mvprintw(5,1, "Modyfication time: ");
							mvprintw(5,20, mtime);

							getch();
							erase();
							free(buf);
							fs_close(serverHandler, fileDescriptor);
						}
						else if(fileMenuReturn == 2)
						{
							//nazwa pliku
							mvreadstr(COORD_Y + 5,COORD_X + WIDTH + 5, fileName, 18, 0);
							//offset
							mvreadstr(COORD_Y + 5,COORD_X + WIDTH + 20, offset, 18, 0);

                            char* buf = (char*) calloc (sizeof(char), READ_LEN);
							fileDescriptor = fs_open(serverHandler, fileName, O_RDONLY);
							fs_lseek(serverHandler, fileDescriptor, atoi(offset), SEEK_SET);
							//odczyt
							erase();
							fs_read(serverHandler, fileDescriptor, buf, READ_LEN);
							if (buf != NULL) mvprintw(1, 1, buf);
							// mvprintw(1, 1, readBufor);
                            fs_close(serverHandler, fileDescriptor);
							getch();
							free(buf);
							erase();

						}
						else if(fileMenuReturn == 5)
						{
							mvreadstr(COORD_Y + 11,COORD_X + WIDTH + 5, fileName, 18, 0);
							mvreadstr(COORD_Y + 11,COORD_X + WIDTH + 20, offset, 18, 0);

							fileDescriptor = fs_open(serverHandler, fileName, O_WRONLY);
							fs_lseek(serverHandler, fileDescriptor, atoi(offset), SEEK_SET);
							erase();
							mvreadstr(COORD_Y - 3,COORD_X - 9, writeBufor, 512, 0);
							char  ch = writeBufor[0];
							int i = 0;
							while(ch != ';')
							{
								i += 1;
								ch = writeBufor[i];
							}
							erase();
							fs_write(serverHandler, fileDescriptor, writeBufor, i);
							fs_close(serverHandler, fileDescriptor);
						}
					} while(fileMenuReturn != 7);
					erase();

				}
				else if(connectedMenuReturn == 4)
				{
					//Rozlaczamy sie z serwerem
					fs_close_server(serverHandler);
				}

			} while(connectedMenuReturn != 4);
}

		}
		else if(mainMenuReturn == 2) //SETTING MENU
		{
			erase();
			do
			{
				//current settings on screen
				mvprintw(COORD_Y - 3,COORD_X,"Current address: ");
				mvprintw(COORD_Y - 3,COORD_X + WIDTH + 3,address);

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
