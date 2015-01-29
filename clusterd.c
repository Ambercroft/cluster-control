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
  int noexit = 1;
  fd_set  ready;        /* used for select */
  int i = 0;        /* used in the multiplex loop */

  // find the nano control board
  // use the argv if available or else we have to search for it.
  // make connection to control board
  cb = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY );
  if(cb < 0){
    printf("Unable to open device\n");
    exit(1);
  }
  // set port to no delay, return immediately if there is no data
  fcntl(cb, F_SETFL, FNDELAY);
  
  // send info to control
  n = 0;
  n = write(cb,":",4); // Activate mini-board
  if ( n<0 ){
    printf("write() to device failed.\n");
  }
  while(noexit){ // main loop : loop until exit command from control node or user
    // read and replies
//    n = 0;
//    n = read(cb,&buf,1023);
//    if (n>0){
//      printf("BUF{%s}\n",buf);
//    }else{
//      //printf("nothing read\n");
//    }
      FD_ZERO(&ready);
      FD_SET(STDIN_FILENO, &ready);
      FD_SET(cb, &ready);

      select(cb+1, &ready, NULL, NULL, NULL);

      if (FD_ISSET(cb, &ready)) {
        /* pf has characters for us */
        i = read(cb, buf, BUFSIZE);
        if (i > 0) {
          write(STDOUT_FILENO, buf, i);
        }
      } 

      if (FD_ISSET(STDIN_FILENO, &ready)) {
        /* standard input has characters for us */
        i = read(STDIN_FILENO, buf, BUFSIZE);
        if (i > 0) {
          write(cb, buf, i);
        }
      } 
      if( noexit == 0 ){  // unending loop for now //command for exit is true){
      noexit = 0;
    }  
  }  
  // shutdown program

  close(cb);

  return 0; // if I get here then everything should be OK.
}


