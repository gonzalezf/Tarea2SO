#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include "cola.h"
#include <sys/sysinfo.h>
#include <stdlib.h>

//Implementar cola...
/////////////////////////////////7777
tcola g_queue; //Crearla
int g_iTotalTimes = 0;
pid_t g_pids[3]; //Arreglo e tamaño 3, 0 -->, 1 -->, 2 -->
char g_buffer[512];
char g_isBufferFree = 0;
FILE * g_log;

void ShowOnlineTime()
{
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Error at doing a fork\n");
		exit(1);
	}
	else if(pid == 0)
	{
		int i;
		for(i = 0; i <= g_iTotalTimes; i++)
		{

			//Aqui reconoci que yo soy el hijo
			enqueue(&g_queue, g_pids[0]);
			while(frontvalue(&g_queue) != g_pids[0] || !g_isBufferFree)
			{
				printf("Hijo: Waiting\n");
				printf("Hijo: Buffer: [%s]\n", g_buffer);
				sleep(1);
			}
			printf("Hijo: Write!\n");
			g_isBufferFree = 0;
			struct sysinfo *info;
			sysinfo(info);
			snprintf(g_buffer, sizeof(g_buffer), "%d: El sistema lleva %ld s encendido.", g_pids[0], info->uptime);
			printf("Hijo: Buffer: [%s]\n", g_buffer);
			dequeue(&g_queue);
			//Aaca
		}
		g_pids[0] = -1;
		exit(0);
		//Aca
	}
	else
	{
		//Yo soy el padre
		g_pids[0] = pid;
	}
}
void ShowAvailableRam()
{
	/*
	pid_t pid = fork();

	if(pid < 0)
	{
		print("Error at doing a fork");
		exit();
	}
	else if(pid == 0)
	{
		int i;
		for(i = 0; i < g_iTotalTimes; i++)
		{
			//Aqui reconoci que yo soy el hijo
			g_queue.enqueue(g_pids[1]);
			while(&g_queue.top != g_pids[1] || !g_isBufferFree);
			g_isBufferFree = false;
			struct sysinfo *info;
			sysinfo(sysinfo);
			snprintf(g_buffer, sizeof(g_buffer), "%d: El sistema lleva %d s enecendido.", g_pids[1]);
			g_queue.dequeue();
		}
		g_pids[0] = -1;
		exit();
	}
	else
	{
		//Yo soy el padre
		g_pids[0] = pid;
	}
	*/
}

void ShowProcesses()
{
	/*
	pid_t pid = fork();

	if(pid < 0)
	{
		print("Error at doing a fork");
		exit();
	}
	else if(pid == 0)
	{
		int i;
		for(i = 0; i < g_iTotalTimes; i++)
		{
			//Aqui reconoci que yo soy el hijo
			g_queue.enqueue(g_pids[0]);
			while(&g_queue.top != g_pids[0] || !g_isBufferFree);
			g_isBufferFree = false;
			struct sysinfo *info;
			sysinfo(sysinfo);
			snprintf(g_buffer, sizeof(g_buffer), "%d: El sistema lleva %d s enecendido.", g_pids[0]);
			g_queue.dequeue();
		}
		g_pids[0] = -1;
		exit();
	}
	else
	{
		//Yo soy el padre
		g_pids[0] = pid;
	}
	*/
}

int main()
{
	printf("How many times to do wish yo check for system information?\n");
	scanf("%d", &g_iTotalTimes);
	printf("Checking\n");
	g_log = fopen("log.txt", "a+");

	//Inicialización
	g_isBufferFree = 1;
	g_buffer[0] = '\0';
	g_pids[1] = -1;
	g_pids[2] = -1;
	init(&g_queue);

	ShowOnlineTime(); //Crear subproceso
	ShowAvailableRam(); //Crear subproceso
	ShowProcesses(); //Creare subproceso

	


	//3 subrocesos
	while(1)
	{
		printf("Padre: Buffer: [%s]\n", g_buffer);
		/*
		if(g_buffer[0] == '\0')
		{
			continue;
		}
		else
		{
			fprintf(g_log, g_buffer);
			g_buffer[0] = '\0';
			g_isBufferFree = 1;
		}
		if(g_pids[0] == -1
		&& g_pids[1] == -1
		&& g_pids[2] == -1)
		{
			fprintf(g_log, "Programa finalizado\n");
			break;
		}
		*/
	}
	fclose(g_log);
	return 0;
}