#ifndef __COLA_INCLUDED_H__
#define __COLA_INCLUDED_H__
#define QUEUE_SIZE 100
typedef pid_t tipocola;
typedef struct{
	int maxsize; //tamano maximo de cola
	int frente; //index del primer elemento
	int ultimo; //index del ultimo elemento
	tipocola *arreglo;
	
	
	}tcola;
	
	
void init(tcola *C);
void clear(tcola *C);
void enqueue(tcola *C, tipocola item);
int lenght(tcola *C);
void dequeue(tcola *C);
int frontvalue(tcola *C);

#endif //__COLA_INCLUDED_H__