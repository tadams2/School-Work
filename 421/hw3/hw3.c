/**
 * @Author Ted Adams (tadams2@umbc.edu)
 * this file is a program that will simulate stock trades with parameters read in from a file 
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//declaring globals so all can see
pthread_mutex_t lock;
//initialize array to max amount (we will never loop through all elements unless there 65535 real elements
int fundArray[65535];
FILE *fp;

/**
 * *PthreadRunner() function that the created threads will run
 * @void *id: an identifer for a thread
 * Return: none, a void function
 */
void *PthreadRunner(void *id)
{
  char line[80];
  int myID =  *(int *) id;
  //to prevent race condition for which thread reads which line (this lock only applies initial pass of the while loop)
  if (pthread_mutex_lock(&lock) == 0)
    {}
  while( fgets(line, 80, fp) != NULL)
    {

      //unlocks after it gets a line (is locked from previous iteration until it gets a line)
      if(pthread_mutex_unlock(&lock) == 0) {}
      
      //parse the line with strtok()
      char* source = strtok(line, " ");
      char* target = strtok(NULL, " ");
      char* amount = strtok(NULL, " ");
      char* requiredTime = strtok(NULL, " ");

      //change strings to ints
      int intSource = atoi(source);
      int intTarget = atoi(target);
      int intAmount = atoi(amount);
      int intRequiredTime = atoi(requiredTime);


      /*******************************************************************************************************************************************
       *                                                             Written part                                                                *
       * My scheduler uses a lowest time first algorithm as shown by the output (the smaller jobs finish before any long jobs                    *
       * I do this by calculating sleep time then releasing the lock for the sleep operation, this lets threads with short jobs jump in and      *
       * finish while longer jobs are still sleeping. So in short the "order of operations" is a thread reads a line (protected by a lock) then  * 
       * checks for available funds in source if there isnt the thread gives up the lock. If there is the thread aquires the lock, subtracts the *
       * amount, calculates sleep time. Then the thread gives up the lock and goes to sleep, after it sleeps it gets the lock back then adds     *
       * funds to the target and prints the transaction.                                                                                         *
       * Note: the thread unlocks after it gets the line then parses the information before getting the lock back, this is done for efficency    *
       *******************************************************************************************************************************************
       */


      //check if there are enough funds in the source if there isnt enough funds we release the lock
      while(fundArray[intSource] < intAmount)
	{
	  if(pthread_mutex_unlock(&lock) == 0){}
	}

      //if so aquire lock and subtract funds
      if(pthread_mutex_lock(&lock) == 0)
	{}
      fundArray[intSource] -= intAmount;

      //calculate sleep time
      int  realSleep = (rand() % intRequiredTime);

      if(pthread_mutex_unlock(&lock) == 0) {}
      //sleep (rand() % requiredTime)
      if(sleep(realSleep) == 0)
	{}

      if(pthread_mutex_lock(&lock) == 0) {}

      //add funds to target
      fundArray[intTarget] += intAmount;
      
      //debugging print
      printf("trader %d moving: %d, from %d to fund %d (required time = %d, real= %d)\n",myID, intAmount, intSource, intTarget, intRequiredTime, realSleep);
      
      
    }

  //unlock the mutex one last time so we dont deadlock when one thread is done
  if(pthread_mutex_unlock(&lock) == 0)
    {}
  return 0;
}

/**
 * main() - runs the program
 * int argc, char *argv[] - command line arguements for number of arguements and the arguements in an array
 * Returns - 0 on completion, -1 on error
 */
int main(int argc, char *argv[])
{
  //initilizing variables, mutex, and srand()
  pthread_mutex_init(&lock, NULL);
  char *fileName = argv[1]; 
  char line[80];
  int s = 0;
  int t = 0;
  int f = 0;
  int seed = atoi(argv[2]);
  srand((unsigned) seed);

  //handling amount of commandline arguements
  if(argc < 3)
    {
      printf("not enough arguements, program exiting\n");
      return -1;
    }

  else if(argc > 3)
    {
      printf("too many arguements\n");
      return -1;
    }

  else
    {
      //prints out the commandline args and opens the file
      printf("filename is %s, and the seed is %s\n", fileName, argv[2]);
      fp = fopen(fileName, "r");
      if(fp < 0)
	{
	  printf("opening file failed");
	}
      
      //if we are not at an EOf (which we wont be its the first few lines)
      if (fgets (line, 80, fp) != NULL)
	{
	  //number of traders (or threads) to create
	  s = atoi(line);
	  printf("Number of stock traders: %d\n", s);
	}

      //reads the second line and parses it into a variable
      if (fgets (line, 80, fp) != NULL)
	{
	  //number of stocks
	  f = atoi(line);
	  printf("Number of stock funds: %d\n", f);
	}

             
      //reads third line and puts it into a variable
      if (fgets (line, 80, fp) != NULL)
	{
	  //number of transactions (not really used for my implementation)
	  t = atoi(line);
	  printf("Number of stock transactions: %d\n", t);
	}

      printf("Initial amounts of each fund: \n");
      for(int i = 0; i < f; i++)
	{
	  //prints out the amounts of each fund, read from the file
	  if (fgets (line, 80, fp) != NULL)
	    {
	      fundArray[i] = atoi(line);
	      printf("fund %d: %d\n", i, fundArray[i]);
	    }
	}
      //spawn threads
      pthread_t pthreadArray[s];
      for(int i = 0; i < s; i++)
	{
	  if(pthread_create(&pthreadArray[i], NULL, &PthreadRunner, &i) != 0)
	    {
	      printf("error on creating thread");
	      return -1;
	    }
	}
      //wait till all threads are done then join
      for(int i = 0; i < s; i++)
	{
	  if(pthread_join(pthreadArray[i], NULL) != 0)
	    {
	      printf("error joining");
	    }
	}
      //print ending amounts
      for(int i = 0; i < f; i++)
	{
	  printf("fund #%d: %d \n", i, fundArray[i]);
	}
    }
  if(fclose(fp) == EOF)
    {
      printf("error closing file");
    }

  
  return 0;
}
