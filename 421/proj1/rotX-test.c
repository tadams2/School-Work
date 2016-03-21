/** 
 *  @Author Ted Adams (tadams2@umbc.edu)  
 *  This file tests the two misc character devices rotX and rotXctl
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
char buf[4096];							  
int rotXfd;
int rotXctlfd;
void *dest;
char buf2[4190];
char buf3[6];

/** testCase1() inital test case for the rotX's modules, makes sure we can write and read appropriatly from the devices
 *
 *  @writeToRotX - string representation of a number to write to /dev/rotX
 *  Return:        0 upon success or negative on error
 */
int testCase1 ( char *writeToRotX )
{
  	if ( write ( rotXfd, writeToRotX, 10) > 0)
	  {
	    printf("write to /dev/rotX successful\n");
	  }
	else
	  {
	    printf("write to /dev/rotX unsuccessful\n");
	    return -1;
	  }
	if(close(rotXfd) < 0)
	  {
	    printf("error closing /dex/rotX\n");
	    return -1;
	  }
	rotXfd = open ("/dev/rotX", O_RDWR);
	
	if (rotXfd < 0)
	  {
	    printf("error opening /dev/rotX\n");
	    return -1;
	  }
	/* reading what we wrote */
	if ( read ( rotXfd, &buf, 4095) > 0)
	  {
	    
	    printf("result: %s \n", buf);
	  }
	else
	  {
	    printf("read from /dev/rotX failed\n");
	    return -1;
	  }


	return 0;
}
/** 
 *  testCase2() tests various encryption values on various strings
 *
 *  @stringToWrite - string to be encrypted
 *  @encKey        - encryption key to be given to /dev/rotX
 *  @goString      - string to signal when to encrypt 
 *  Return:          0 upon success or negative on error
 */
int testCase2(char *stringToWrite, char *encKey, char *goString)
{
  
        dest = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, rotXfd, 0);

	if(dest == MAP_FAILED)
	  {
	    fprintf(stderr, "mmap() error\n");
	    exit(1);
	  }
	
	if ( write ( rotXfd, encKey, 10) < 0)
	  {
	    printf("write to /dev/rotX unsuccessful\n");
	    return -1;
	  }
		
	strcpy(dest, stringToWrite);
	
	
	if ( write ( rotXctlfd, goString, 10) < 0)
	  {
	    printf("write to /dev/rotX unsuccessful\n");
	    return -1;	    
	  }
	
	strcpy(buf, dest);
	printf("encrypted string: %s\n", buf);
	munmap(dest, 4096);
	return 0;


}
/**
 * threadRunner() - function to preform thread actions, mmaps hello, writes an encryption key of 1 and sends the go phrase
 * @args          - ignored
 * Return:        - NULL
 */
void *threadRunner(void *args)
{
        dest = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, rotXfd, 0);

	if(dest == MAP_FAILED)
	  {
	    fprintf(stderr, "mmap() error\n");
	    exit(1);
	  }

	if ( write ( rotXfd, "1", 10) > 0)
	  {
	    
	  }
	else
	  {
	    
	    return NULL;
	  }
	
	strcpy(dest, "hello");
	
	
	if ( write ( rotXctlfd, "go", 10) > 0)
	  {
	    
	  }
	else
	  {
	    
	    return NULL;
	  }
	strcpy(buf, dest);
	strcpy(buf3, "ifmmp");
	
	if(strcmp(buf,buf3) != 0)
	  {
	    printf("threading error\n");
	  }
	
	munmap(dest, 4096);
  return NULL;
}

