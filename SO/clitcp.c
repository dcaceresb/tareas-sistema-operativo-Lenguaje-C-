/*El Cliente*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define MAXBUF  512
#define MAXNAME  10

FILE *fd;

int main (int argc, char *argv[]) 
{
  int    sock, nreceived;
  struct sockaddr_in name;
  struct hostent *hp;
  char   buf[MAXBUF], buffer[MAXBUF], car;

  if ( argc !=4  ) 
  {
   printf("Usar : %s <ip-servidor> <puerto> <accion> \n", argv[0]);
   exit(0);
  }

  	// socket: Crea un punto de comunicación
  	// La función socket retorna un descriptor de archivo (de tipo int)
  sock = socket(AF_INET,SOCK_STREAM,0);
  if ( sock < 0 )
  {
    perror("Error socket() \n");
    printf("socket() %s \n", strerror(errno));

    exit(-1);
  }

  // gethostbyname(argv[1]): retorna una estructura de tipo hostent para un nombre
  // de host dado. En este caso argv[1] es el nombre del host o su dirección IPv4.
  hp = gethostbyname(argv[1]);
  if ( hp == NULL ) 
  {
    perror("\nError en gethostbyname(char *) \n");
    printf("gethostbyname() %s \n", strerror(errno));

    exit(-1);
  
  }

  // Llenado de la estructura name de tipo struct sockaddr_in
  name.sin_family = AF_INET; // Para protocolo de Internet IPv4.
  // bcopy: copia una secuencia de hp->h_length bytes desde la fuente hp->h_addr a
  // name.sin_addr
  bcopy ((char *)hp->h_addr,(char *)&name.sin_addr,hp->h_length);
  // Copiamos el número puerto dado por el server name.sin_port. Con la función 
  // htons transformamos la representación entera del puerto a network-order 
  // (big-endian).
  name.sin_port = htons(atoi(argv[2]));
  // connect: inicia una conexión usando el socket sock a la dirección contenida 
  // en la estructura name.
  int con = connect(sock, (struct sockaddr *)&name, sizeof name);
  if ( con < 0 )
  {
    perror("Error connect() \n");
    printf("connect() %s \n", strerror(errno));

    exit(-1);
  }

  /*send: se envía primero al server el nombre del archivo */
  int sent = send(sock,argv[3],MAXNAME,0);
  if ( sent < 0 )
  {
    perror("Error send() \n");
    printf("send() %s \n", strerror(errno));

    exit(-1);
  }

  char* salida;
  int nbytes;
  while(1)
  {
    
    nbytes = recv(sock,salida,MAXNAME,0);
    if(nbytes!=0)
    {
      int i=0;
      while(salida[i]!='\0')
      {
        printf("%c",salida[i] );
        i++;
      }
      
      break;
    }
  }
  /*
  do 
  {
  	int i;  ///Lee contenido del archivo   	
    for (i=0; !feof(fd) && i<MAXBUF; i++) 
    {
     	// El contenido del archivo se lee byte a byte
      fread(&car,1,1,fd);
      buffer[i] = car;
    }
    buffer[i] = '\0';
    // Envía el contenido del archivo por pedazos (de tamaño 'i' o MAXBUF bytes)
    int sent = send(sock,buffer,i,0);
    if ( sent < 0 )
    {
      perror("Error send() \n");
      printf("send() %s \n", strerror(errno));
      
      exit(-1);
    }
    // Recibe confirmación desde el servidor (en realidad esto no es necesario)
    nreceived = recv(sock,buf,MAXBUF,0);
    if ( nreceived < 0 )
    {
      perror("Error recv() \n");
      printf("recv() %s \n", strerror(errno));

      exit(-1);
    }
    printf("\n-> %s",buf); printf("\n");
    // if ( !strcmp(buf,"Q") ) break; 
  } while ( !feof(fd) );*/

  exit(0);
}
