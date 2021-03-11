//Michael Heller
//sender.c

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

//TODO protect access/appending to file with named semaphore
int main(int argc, char *argv[]) {
  //initialize shared semaphores
  sem_t *writing;
  sem_t *access;

  char input_string[200];
  FILE *fp;
  printf(">sender ");
  fgets(input_string, 200, stdin);

  if (input_string[0] != '\"') {
    printf("error, please enter string within double quotes.\n");
    exit(-1);
  }
  

  writing = sem_open("mphWriting", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 0); //signals new content to the file
  access = sem_open("mphAccess", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1); //provides exclusive access to file
  //lock semaphore for file access
  sem_wait(access);
  //open file and input the message string
  fp = fopen("channel.txt", "a");
  fputs(input_string, fp);
  fclose(fp);
  //unlock semaphore that provides file access
  sem_post(access);
  //unlock semaphore that concludes writing
  sem_post(writing);

  sem_close(writing);
  sem_close(access);

return 0;
}
