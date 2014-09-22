#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include "cola.h"
#include "main.h"

//Inicializar semaforo
void InitSemaforo(tSemaforo *semaforo)
{
	semaforo->disponible = 1;
	semaforo->buffer[0] = '\0';
	semaforo->cola_ocupada = 0;
	InitCola(&semaforo->cola);
	semaforo->procesos[0] = -1;
	semaforo->procesos[1] = -1;
	semaforo->procesos[2] = -1;
}

//Variable globa lde semaforo
tSemaforo *g_pSem;

int main()
{
	//Declaracion de variables
	int total, i, fd, delay;

	//Archivo log.txt
	FILE *logfile;
	logfile = fopen("log.txt", "a+"); //<-Nota, no se si es a+ o w+ (Debemos borar el contenido anterior cuando inicializamos el programa??)

	// Semaforo
	//Todo este proceso me parece mas bien generico para mapear cualquiera TDA
	fd = shm_open("semaforo.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);
	ftruncate(fd, sizeof(tSemaforo));
	g_pSem = mmap(NULL, sizeof(tSemaforo), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	//A esta altura, g_pSem funciona como si hubieramos hecho un malloc, pero puede ser compartido a otros
	//threads y a los procesos que creamos con fork
	InitSemaforo(g_pSem);

	//Preguntamos
	printf("¿Cuantas veces desea obtener la información?\n");
	scanf("%d", &total);
	printf("Ingrese intervalo de tiempo (milisegundos) entre el escaneo del sistema\n");
	scanf("%d", &delay);
	pid_t pid;

	//No puse todo en funciones distintas porque estaba aprendiendo como hacerlo, aunque lo podemos hacer despues para
	//que sea mas legible todo :D.
	//Hijo 1
	pid = fork();
	if(pid < 0)
	{
		printf("Error at doing a fork\n");
		exit(1);
	}
	else if(pid == 0)
	{
		//Soy el hijo
		printf("(%d): Running...\n", (int) getpid());
		struct sysinfo info;
		for(i = 0; i <= total; i++)
		{
			//Esto es en caso de que algun otro proceso este haciend oqueue y dequeue en la cola
			//en donde podriamos tener errores de datos. Solo por seguriddad
			while(g_pSem->cola_ocupada);
			g_pSem->cola_ocupada = 1;
			enqueue(&g_pSem->cola, (int) getpid());
			g_pSem->cola_ocupada = 0;

			//fin de queue
			//Ahora debo eseprar a que la cola no este ocupada (para leer el fronvalue) y el buffer dsinpobile
			//y, ademas, que me toque a mi
			while(g_pSem->cola_ocupada || !g_pSem->disponible || frontvalue(&g_pSem->cola) != getpid());
			//Seccion critica
			//Me toca!
			g_pSem->disponible = 0;

			//Mensajes rutinarios
			if(i < total)
			{
				sysinfo(&info);
				snprintf(g_pSem->buffer, sizeof(g_pSem->buffer), "(%d): El sistema lleva %ld s encendido.", (int) getpid(), info.uptime);
			}
			else
				snprintf(g_pSem->buffer, sizeof(g_pSem->buffer), "(%d): Subproceso finalizado", (int) getpid());

			//Vuelvo a esperar a que la cola este libre (poco probable que no lo este, pero es por seguridad)
			while(g_pSem->cola_ocupada);
			g_pSem->cola_ocupada = 1;
			dequeue(&g_pSem->cola);
			g_pSem->cola_ocupada = 0;
			//fin dequeue
			//Fin sección critica
			//Esperamos un poquito
			usleep(delay*1000);
			
		}
		//Terminamos
		printf("(%d) Done\n", getpid());
		g_pSem->procesos[0] = -1;
		exit(1);
	}
	g_pSem->procesos[0] = pid;

	//Hijo 2
	pid = fork();
	if(pid < 0)
	{
		printf("Error at doing a fork\n");
		exit(1);
	}
	else if(pid == 0)
	{
		struct sysinfo info;
		for(i = 0; i <= total; i++)
		{
			while(g_pSem->cola_ocupada);
			g_pSem->cola_ocupada = 1;
			enqueue(&g_pSem->cola, (int) getpid());
			g_pSem->cola_ocupada = 0;
			while(g_pSem->cola_ocupada || !g_pSem->disponible || frontvalue(&g_pSem->cola) != getpid());
			//Seccion critica
			g_pSem->disponible = 0;
			if(i < total)
			{
				sysinfo(&info);
				//Debemos darle formato a los bytes
				snprintf(g_pSem->buffer, sizeof(g_pSem->buffer), "(%d): Disponibles %ldMB de %ldMB", (int) getpid(), info.freeram/1048576, info.totalram/1048576);
			}
			else
				snprintf(g_pSem->buffer, sizeof(g_pSem->buffer), "(%d): Subproceso finalizado", (int) getpid());
			while(g_pSem->cola_ocupada);
			g_pSem->cola_ocupada = 1;
			dequeue(&g_pSem->cola);
			g_pSem->cola_ocupada = 0;
			//Fin sección critica
			usleep(delay*1000);
			
		}
		printf("(%d) Done\n", getpid());
		g_pSem->procesos[1] = -1;
		exit(1);
	}
	g_pSem->procesos[1] = pid;

	//Hijo 3
	pid = fork();
	if(pid < 0)
	{
		printf("Error at doing a fork\n");
		exit(1);
	}
	else if(pid == 0)
	{
		printf("(%d): Running...\n", (int) getpid());
		struct sysinfo info;
		for(i = 0; i <= total; i++)
		{
			while(g_pSem->cola_ocupada);
			g_pSem->cola_ocupada = 1;
			enqueue(&g_pSem->cola, (int) getpid());
			g_pSem->cola_ocupada = 0;
			while(g_pSem->cola_ocupada || !g_pSem->disponible || frontvalue(&g_pSem->cola) != getpid());
			//Seccion critica
			g_pSem->disponible = 0;
			if(i < total)
			{
				sysinfo(&info);
				snprintf(g_pSem->buffer, sizeof(g_pSem->buffer), "(%d): Hay %d procesos en ejecución", (int) getpid(), info.procs);
			}
			else
				snprintf(g_pSem->buffer, sizeof(g_pSem->buffer), "(%d): Subproceso finalizado", (int) getpid());
			while(g_pSem->cola_ocupada);
			g_pSem->cola_ocupada = 1;
			dequeue(&g_pSem->cola);
			g_pSem->cola_ocupada = 0;
			//Fin sección critica
			usleep(delay*1000);
			
		}
		printf("(%d): Done\n", getpid());
		g_pSem->procesos[2] = -1;
		exit(1);
	}
	g_pSem->procesos[2] = pid;

	//El padre debe leer!
	while(1)
	{
		//si el buffer no esta disponible para los hijos, entonces esta disponible para el padre
		if(!g_pSem->disponible)
		{
			printf("Father writing\n");
			fprintf(logfile, "%s\n", g_pSem->buffer);
			g_pSem->disponible = 1;
		}

		//Todos los procesos terminaron? terminar yo tambien pues!
		if(g_pSem->procesos[0] == -1
		&& g_pSem->procesos[1] == -1
		&& g_pSem->procesos[2] == -1)
		{
			break;
		}
	}
	fprintf(logfile, "(%d) Programa finalizado", getpid());
	fclose(logfile);
	printf("Parent done\n");
	return 0;
}