// David caceres 19008719-0


/*
 * prodcons.c
 *
 * Autor: Pedro A. Rodríguez M.
 * Curso: Sistemas Operativos
 *
 * Test N° 2 de laboratorio
 *
 * El problema de Productores y Consumidores con emulación de monitores
 *
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MaxBuffer 100
#define N 20
#define M 5

typedef struct {
	int buffer[MaxBuffer];
	int in, out;
  // practicamente con el largo se puede reemplazar in,out,full,empty
  int largo;
	int full, empty;
	pthread_mutex_t mutex;
	pthread_cond_t notFull, notEmpty;
} mon_buffer;

pthread_mutexattr_t     attr_mutex;
pthread_attr_t		atrib_thread_prod, atrib_thread_cons;

bool flag = true;

void *thread_exit() {
    long status;
    char c;

    do {
        printf("%c[%d;%dH Ingrese 'q' para terminar y luego <enter>",27,15,5);
        scanf("%c",&c);
        if ( c == 'q' ) flag = false;
    } while ( flag );

   pthread_exit(&status);
 }

void *productor(mon_buffer *monitor) 
{
  
	while ( flag ) 
  {
    //espera a que este disponible el monitor para añadir el elemento sin sobreescribir o errores
    pthread_mutex_lock(&monitor->mutex);

    if(monitor->largo==MaxBuffer)
    {
      //-- ya que deberia estar en 100, y asi lo dejamos en 99
      monitor->out--;
      pthread_cond_wait(&monitor->notFull, &monitor->mutex);

    }
		int item = random();
    monitor->buffer[monitor->in]=item;
    monitor->in=(monitor->in+1)%100;
    monitor->out=(monitor->out+1)%100;
    printf("Produciendo: %d\n",item );
    if(monitor->largo!=100)
    {
      monitor->largo++;
    }
    // luego de asignar un elemento, despierta a un consumidor
    pthread_cond_signal(&monitor->notEmpty);
    pthread_mutex_unlock(&monitor->mutex);

		
	}

	pthread_exit(NULL);
}

void *consumidor(mon_buffer *monitor)
{
  

	while ( flag ) {
	  //espera a que este disponible el monitor para añadir el elemento sin sobreescribir o errores
    pthread_mutex_lock(&monitor->mutex);
    

    if(monitor->out==-1)
    {
      monitor->largo=0;
      monitor->in=0;
      monitor->full=0;
      monitor->empty=1;
      pthread_cond_wait(&monitor->notEmpty, &monitor->mutex);

    }
    printf("Consumiendo: %d \n",monitor->buffer[monitor->out] );
    monitor->buffer[monitor->out]=0;
    monitor->in=(monitor->in-1)%100;
    monitor->out=(monitor->out-1)%100;
    if(monitor->largo!=0)
    {
      monitor->largo--;
    }

    pthread_cond_signal(&monitor->notFull);
    pthread_mutex_unlock(&monitor->mutex);
	}

	pthread_exit(NULL);
}


int main(int argc, char **argv) 
{
	pthread_t th_prod[N], th_cons[M], th_exit;
	mon_buffer *monitor;
  int i,status;

  /* Inicializamos los atributos de los threads costureras */
  pthread_attr_init(&atrib_thread_prod);
  pthread_attr_setscope(&atrib_thread_prod, PTHREAD_SCOPE_SYSTEM);
  pthread_attr_init(&atrib_thread_cons);
  pthread_attr_setscope(&atrib_thread_cons, PTHREAD_SCOPE_SYSTEM);
  	
  monitor = (mon_buffer *)malloc(sizeof(mon_buffer));
  monitor->largo=0;
  monitor->in=0;
  monitor->out=-1;

  /* Inicializamos los mutex para controlar acceso a las secciones críticas */
  pthread_mutexattr_init(&attr_mutex);
  pthread_mutex_init(&monitor->mutex, &attr_mutex);

	pthread_mutex_init(&monitor->mutex, NULL);
	pthread_cond_init(&monitor->notFull, NULL);
	pthread_cond_init(&monitor->notEmpty, NULL);

  /* Creamos los threads */
  for(i=0;i<N;i++)
  {
    status = pthread_create(&th_prod[i],NULL,(void *)productor,monitor);
    if ( status != 0 ) 
    { /* Esto se debería hacer con todos los threads */
      printf("Create thread\n");
     exit(0); 
    }
  }

  for(i=0;i<M;i++)
  {
    status = pthread_create(&th_cons[i],NULL,(void *)consumidor,monitor);
    if ( status != 0 ) 
    { /* Esto se debería hacer con todos los threads */
      printf("Create thread\n");
     exit(0); 
    }
  }

  /* Creamos el thread que espera el caráter 'q' para terminar */
  status = pthread_create(&th_exit,NULL,(void *)thread_exit,NULL);
  if ( status != 0 ) 
  { /* Esto se debería hacer con todos los threads */
    printf("Create thread\n");
   exit(0); 
  }
  
  /* Esperamos que el thread eque espera el carácter 'q' termine */
  status = pthread_join((pthread_t)th_exit,(void **)NULL);
  if ( status != 0 ) 
  {
    printf("Join thread exit\n");
    exit(0);
  }
 }
