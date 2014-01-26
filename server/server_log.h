#ifndef _SERVER_LOG_H
#define _SERVER_LOG_H

void logopen();
void logprint();
void logclose();

extern char logbuffer[128];

#endif
