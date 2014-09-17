#include <stdlib.h>
#include <stdio.h>
#define SIZE 100
typedef tipocola;


//ultimo parte en posicion cero
// frente , parte una posicion despues.!
//modulo con el maxsize!
typedef struct{
	int maxsize; //tamano maximo de cola
	int frente; //index del primer elemento
	int ultimo; //index del ultimo elemento
	tipocola *arreglo;
	
	
	}tcola;
	
	
void init(tcola *C){
	C->maxsize = SIZE+1;
	C->ultimo =0; C->frente =1; //cola es circular. ultimo debe estar uno antes del frente!
	C->arreglo = (tipocola *)malloc(sizeof(C->maxsize)) ;//se pide un arreglo para n+1 posiciones. n es el tamño maximo de elementos.
	
	}

void clear(tcola *C){
	free((void *)(C->arreglo)); //free recibe un puntero de tipo void! por eso se castea antes!
	init(C);
	
	}


void enqueue(tcola *C,tipocola item){ //solo se mueve la posicion del ultimo, y al moverla se agrega el elemento!
	if(((C->ultimo+2)%C->maxsize) == C->frente){ printf("LLENO! "); return;} 
	C->ultimo = (C->ultimo+1)%C->maxsize;
	C->arreglo[C->ultimo]=item;
	
	}

int lenght(tcola *C){
	return ((C->ultimo+C->maxsize)-C->frente+1)%C->maxsize;


}
void dequeue(tcola *C){
	if(lenght(C)==0) printf("COLA VACIA! \n");
	
	C->frente = (C->frente+1)%C->maxsize; //incremento circular!
	
	}

int frontvalue(tcola *C){
	if(lenght(C)==0) printf("COLA VACIA \n");
	printf("%d ",C->arreglo[C->frente]);
	return C->arreglo[C->frente];
		
	}

int main(){
	
	return 1;

}
