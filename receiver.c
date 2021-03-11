//Michael Heller
//receiver.c

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

int main(int argc, char *argv[]) {
  sem_t *writing;
  sem_t *access;

  writing = sem_open("mphWriting", O_RDWR, S_IRUSR | S_IWUSR, 0); //signals new content to the file
  access = sem_open("mphAccess", O_RDWR, S_IRUSR | S_IWUSR, 0); //provides exclusive access to file
  char *last_message;

  while(1) {
    last_message = (char *) malloc(100);
    int i = 2;
    //lock semaphore - wait for new content
    sem_wait(writing);
    //lock semaphore - wait on exclusive access to file
    sem_wait(access);
    while (last_message[0] != '\"') {
      i++;
      FILE *fp = fopen("channel.txt", "r");
      fseek(fp, (-1*i), SEEK_END);
      fread(last_message, 1, i, fp);
      fclose(fp);
    }
    //print last line
    printf("receiver [msg arrival]: %s", last_message);
    last_message = " ";
    //unlock semaphore
    sem_post(access);
  }

  free(last_message);

  sem_close(writing);
  sem_close(access);

  sem_unlink("mphWriting");
  sem_unlink("mphAccess");


return 0;
}
