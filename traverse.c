/*
 * CSCI 315 Operating Systems Design
 * Original developed by CSCI 315 staff
 * Modified by: L. Felipe Perrone
 * Date: 2011-4-21
 * Copyright (c) 2011 Bucknell University
 *
 * Permission is hereby granted, free of charge, to any individual or
 * institution obtaining a copy of this software and associated
 * documentation files (the "Software"), to use, copy, modify, and
 * distribute without restriction, provided that this copyright and
 * permission notice is maintained, intact, in all copies and supporting
 * documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL BUCKNELL UNIVERSITY BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "read_dir.h"
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

// function prototypes
void process(char *name);
void enqueue(char *name, que_t *q);
void dequeue(que_t *q);
void peek_front(char *name,que_t q);
bool queue_empty(que_t q);
void initq(que_t *q);

// main function
int main(int argc,char *argv[]) {

  if (2 != argc) {
    printf(" usage: %s dirpath\n", argv[0]);
    exit(-1);
  }
  else {
    // pass in the starting directory
    //prints directory name, each filename and the total number of files
    process(argv[1]);
  }
  return 0;
}

// traverse a directory tree applying a function when a file is found
void process(char *root)
{
  int numOfDir = 0;
  int numOfFiles = 0;
  int numOfRegFiles = 0;
  unsigned int curr_size = 0;
  unsigned int max_size = 0;
  unsigned int min_size = 0;
  unsigned int total_size = 0;
  unsigned int avg_size = 0;
  time_t most_rec_t;
  time_t least_rec_t;
  que_t nameq;
  char dname[MAXLENGTH];
  char cname[MAXLENGTH];
  char prefix[MAXLENGTH];
  char mrec_name[35];
  char lrec_name[35];

  struct dirent *dp;
  DIR *dirp;

    initq(&nameq);
    enqueue(root,&nameq);

    while (true != queue_empty(nameq)) {
      peek_front(dname,nameq);
      dequeue(&nameq);

      dirp = opendir(dname);
      if (dirp != NULL) { // it is a directory
    numOfDir++;
	printf("directory : %s\n",dname);
	strncpy(prefix, dname, MAXLENGTH);
	strncat(prefix,"/", MAXLENGTH);

	for (dp = readdir(dirp); NULL != dp; dp = readdir(dirp)) {
	  if ((strcmp(dp->d_name,"..") != 0) &&
	      (strcmp(dp->d_name,".") != 0)) {

	    // prevent from infinite loop
	    strncpy(cname, prefix, MAXLENGTH);

	    // concatenate the prefix
	    strncat(cname, dp->d_name, MAXLENGTH);
	    enqueue(cname,&nameq);
	  }
	}
	closedir (dirp);
      } else {
	// test if it is a regular file and not a device or link -- TO-DO
	// if this is a regular file, then process it -- TO-DO
    numOfFiles++;
    unsigned int fd;
    fd = open(dname, O_RDONLY);
    struct stat file_info;
    if (lstat(dname, &file_info) == 0) {
        if (S_ISREG(file_info.st_mode))
            numOfRegFiles++;
        curr_size = file_info.st_size;
        if (numOfFiles == 1) {
            max_size = curr_size;
            min_size = curr_size;

            strcpy(mrec_name, dname);
            //printf("most recently modified: %s\n", mrec_name);
            strcpy(lrec_name, dname);
            //printf("least recently modified: %s\n", lrec_name);
            most_rec_t = file_info.st_mtime;
            least_rec_t = file_info.st_mtime;
        }
        else {
            if (file_info.st_mtime > most_rec_t) {
                most_rec_t = file_info.st_mtime;
                strcpy(mrec_name, dname);
                //printf("most recently modified: %s\n", mrec_name);
            }
            if (file_info.st_mtime < least_rec_t) {
                least_rec_t = file_info.st_mtime;
                strcpy(lrec_name, dname);
                //printf("least recently modified: %s\n", lrec_name);
            }
            if (curr_size > max_size)
                max_size = curr_size;
            if (curr_size < min_size)
                min_size = curr_size;
        }
        total_size += curr_size;
    }

	printf(" processing file: %s\n", dname);

    fflush(stdout);
    close(fd);
      }
    } // while

    avg_size = total_size/numOfFiles;

    printf(" smallest file size: %d\n", min_size);
    printf(" largest file size: %d\n", max_size);
    printf(" average file size: %d\n", avg_size);

    printf(" a total of %d directories were counted\n", numOfDir);

    printf(" a total of %d REGULAR files were counted\n", numOfRegFiles);

    printf(" most recently modified file: %s\n", mrec_name);
    printf(" least recently modified file: %s\n", lrec_name);

    printf(" a total of %d files were counted\n", numOfFiles);
}

// initialize queue data structure
void initq(que_t *q) {
  q->head = q->tail = NULL;
}

// test whether queue data structure is empty
bool queue_empty(que_t q) {
  if (NULL == q.head) {
     return true;
  } else {
    return false;
  }
}

// add an element to queue
void enqueue(char *name, que_t *q) {
  item_t *temp;

  temp = (item_t *)malloc(sizeof(item_t));
  strncpy(temp->name,name,MAXLENGTH);
  temp->next = NULL;

  if (true == queue_empty(*q)) {
    q->head = temp;
    q->tail = temp;
  } else {
    q->tail->next = temp;
    q->tail = q->tail->next;
  }
}

// remove an element from the front of the queue
void dequeue(que_t *q) {
  item_t *temp;

  if (true == queue_empty(*q)) {
    printf(" error in dequeue \n");
    exit(-1);
  } else {
    temp = q->head;
    q->head = q->head->next;
    free(temp);
  }
}

// find element at front of queue without removing
void peek_front(char *name, que_t q) {
  if (true == queue_empty(q)) {
    printf(" error in dequeue \n");
     exit(-1);
  } else {
    strncpy(name, q.head->name, MAXLENGTH);
  }
}
