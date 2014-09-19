#ifndef __COLA_INCLUDED_H__
#define __COLA_INCLUDED_H__
#define QUEUE_SIZE 100
typedef pid_t tipocola;
typedef struct{
	int maxsize; //tamano maximo de cola
	int frente; //index del primer elemento
	int ultimo; //index del ultimo elemento
	tipocola *arreglo;
	
	
	}tCola;
	
	
void InitCola(tCola *C);
void clear(tCola *C);
void enqueue(tCola *C, tipocola item);
int lenght(tCola *C);
void dequeue(tCola *C);
int frontvalue(tCola *C);

#endif //__COLA_INCLUDED_H__