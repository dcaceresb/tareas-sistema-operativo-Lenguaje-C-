// David Caceres 19008719-0
#include <sys/types.h>
#include <sys/dir.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define MAX_PATH 1000

int espacio(char *,long long *);

int main(int argc, char *argv[]) {
     	int sz = 0;
     	DIR 	*dir;
     	struct dirent *d;
     	struct stat st;

     	if ( argc != 1 ) {
     		
     	}
     	else
     	{
     		printf("Use %s <directorio>\n",argv[0] );
     		exit(1);
       	}
     	
     	dir = opendir(argv[1]);
    	//strcat(argv[1],"/");
     	
     	stat(argv[1],&st);
    	if ( !S_ISDIR(st.st_mode) ) {
		printf("Debe ingresar el nombre de un directorio\n");
		printf("En todo caso, el tamaño del archivo %s es %d bytes\n",argv[1], (int)st.st_size);
      	exit(0);
      }
      d=readdir(dir);
      long long tam=0;
     	printf("El numero de archivos para el directorio %s es %d \n",d->d_name ,espacio(argv[1],&tam));
     	printf("Bytes: %lld\n",tam );
     	float kilo = (float)tam/1000;
     	printf("KB: %f\n",kilo );
     	float mega = kilo/1000;
     	printf("MB: %f\n",mega);

     	exit(0);
}

int espacio(char *name_dir,long long *tam) { 
    	struct stat st;
    	struct dirent *fd;
    	DIR *dirp;
    	int counter=0;
    	char 		buf[MAX_PATH+1];
    	
    	if( stat(name_dir, &st) != 0 )
          	perror(strerror(errno)); //error("Can't stat %s", name_dir);
    	if( !S_ISDIR(st.st_mode) )
           	return;

	if( (dirp = opendir(name_dir)) == NULL) {
        	error("Can't open directory %s", name_dir);
		perror(strerror(errno));
      }
      
      if ( getcwd(buf, MAX_PATH+1) == NULL )
        	error("Can't get cwd");
      if ( chdir(name_dir) != 0 )
        	perror(strerror(errno)); //error("Can't chdir");
	
    	while ( (fd=readdir(dirp)) != NULL ) 
    	{
        	stat(fd->d_name,&st);
        	if ( (strcmp(fd->d_name,".") != 0) && (strcmp(fd->d_name,"..") != 0) ) {
        		if ( !S_ISDIR(st.st_mode) )
        		{
        			 counter++;
        			*tam+=(long long)st.st_size;
        		}
        		else {
            	 	//dir = opendir(fd->d_name);
	     			//chdir(fd->d_name);
            	 	*tam+=(long long)st.st_size;
            	 	counter++;
            	 	counter += espacio(fd->d_name,tam);
           		}
           	}
     	}
    	closedir(dirp);
   	chdir(buf);
    	
    	return counter;
}
