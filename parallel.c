
#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h> 
#include<unistd.h>
#include<sys/wait.h>
#include<math.h>




FILE *f,*f1,*f2, *f3;

void p1( int c1){  //fils p1

    f = fopen("f.txt","r");
    f1 = fopen("f1.txt","w+");
    
    pid_t id;
    int p, status, premier1;
    //fscanf(f, "%d", &p); //to avoid 2, we only want to put multiples



    while (fscanf(f, "%d", &p) != EOF )  // trouver le nombre P immédiatement superiur a c1----------- first iteration: p = 3
    {
        if(p %c1 != 0 && p>c1)
        {

          break; //we found P, exit the loop
        }
    }

    //p=3


    // p will have this value of P after the fork

  

        if ((id = fork()) < 0) {  //create child p
            perror("p creation failed !\n");
            fclose(f);
            fclose(f1);
            exit(-1);

        }else if(id ==0) //p2
        {

            
            
            f = fopen("f.txt","r");
            f2 = fopen("f2.txt","w+");
            
            int c, p2;

          //  2 3 4 5 6 7 8 9 10 11 12 13 14
            while (fscanf(f, "%d", &c) != EOF) //écrire les multiples de P dans F2
             {
            
                if ((c%p == 0 && c>p)) //p
                {
                    fprintf(f2, "%d  ", c);  
                    //fscanf(f,"%d",&c);

                } 
    
             }

            rewind(f); // return the pointer to the beginning of f.


             // nombre immédiatement sup a P=3
          

           //  2 3 4 5 6 7 8 9 10 11 12 13 14

            while (fscanf(f, "%d", &p2) != EOF ) // trouver le nombre p immédiatement superiur a le P precedent
            {
                if((p2 % p != 0) && (p2 > p )&& (p2 %c1 != 0))
                {
                 break; // we exists p, exit the loop.
                }else{
                  continue;
                }
            }
            //p2 = 5
            printf("\n nombre P immédiatement sup a %d = %d  (trouvé par p1)\n", c1, p);
            printf(" nombre P2 immédiatement sup a p = %d  (trouvé par p2)\n", p2); 
            fclose(f); fclose(f2);
            

            exit(p2); //send the number p to the the process P1--------- first iteration: p = 5

            

            
             
             


    } else { //p1 //en parallel ye7seb multiples de C1 -----first iteration: c1 =2


        int c;
        rewind(f); //pointer to the begining

        while (fscanf(f, "%d", &c) != EOF) //multiples of c1 in f1
        { 
        
            if ((c%c1 == 0 && c > c1)) 
            {
                    fprintf(f1, "%d  ", c);  
                    fscanf(f,"%d",&c);

            }
        

        }

         wait(&status);  //wait for the child process P2 to send the number.--------first iteration: p2 will send 5
        if (WIFEXITED(status))
        {
            // premier1 will have the value received from P2-----first iteration: 5
            premier1 = WEXITSTATUS(status);  printf("\n P1 a recu %d de P2\n", WEXITSTATUS(status));
        }else{
            printf(" \n\np didn't exit normally: %d\n\n", WEXITSTATUS(status));
        }
    }
    
    fclose(f); fclose(f1);
    //premier1 = 5
    
    exit(premier1);  // now P1 will send the number received from p2 (5 in first iteration) to the main Process (to update the variable "Premier" and iterate)

}





void fill(int n){  
int r;
	f=fopen("f.txt","w+");

	if(f==NULL){
	printf("error\n");
	exit(1);}
	
for(int i=2;i<=n;i++){

	fprintf(f," %d \n",i);
	}
		fclose(f);
}



void printFile() 
{
    int primeNumber;
    f = fopen("f.txt", "r");
    if (f == NULL) // In case we could not open the file.
    {
        printf("error\n");
        exit(1);
    }
    while (fscanf(f, "%d", &primeNumber) != EOF)
    {
        printf("%d  ", primeNumber);
    }
    printf("\n");
    fclose(f);
}

