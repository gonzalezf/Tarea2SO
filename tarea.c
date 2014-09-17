#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
 /* crear subprocesos mediante fork
  * cada subproceso entregar informacion sobre estado del sistema y enviar resultados al padre mediante
  * memoria compartida
  * 
  * Registrar los resultados, PID del proceso que envia el mensaje y el contenido en el archivo log.txt
  * Cuando hijo termine escribir PID subproveso finalizado
  * CUando padre termine escribir PID Programa finalizado
  * 
  * Cada hijo analiza el sistema n veces, el usuario determina n.
  * Cada hijo toma el tiempo de espera entre cada analisis realizado
  * 
  * Padre creare 3 subprocesos que funcionen en paralelo
  *el primero de ellos entrega tiempo en segundos que lleva encendido el sistema
  * el segundo entrega cantidad de ram disponible sobre la total
  * tercero indica la cantidad de procesos en ejecucion
  * */
 
 int main(){
	 pid_t pid;
	 pid = fork();
	 printf("%d\n", pid);
	 
	 if(pid == -1)
	 {
	 	printf("Error al crear proceso hijo \n");
	 	printf("%s", strerror(errno));
	 	return 0;
	 }
	 if(pid == 0){ /* ejecutar instrucciones del hijo*/
	
		printf("Soy el hijo \n");
		return 0;
	}
	else if(pid >0) /* proceso padre, retorna el PID del hijo*/
	{
	 	printf("Proceso padre, Pid del hijo es %d \n",pid);
		return 0;
	}
	 	 
 }
