// David Esteban Caceres Beain   19008719-0

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

#define READ 0
#define WRITE 1


int buff;
int pipeno[2];

void to_kill(int desc);
void send_pid(int desc);

int main()
{
	buff=0;
	pipe(pipeno);
	
	///hijo1
	if(fork()==0)
	{
		send_pid(pipeno[WRITE]);
	}
	else
	{
		to_kill(pipeno[READ]);
		/// hijo2
		if(fork()==0)
		{
			send_pid(pipeno[WRITE]);
		}
		
		else
		{
			to_kill(pipeno[READ]);
		}

	}
	return 0;
}

void to_kill(int desc)
{
	read(desc,&buff,sizeof(int));
	int killing = buff;
	pid_t pid=(pid_t)killing;
	printf("a matar el proceso con pid: %d\n",killing );
	kill(pid,SIGKILL);
}

void send_pid(int desc)
{
	
	pid_t pid;
	pid=getpid();
	write(desc,&pid,sizeof(int));
}