void printF2() 
{
    int primeNumber;
    f2 = fopen("f2.txt", "r");
    if (f2 == NULL) // In case we could not open the file.
    {
        printf("error\n");
        exit(1);
    }
    while (fscanf(f2, "%d", &primeNumber) != EOF)
    {
        printf("%d  ", primeNumber);
    }
    printf("\n");
    fclose(f2);
}

void printF1() 
{
    int primeNumber;
    f1 = fopen("f1.txt", "r");
    if (f1 == NULL) // In case we could not open the file.
    {
        printf("error\n");
        exit(1);
    }
    while (fscanf(f1, "%d", &primeNumber) != EOF)
    {
        printf("%d  ", primeNumber);
    }
    printf("\n");
    fclose(f1);
}




void main(){
    

    int id, n, status, premier = 2; 

  	printf(" insert n  \n");
	scanf("%d",&n);
    int sqr = sqrt(n);  
    printf("\nracine de n: %d\n\n", sqr); 
    fill(n);

   
   


    while(premier <= sqr) 
    {
        printf("________________________________________");
        printf("\n PREMIER est maintenant = %d\n", premier);

        
        id = fork();  // create p1
        if(id <0)
        {
            printf("error fork failed\n");
            exit(-1);
        }else if(id == 0){ 
            
            
            p1(premier); // premier = 2 in the beginning, and then 5, then 7 ... 
        
        }else
        {
            
            wait(&status); //wait for p1 to send the next prime number------- first iteration: rah yejih 5, then we update premier =5 to iterate again
            if (WIFEXITED(status))
            {
                sleep(3);
                //update premier
                premier = WEXITSTATUS(status);   printf("\n Processus Parent a recu: %d de P1", premier );
                
            }else{
                printf("\n P1 didn't exit normally: %d\n", WEXITSTATUS(status));
            }

            printf("\n processus Parent continue...");  
          


           // printFile();
            printf("\n\n");
           // printF3();

           printf("f1 contient: ");
           printF1();

           printf("\nF2 contient: ");
           printF2();
            


            //update f
            f = fopen("f.txt", "r");
            f3 = fopen("f3.txt", "w"); //temporaire

            int exists = 0, f_number, f1_number, f2_number;

            
            // remove all the numbers of F1 and F2 from F, put the result in a remporary file F3 and then copy it back to F at the end
            while(fscanf(f, "%d", &f_number) != EOF && exists == 0)  //update f 
            {
                f1 = fopen("f1.txt", "r"); 
                f2 = fopen("f2.txt", "r");

                if (f1 == NULL || f2 == NULL) // In case we could not open the file.
                 {
                    printf("error creating f1, f2\n");
                    exit(1);
                }else
                {
                    
                    while (fscanf(f1, "%d", &f1_number) != EOF && exists == 0 ) //check if f_number exists in f1
                    {
                        if (f_number == f1_number)
                        {
                            exists = 1;
                        }
                    }
                    
                    while (fscanf(f2, "%d", &f2_number) != EOF && exists == 0) //check if f_number exists in f2
                    {
                        if (f_number == f2_number)
                        {
                            exists = 1;
                            
                        }
                    }
                    // If the value does not exist in both files f1 & f2 then we will add the value into the temp file F3.
                    if (exists == 0)
                    {
                        fprintf(f3, " %d\n", f_number);
                    }

                    exists = 0;  // Set it to 0 for the next iteration.
                    fclose(f1); 
                  fclose(f2); 
               
                }
                  
         

            }

            
          

            fclose(f);
            fclose(f3);

            f3 = fopen("f3.txt", "r");
            f = fopen("f.txt", "w+");

            if (f == NULL || f3 == NULL) // In case we could not open the file.
            {
                printf("error\n");
                exit(1);
            }else
            {

           

            // update F: copy f3 into F 
            while (fscanf(f3, "%d", &f_number) != EOF)
            {
                fprintf(f, " %d\n", f_number);
            }
            fclose(f);
            //printFile();
            fclose(f3);
             }

            
        
        }

    }
    printf("\n _______________");
    printf("\n Boucle Terminé!\n");
    printf(" _______________\n");
    

    printf("\n les nombres premiers sont: \n");
    printFile();
    exit(0);
    

}