int main(void) {
  
  
	printf("Hello, world! welcome to the rotX test cases!\n");
	/* TEST CASES*/
	printf("first opening both /dev/rotX and /dev/rotXctl\n");
	rotXfd = open ("/dev/rotX", O_RDWR);
	
	if (rotXfd < 0)
	  {
	    printf("error opening /dev/rotX\n");
	  }
	else
	  {
	    printf("opened /dev/rotX successfully\n");
	  }

	rotXctlfd = open ("/dev/rotXctl", O_WRONLY);
	if (rotXctlfd < 0)
	  {
	    printf("error opening /dev/rotXctl\n");
	  }
	else
	  {
	    printf("opened /dev/rotXctl successfully\n");
	  }
	
	/* test case 1: basic case, writing and reading to /dev/rotX */
	/*inital read */
	if ( read ( rotXfd, &buf, 4095) > 0)
	  {
	    
	    printf("nothing has been writting to /dev/rotX yet so should be 0, result: %s \n", buf);
	  }
	else
	  {
	    printf("read from /dev/rotX failed\n");
	  }
	/*testing 13, basic case*/
	printf("writing 13, expected 13\n");
	if( testCase1("13") < 0)
	  {
	    printf("test case 1 broke\n");
	  }
	
	/* testing 26 should be 0 */
	printf("writing 26, expected 0\n");
	if ( testCase1("26") < 0)
	  {
	    printf("test case 1 broke\n");
	  }
	printf("writing -23 expecting error\n");
	if ( testCase1("-23") < 0)
	  {
	    printf("error writing negative value\n");
	  }
	/*end test case 1*/
	/* establishing mmap*/
	if(close(rotXfd) < 0)
	  {
	    printf("error closing /dex/rotX\n");
	    return -1;
	  }
	rotXfd = open ("/dev/rotX", O_RDWR);
	
	if (rotXfd < 0)
	  {
	    printf("error opening /dev/rotX\n");
	    return -1;
	  }
	
	/*running test case 2*/
	printf("encryptings hello with a key of 13, output should be uryyb\n");
	if(testCase2("hello", "13", "go") < 0)
	  {
	    printf("test case 2.1 failed\n");
	  }

	printf("encrypting Hello World with a key of 20, output should be Byffi Qilfx\n");
	if(testCase2("Hello World", "20", "google") < 0)
	  {
	    printf("test case 2.2 failed\n");
	  }

	printf("absdfsd with a bad go signal (shouldnt encrpyt)\n");
	if(testCase2("absdfsd", "3", "Go") < 0)
	  {
	    printf("test case 2.3 failed\n");
	  }

	printf("testing HeLlO - wOrLd with a key of 254 should output ByFfI - qIlFx\n");
	if(testCase2("HeLlO - wOrLd", "254", "goooooooooo") < 0)
	  {
	    printf("test case 2.4 failed\n");
	  }

	printf("testing //2342// with a key of 40, should output the same message (not alphabet characters)\n");
	if(testCase2("//2342//", "40", "go") < 0)
	  {
	    printf("test case 2.5 failed\n");
	  }

	printf("testing hello with a key of 1 and bad go signal of 'g' should not encrypt\n");
	if(testCase2("hello", "1", "g") < 0)
	  {
	    printf("test case 2.6 failed\n");
	  }

	printf("testing blank string with a key of 1 and a go singal of "", should not encrypt\n");
	if(testCase2("", "1", "") < 0)
	  {
	    printf("test case 2.7 failed\n");
	  }

	printf("testing blank string with a key of 1 and a go singal of go, should do nothing\n");
	if(testCase2("", "1", "go") < 0)
	  {
	    printf("test case 2.8 failed\n");
	  }

	/* test case 3, weird stuff with the buffer */
	void *m1 = malloc(4100);
	memset(m1, 'a', 4096);
	printf("testing a full buffer of a's with a key of 1 and valid go\n");
	dest = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, rotXfd, 0);

	if(dest == MAP_FAILED)
	  {
	    fprintf(stderr, "mmap() error\n");
	    exit(1);
	  }
	else
	  {
	    printf("mmap() to /dev/rotX established\n");
	  }

	strcpy(dest, m1);

	if ( write ( rotXctlfd, "go", 10) > 0)
	  {
	    printf("write to /dev/rotXctl successful\n");
	  }
	strcpy(buf2,dest);
	printf("strcpy successful\n");
        munmap(dest, 4096);

	/*test case 4: thread safety, going to spawn 1000 threads all doing the same thing*/
	printf("spawning 1000 threads trying to encrypt hello with a key of 1\n");
	pthread_t tids[1000];
	for(int i = 0; i < 1000; i++)
	  {
	    pthread_create(tids + i, NULL, threadRunner, NULL);
	  }
	for(int i = 0; i < 1000; i++)
	  {
	    pthread_join(tids[i], NULL);
	  }
	printf("if no threading errors printed out then we are threadsafe\n");
	return 0;
}

