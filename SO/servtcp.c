/*Servidor Concurrente*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <signal.h>
#include <netinet/in.h>
#include <errno.h>
#define MAXBUF 512
#define MAXNAME 4

char opcion[MAXNAME];
char str_pid[12];
int  count=0;
char tarea[8];
void servidor_esclavo(int sock);
char buffer[MAXBUF];
struct sockaddr_in name;
int lenght;

int main () 
{


	void ctrlc();
	int sock,s,n,i,nbytes;

	char buf[MAXBUF];

	// Programa la rutina o función de atención de interrupciones.
	signal (SIGINT,ctrlc);
	// socket: Crea un punto de comunicación
	// La función socket retorna un descriptor de archivos (de tipo int)
	// Este socket se usa sólo para aceptar conexiones (listener)
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		perror("Error en socket(int, int, int) \n");
		perror(strerror(errno));
		printf("socket() %s \n", strerror(errno));
		exit(-1);
	}

	// Llenado de la estructura name de tipo struct sockaddr_in
	name.sin_family = AF_INET; // Para protocolo de Internet IPv4.
	// El socket listener es enlazado usando la constante INADDR_ANY
	// a cualquier dirección IP del host o máquina donde se ejecuta el server. 
	// Esto sirve cuando el host tiene más de una interface o dirección IP asignada.
	name.sin_addr.s_addr = INADDR_ANY;
	// El puerto se inicializa en 0 para que el SO le asigne uno.
	name.sin_port = 0;
	// Enlaza el nombre o estructura 'name' al socket 'sock' (Asignando nombre y puerto al socket)
	int b = bind(sock,(struct sockaddr *)&name, sizeof name);
	if( b == -1 ) 
	{
		perror("Error en bind() \n");
		perror(strerror(errno));
		printf("bind() %s \n", strerror(errno));
		exit(-1);
	}
	
	// Calcula tamaño en bytes de la estructura name.
	lenght = sizeof name;
	// getsockname: retorna la dirección a la cual el socket 'sock' está
	// ligado. Este dirección queda en la estructura 'name'. También asigna 
	// el puerto real de coomunicación.
	int sock_name = getsockname(sock, (struct sockaddr *)&name, &lenght);
	if ( sock_name == -1 ) 
	{
		perror("Error en getsockname() \n");
		printf("getsockname() %s \n", strerror(errno));
		exit(-1);
	}
	printf ("sockName: %d\n", sock_name);
	
	// ntohs: convierte representación de red a representación del host local.
	printf ("El puerto asociada al socket es %d\n", ntohs(name.sin_port));
	printf ("El servidor esta  OK, Para bajarlo  Presione Control C\n");
	// Define el tamaño máximo de la cola de espera para mensajes de entrada
	int l = listen (sock, 5);
	if( l < 0 ) 
	{
		perror("Error en listen() \n");
		printf("listen() %s \n", strerror(errno));
		exit(-1);
	}
	
	while(1) 
	{
		// accept: espera por conexiones remotas de clientes. Retorna un
		// descriptor de socket con la cual un proceso esclavo se va a
		// comunicar con un cliente.
		s = accept(sock,0,0);
		if ( s < 0 ) {
			perror("Error en accept() \n");
			printf("accept() %s \n", strerror(errno));
			exit(-1);	
		}
		int pid = fork();
    		if ( pid < 0 ) {
    			perror("Error en fork() \n");
    			printf("fork() %s \n", strerror(errno));
			exit(-1);
    		}
    		if ( pid == 0 ) servidor_esclavo(s);
	}

	exit(0);
}

void servidor_esclavo(int sock) 
{
	int i,nbytes;
	int pid,sent;
	char* ipv4;
	printf("Creo proceso hijo!!\n");
	/*Lee nombre del archivo maximo 10 caracteres*/
	nbytes = recv(sock,opcion,MAXNAME,0);
	if(strcmp(opcion,"pid")==0)
	{
		printf("se pide el pid\n");
		pid=getpid();
		sprintf(str_pid, "%d", pid);
		printf("%s\n",str_pid );
		sent = send(sock,str_pid,12*sizeof(int),0);
	}
	else
	{
		printf("se pide la ip\n");
		sent = send(sock,"ip",lenght,0);
	}
	

	if ( nbytes <= 0 ) 
	{
		close(sock); 
		exit(0);
	}
	while (1) 
	{
    	nbytes = recv(sock,buffer,MAXBUF,0);
    	if ( nbytes <=0 ) 
    	{
			close (sock);
			printf ("Fin de la Conexion\n");
			break;
      	}
    	buffer[nbytes]='\0';
	}

	exit(0);
}

void ctrlc() 
{
	printf("\nServidor is OUT\n");
	exit(0);
}

