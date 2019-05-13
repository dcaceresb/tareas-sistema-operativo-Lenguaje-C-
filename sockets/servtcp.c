/*Servidor Concurrente*/
// David Caceres	19008719-0
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
#define MAXNAME 10
#define READ  0
#define WRITE 1

typedef struct package {
        int pid;
        float pi;
        char mensaje[10];
}pack;

FILE *fd;
int  count=0;
char tarea[8];
void esclavo(int sock);

int main () {
	void ctrlc();
	int sock,s,n,lenght,i,nbytes;
	struct sockaddr_in name;

	// Programa la rutina o función de atención de interrupciones.
	signal (SIGINT,ctrlc);
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		perror("Error en socket(int, int, int) \n");
		printf("socket() %s \n", strerror(errno));
		exit(-1);
	}

	name.sin_family = AF_INET;
	///
	name.sin_addr.s_addr = INADDR_ANY;
	///
	name.sin_port = 0;
	int b = bind(sock,(struct sockaddr *)&name, sizeof name);
	if( b == -1 ) {
		perror("Error en bind() \n");
		printf("bind() %s \n", strerror(errno));
		exit(-1);
	}

	lenght = sizeof name;
	int sock_name = getsockname(sock, (struct sockaddr *)&name, &lenght);
	if ( sock_name == -1 ) {
		perror("Error en getsockname() \n");
		printf("getsockname() %s \n", strerror(errno));
		exit(-1);
	}
	printf ("sockName: %d\n", sock_name);
	printf ("El puerto asociada al socket es %d\n", ntohs(name.sin_port));
	printf ("El servidor esta  OK, Para bajarlo  Presione Control C\n");
	int l = listen (sock, 5);
	if( l < 0 ) {
		perror("Error en listen() \n");
		printf("listen() %s \n", strerror(errno));
		exit(-1);
	}

	while(1) {
		s = accept(sock,0,0);
		if ( s < 0 ) {
			perror("Error en accept() \n");
			printf("accept() %s \n", strerror(errno));
			exit(-1);
		}
		int child = fork();
    		if ( child < 0 ) {
    			perror("Error en fork() \n");
    			printf("fork() %s \n", strerror(errno));
			exit(-1);
    		}
    		if ( child == 0 ) esclavo(s);
	}

	exit(0);
}

void esclavo(int sock) {
	int i, nbytes, pid;
	pack recibir;
	printf("Creo proceso hijo!!\n");
	nbytes = recv(sock,&recibir,sizeof(recibir),0);

	printf("Identificador de proceso recibido %i \n",recibir.pid);
	printf("Identificador de proceso recibido %f \n",recibir.pi);
	printf("Identificador de proceso recibido %s \n",recibir.mensaje);
	exit(0);
}

void ctrlc() {
	printf("\nServidor is OUT\n");
	exit(0);
}

