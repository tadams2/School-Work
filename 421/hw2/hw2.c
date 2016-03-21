#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
/** Menu() - displays a menu to the user then they pick which option to run
 *  @void:      
 *  Return choice:       - what the user chose
 */
int Menu(void){

  int choice = 0;

  printf("1: Display children states \n");
  printf("2: Kill a child \n");
  printf("3: Signal a child \n");
  printf("4: Reap a child \n");
  printf("5: Kill and reap all children \n");
  printf("enter your choice\n");
  if(scanf ("%d", &choice) == 1)
    {
      printf("you chose %d \n", choice);
    }
  
  return choice;
}
/** DisplayChildStates() - displays the states of all the child processess in an array
 *  @childArray[]:       - an integer array of pid's
 *  Return:              - none, void function
 */
void DisplayChildStates(int childArray[]){
  char fileName[80];
  char line[256];
  char *token;
  FILE *openFile;
  int fReadSuccess;
  /**Written part of part 3
   * There are 5 different states in /proc/pid/stat
   * These states are R - running, S - sleeping, D- sleeping in an uninterruptible wait, Z - zombie, T - traced or stopped
   * Source = sites.google.com/site/linuxpebbles/bits-and-pieces/documentation-of-procpidstat
   */
  for(int i = 0; i < 4; i++)
    {
      int childPid = childArray[i];
      snprintf(fileName, sizeof fileName, "/proc/%d/stat", childPid);
      openFile = fopen(fileName, "r");
      // make sure the openFile != NULL, and print N/A if it does (this only happens if that process has been reaped properly)
      if(openFile == NULL)
	{
	  printf("N/A \n");
	}
      else 
	{
	  //read the line in the file (there is only 1 line in /proc/pid/stat)
	  fReadSuccess = fread(&line, 1, sizeof(line), openFile);
	  //so we dont get a warning (/proc/pid/stat will always be populated)
	  fReadSuccess ++;
	  //read in 2 garbage tokens delimted by white space then print the third
	  token = strtok(line, " ");
	  token = strtok(NULL, " ");
	  token = strtok(NULL, " ");
      
	  printf("I am child %d , my pid is %d, and my state is %s \n", i + 1, childArray[i], token);
	  //close the poor file
	  fclose(openFile);
	}
     }

  


}
/** ReapAChild()  - terminates a zombie process and prints the results to the user
 *  @childNum:    - pid of the child to be terminated
 *  @childToAct:  - the child's symbolic number
 *  Return:       - none, void function
 */
void ReapAChild(int childNum, int childToAct)
{
  int waitPidValue = waitpid(childNum, NULL, WNOHANG);
  //if waitpidValue is greater than 0 that means the child was reaped, elsewise the child could not be reaped
  if(waitPidValue > 0)
    {
      printf("child %d has been reaped\n", childToAct);
    }
  else if (waitPidValue == 0 )
    {
      printf("child %d was not reaped\n", childToAct);
    }
  
}

/** PrintChildren() - prints the children to the user, to be used at the start of the program after processes are spawned
 *  @children[]:    - an array of child pid's to be printed
 *  Return:         - none, void function
 */
void PrintChildren(int children[])
{
  //loops through the array then prints the child's number and it's pid
  for(int i = 0; i < 4; i++)
    {
      printf("child # %d PID is %d\n", i + 1, children[i]); 
    }
}
/** SignalHandler() - Handles SIGUSR1 and displays a message to the user whenver it is caught
 *  @sigNum:        - the signal number passed in
 *  Return:         - none, void function
 */
void SignalHandler(int sigNum)
{
  //if we receive SIGUSR1 handle it by printing that we received it
  if(sigNum == SIGUSR1)
    {
      printf("received SIGUSR1\n");
    }
}
/** SignalAChild() - sends SIGUSR1 to a child process and then handles it
 *  @childNum:     - the pid of the selected child from main
 *  Return:        - none, void function
 */
void SignalAChild(int childNum)
{
  //send the signal to the correct child and tell it to display a message
  if(kill(childNum, SIGUSR1) == 0)
    {	    
      SignalHandler(SIGUSR1);
    }
}


