#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//tempos em segundos
#define GREEN_TIME 10
#define YELLOW_TIME 5
#define DEAD_TIME 1

pthread_t id_sem1, id_sem2;
sem_t S;
int instant=0, first=0;

void * thread_semaforo(void *t_id){
	int sem_id=(int)t_id;
	int state=0, sem_value;

	while(1){
	sem_getvalue(&S, &sem_value);
		switch(state){
			case 0:
			if(!sem_value || !first){
				if(!first)printf("instante %d\n", instant);
				first=1;
				printf("  Semáforo %d vermelho\n", sem_id);
				sem_wait(&S);
				state++;
			}	
			break;
			case 1:
			printf("  Semáforo %d verde\n", sem_id);
			usleep(GREEN_TIME*1000000);
			state++;
			break;
			case 2:
			instant++;
			printf("instante %d\n", instant);
			printf("  Semáforo %d amarelo\n", sem_id);
			usleep(YELLOW_TIME*1000000);
			state++;
			break;
			case 3:
			instant++;
			printf("instante %d\n", instant);
			printf("  Semáforo %d vermelho\n", sem_id);
			usleep(DEAD_TIME*1000000);
			sem_post(&S);
			state=0;
			break;
			default:
			break;
		}
	}
	
	pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
	int sem_id1=1, sem_id2=2;
	
	sem_init(&S, 0, 1);
	
	pthread_create(&id_sem1, NULL, (void *)thread_semaforo, (void *)sem_id1);
	pthread_create(&id_sem2, NULL, (void *)thread_semaforo, (void *)sem_id2);
	while(1);
	return 0;
}
