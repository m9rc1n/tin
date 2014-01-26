#include "server_log.h"
#include <stdio.h>
#include <string.h>

static FILE *logfile;

char logbuffer[128];

void logopen()
{
    logfile = fopen("server.log", "at");
    
    if(logfile == NULL)
    {
        perror("Unable to create log file!");
    }
}

void logprint()
{
    fwrite(logbuffer, sizeof(char), strlen(logbuffer), logfile);
    fflush(logfile);
}

void logclose()
{
    fclose(logfile);
    logfile = NULL;
}