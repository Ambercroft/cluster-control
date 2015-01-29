//Control program for nano relay usb
//
//Copyright Bob Wickson 2015/01/22
//
//controld

#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "controld.h"


#define BUFSIZE 1024

int main(int argc, char **argv){
  int n = 0;
  char buf[BUFSIZE];
  int cb = 0;
  // find the nano control board

  // make connection to control board
  cb = open(argv[1], O_RDWR);
  if(cb < 0){
    printf("Unable to open device\n");
    exit(1);
  }
  // set port to no delay, return immediately if there is no data
  fcntl(cb, F_SETFL, FNDELAY);
  
  //{
  // accept control codes

  while(1){
    // send info to control
    n = 0;
    n = write(cb,": a\n",64);
    if ( n<0 ){
      //printf("write() to device failed.\n");
    }

    // read and replies
    n = 0;
    n = read(cb,&buf,1023);
    if (n>0){
      printf("BUF{%s}\n",buf);
    }else{
      //printf("nothing read\n");
    }

    // confirm action
    //
    //#include "nanocom.h"




/* main program loop */
  int pf; 
  fd_set  ready;        /* used for select */
  int i = 0;        /* used in the multiplex loop */
  int done = 0;
  char buf[BUFSIZE];
  struct timeval tv;
  tv.tv_usec = 0;

    do { /* loop forever */
        FD_ZERO(&ready);
        FD_SET(STDIN_FILENO, &ready);
        FD_SET(pf, &ready);

        select(pf+1, &ready, NULL, NULL, NULL);

        if (FD_ISSET(pf, &ready)) {
            /* pf has characters for us */
            i = read(pf, buf, BUFSIZE);

            if (i > 0) {
                write(STDOUT_FILENO, buf, i);
            }
           else {
                done = 1;
            }
        } /* if */

        if (FD_ISSET(STDIN_FILENO, &ready)) {
                /* standard input has characters for us */
                i = read(STDIN_FILENO, buf, BUFSIZE);
            if (i > 0) {
                cook_buf(pf, buf, i);
            }
            else {
                done = 1;
            }
        } /* if */
    }  while (!done); /* do */
}




                                                               73,1   Command
            else {  
  }  
  // shutdown program

  close(cb);

  return 0; // if I get here then everything should be OK.
}


