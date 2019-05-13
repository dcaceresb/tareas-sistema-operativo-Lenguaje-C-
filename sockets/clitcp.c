/*El Cliente*/
// david caceres 19008719-0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

typedef struct package {
        int pid;
        float pi;
        char mensaje[10];
}pack;

int main (int argc, char *argv[]) {
    	int    sock, nreceived;
    	struct sockaddr_in name;
    	struct hostent *hp;

    	if ( argc < 2 ) {
        	printf("Usar : %s <ip-servidor> <puerto> \n", argv[0]);
        	exit(0);
    	}

    	sock = socket(AF_INET,SOCK_STREAM,0);
    	if ( sock < 0 ){
		perror("Error socket() \n");
		printf("socket() %s \n", strerror(errno));

		exit(-1);
	}

    	hp = gethostbyname(argv[1]);
    	if ( hp == NULL ) {
		perror("\nError en gethostbyname(char *) \n");
		printf("gethostbyname() %s \n", strerror(errno));

		exit(-1);
	}

    	name.sin_family = AF_INET; 
    	bcopy ((char *)hp->h_addr,(char *)&name.sin_addr,hp->h_length);
    	name.sin_port = htons(atoi(argv[2]));
    	int con = connect(sock, (struct sockaddr *)&name, sizeof name);
    	if ( con < 0 ){
		perror("Error connect() \n");
		printf("connect() %s \n", strerror(errno));

		exit(-1);
	}
	

    	int pid = (int)getpid();

        pack nuevo;
        nuevo.pid=pid;
        nuevo.pi=3.1415;
        strcpy(nuevo.mensaje,"hola");
        //nuevo.mensaje[0]="hola";

    	printf("pic: %d\n", pid);
    	
    	/*send: se envía PID del proceso al servidor */
    	int sent = send(sock,&nuevo,sizeof(pack),0);
    	if ( sent < 0 ){
		perror("Error send() \n");
		printf("send() %s \n", strerror(errno));
		
		exit(-1);
	}

    	exit(0);
 }
