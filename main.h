#ifndef __MAIN_H_INCLUDED__
#define __MAIN_H_INCLUDED__

//Esta es la estructura del semaforo
typedef struct
{
	int procesos[3];		//Esto es para ver cuando terminan los procesos
	char buffer[512];		//El buffer donde escribimos cosas
	char disponible;		//Esta el buffer disponible?
	char cola_ocupada;		//Para evitar posibles errores en la cola, los procesos de queue
							//y enqueue deben ser analizados tambien!
	tCola cola;				//Cola de procesamiento (Los que estan en lista de espera)
} tSemaforo;

//Inicializar las variables del semaforo
void InitSemaforo(tSemaforo *sem);


#endif //__MAIN_H_INCLUDED__