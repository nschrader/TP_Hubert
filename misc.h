#ifndef MISC_H
#define MISC_H

#define IPC_NOFLAGS   0
#define IPC_ALLWRITE  0666

#define ERROR         -1
#define NO_ERRNO      0
#define CHILD         0

#define fatal(x) _fatal("[Fatal] " x);
void _fatal(char* errorMessage);

#define warning(x) _warning("[Warning] " x);
void _warning(char* errorMessage);

#define info(x) _info("[Info] " x)
void _info(char* infoMessage);

void V(int semid, int sem_num);
void P(int semid, int sem_num);
void V2(int semid, int sem_num);
void P2(int semid, int sem_num);

#endif
