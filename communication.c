

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>



#define M 12
#define tampon_taille 5

struct wrapper
{
    
    int buffer[tampon_taille];
    sem_t vide;
    //sem_t cmutex;
    sem_t full;
    
};

int id_mem; //id of the mem_partag shared between processes
struct wrapper *mem_partag ;
int rc;
int N = 20;
int in;
int out;


void kill() {

    rc = shmctl(id_mem, IPC_RMID, NULL); //destroy with IPC_RMID after process detaches it
    rc = shmdt(mem_partag); //detach shared mem_partag


    sem_destroy(&mem_partag->vide);
    sem_destroy(&mem_partag->full);
}


void  producer() 
{
     for(int i=1; i<=N; i++)
    {
        

        sem_wait(&mem_partag->vide); 
    
        
        (mem_partag->buffer)[in]=i;
        
        in = (in+1)%tampon_taille;

      
        printf("génération nbr:%d\t \n",i);
        usleep(10000);
        
        sem_post(&mem_partag->full); 
        

    }
}

void  consumer(int * tube)
{
      while(1)
    {
        sem_wait(&mem_partag->full); 
        

        
        int temp = (mem_partag->buffer)[out];
        out = (out+1)%tampon_taille;

        
        printf("Élément supprimé: %d\n",temp);

        
       

    
        sem_post(&mem_partag->vide); 
      


       if (write(*tube, &temp, sizeof(temp)) > 0) {
			
		
		}
		
    }
}


void P3(int * tube) {
	
	// timer pour attendre maximum 5 secondes

    int y;
	while (N>0) {
		// lecture dans le tube
		if (read(*tube, &y, sizeof(y)) > 0) {
            
			printf("\nMessage du processus P3 : %d\n", y);
			
		}else{
            //printf("error reading");
        }
		
        
        
	}
}

int main() {

    

    //creating key for shared mem_partag
    
    key_t sharedMemoryKey = ftok(".", M); //just creating a key for the shared mem_partag
    if (sharedMemoryKey == -1) {
        perror("ftok():");
        exit(1);
    }

    id_mem = shmget(sharedMemoryKey, sizeof(struct wrapper), IPC_CREAT | 0600); //get shared mem_partag segment
    if (id_mem == -1) {
        perror("shmget():");
        exit(1);
    }

    mem_partag = shmat(id_mem, NULL, 0); //attach shared mem_partag segment


    //initialization

    printf("Initialisation!\n");
    memset(&mem_partag->buffer, 0, sizeof(mem_partag->buffer));
    //sem_init(&mem_partag->cmutex, 1, 1);

    sem_init(&mem_partag->vide, 1, tampon_taille); //initilaizing semaphores with 1 means we work with processes not threads
    sem_init(&mem_partag->full, 1, 0);

   
    in = 0;
    out = 0;



    //call producer consumer
    pid_t id1, id2, id3;

    id1 = fork();

    if (id1 == 0) {
        id2 = fork();
        if (id2 == 0) // p2
        {
            int tube[2];
		    pipe(tube);

            id3 = fork();
            if (id3 == -1) {
                // Il y a une erreur
                perror("fork");
                return EXIT_FAILURE;
            } else if (id3 == 0) { //p3
                // On est dans le fils
                close(tube[1]);
                P3(&tube[0]);
                close(tube[0]);
                exit(EXIT_SUCCESS);
            } else { //p2
                
                
                consumer(&tube[1]);
                close(tube[0]);
                close(tube[1]);

                exit(EXIT_SUCCESS);
            }

            
        }else{
            producer();
            exit(EXIT_SUCCESS);
        }
    }else
    { 
        //programme principale
     
       kill();
      return 0;
    }

   



   
 
}

