#include <stdlib.h>
#include <stdio.h>
#include "cola.h"

void InitCola(tCola *C)
{
	C->maxsize = QUEUE_SIZE+1;
	C->ultimo =0; C->frente =1; //cola es circular. ultimo debe estar uno antes del frente!
	C->arreglo = (tipocola *)malloc(sizeof(C->maxsize)) ;//se pide un arreglo para n+1 posiciones. n es el tamño maximo de elementos.
}

void clear(tCola *C)
{
	free((void *)(C->arreglo)); //free recibe un puntero de tipo void! por eso se castea antes!
	InitCola(C);
}


void enqueue(tCola *C,tipocola item)
{ //solo se mueve la posicion del ultimo, y al moverla se agrega el elemento!
	if(((C->ultimo+2)%C->maxsize) == C->frente)
	{
		printf("LLENO! ");
		return;
	}
	C->ultimo = (C->ultimo+1)%C->maxsize;
	C->arreglo[C->ultimo]=item;	
}
	
int lenght(tCola *C)
{
	return ((C->ultimo+C->maxsize)-C->frente+1)%C->maxsize;
}

void dequeue(tCola *C)
{
	if(lenght(C)==0)
		printf("COLA VACIA! \n");
	
	C->frente = (C->frente+1)%C->maxsize; //incremento circular!
}

int frontvalue(tCola *C)
{
	if(lenght(C)==0)
		printf("COLA VACIA \n");
	return C->arreglo[C->frente];		
}