int main(void) {
  int NUMCHILDREN = 4;
  int childArray[NUMCHILDREN];
  int sharedChildNum[2];

  size_t pid;
  bool child = false;
  int menuChoice = 0;
  int forkResult;

  pid = getpid();
  printf("the pid of the parent is %zu \n", pid);
  //set up the pipe -1 if the pipe failed to create
  int fd[2];
  if(pipe(fd) == -1)
    {
      perror("pipe broken");
      exit(1);
    }
  
  /*spawns NUMCHILDREN children the child flag is to make sure the children do not make their own processes*/
  for(int i = 0; i < NUMCHILDREN; i++)
    {
      if (child != true)
	{
	  /*child processes*/
	  
	  /*stores the result of the fork*/
	  forkResult = fork();
	  /* if the fork is 0 it means it was successful and we are in the child process, sets flag to true*/
	  if(forkResult == 0)
	    {
	      child = true;
	    }
	  /*when fork is greater than 0 it returns the just spawned child's pid to the parent, we store this in an array for reference*/
	  else if(forkResult > 0)
	    {
	      /*parent process*/
	      childArray[i] = forkResult;
	     
	    }
	}

    }

  
  /*parent*/
  if(child != true)
    {
      /*printing the children*/
      close(fd[0]);  
      PrintChildren(childArray);

      //keep running the menu until we kill and reap all children
      while(true)
	{
	menuChoice = Menu();
	int childToAct = 0;
	if(menuChoice == 1)
	  {
	    DisplayChildStates(childArray);
	  }
	//not working properly
	else if(menuChoice == 2)
	  {
	    printf("which child would you like to kill (1-4): ");
	    if(scanf("%d", &childToAct))
	      {
		printf("you chose to kill child %d\n", childToAct);
		
		//subtract 1 from the chosen child to get its array index then write to the pipe
		sharedChildNum[0] = childArray[childToAct -1];
		//is writing the proper amount of bytes
		if (write(fd[1], &sharedChildNum, sizeof(sharedChildNum)) > 0)
		  {
		    printf("wrote %d bytes\n", fd[1]);
		    printf("wrote %d to the buffer\n", sharedChildNum[0]);
		  }
	      }
	    
	  }
	else if(menuChoice == 3)
	  {
	    printf("which child would you like to signal \n");
	    if(scanf("%d", &childToAct))
	      {
		SignalAChild(childArray[childToAct - 1]);
	      }
	  }
	else if(menuChoice == 4)
	  {
	    printf("which child would you like to reap (1-4): ");
	    //read in which child to reap then reap the child
	    if(scanf("%d", &childToAct))
	      {
		ReapAChild(childArray[childToAct - 1], childToAct);
	      }
	  }
	else if(menuChoice == 5)
	  {
	    /** Written part for part 7
	     * having orphaned processes still running sucks up resources from the machine
	     * Also if you have too many orphaned processes you will run out of valid pid's for new processes
	     */
	    printf("you chose Kill and Reap all Children\n");
	    //loop through all the children
	    for(int i = 0; i < 4; i++)
	      {
		//using signal to kill until kill works properly
		SignalAChild(childArray[i]);
		//rest inbetween commands to ensure everythread gets reaped
		sleep(1);
		//reap the kids
		ReapAChild(childArray[i], i + 1);
	      }
	    //display the states so we know all the children got reaped
	    DisplayChildStates(childArray);
	    //exit program
	    exit(0);
	  }
	}
    }
  /*children*/
  else
    {
      close(fd[1]);
      
        while(1)
	{
	  //if we got something read will be a positive value then we compare the pids
	  //if the pids match then we terminate the process
	  //read is always 0, is not reading for some reason
	  if(read(fd[0], &sharedChildNum, sizeof(sharedChildNum)) > 0)
	    { 
	      printf("made it");
	      printf("got %d", sharedChildNum[0]);
	      if(sharedChildNum[1] == (int)getpid())
		{
		  printf("killing child %d", getpid());
		  exit(0);
		}
	      
	    }
	  else
	    {sleep(1);}
	  

	}
    }	    
  return 0;
}
