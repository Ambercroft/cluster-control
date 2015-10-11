
//Control program for nano relay usb
//
//Copyright Bob Wickson 2015/01/22
//
//controld

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "controld.h"

#include <string.h>
#include <libudev.h>
//#include <locale.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024

int main(int argc, char **argv){
  int n = 0;
  char buf[BUFSIZE];
  int cb = 0; // board file descriptor
  int cl = 0; // listening file descriptor
  int cp = 0; // control file descriptor
  int sosa = 0; // size of sa
  int noexit = 1;
  fd_set  ready;        /* used for select */
  int i = 0;        /* used in the multiplex loop */

  struct udev *udev;
  struct udev_enumerate *enumerate;
  struct udev_list_entry *devices, *dev_list_entry;
  struct udev_device *dev;


  // find the nano control board
  // use the argv if available or else we have to search for it.
  // make connection to control board
  printf("Arg passed = %d\n",argc);
  if ( argc > 1 ){
    //what is the proper arg required for the open stat. ??
    cb = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY );
    if(cb < 0){
      printf("Unable to open mini-board device\n");
      exit(1);
    }
  } else {
    // finding nano board
    // expect /dev/USBttyx or /dev/ttyACM0 
    udev = udev_new();
    if (!udev) {
      printf("Can't create udev\n");
      exit(1);
    }
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For each item enumerated, print out its information.
           udev_list_entry_foreach is a macro which expands to
           a loop. The loop will be executed for each member in
           devices, setting dev_list_entry to a list entry
           which contains the device's path in /sys. */
    udev_list_entry_foreach(dev_list_entry, devices) {
      const char *path;

      /* Get the filename of the /sys entry for the device
         and create a udev_device object (dev) representing it */
      path = udev_list_entry_get_name(dev_list_entry);
      dev = udev_device_new_from_syspath(udev, path);

      /* usb_device_get_devnode() returns the path to the device node itself in /dev. */
      printf("Device Node Path: %s\n", udev_device_get_devnode(dev));
      // copy %s to buf
      strncpy(buf,udev_device_get_devnode(dev),BUFSIZE);
      printf("Device Node Path(2): %s\n",buf);

      /* The device pointed to by dev contains information about
         the hidraw device. In order to get information about the
         USB device, get the parent device with the
         subsystem/devtype pair of "usb"/"usb_device". This will
         be several levels up the tree, but the function will find it.*/
      dev = udev_device_get_parent_with_subsystem_devtype(dev,"usb","usb_device");
      if (!dev) {
        printf("Unable to find parent usb device.\n");
        //exit(1);
        break;
      } else {

      /* From here, we can call get_sysattr_value() for each file
         in the device's /sys entry. The strings passed into these
         functions (idProduct, idVendor, serial, etc.) correspond
         directly to the files in the directory which represents
         the USB device. Note that USB strings are Unicode, UCS2
         encoded, but the strings returned from
         udev_device_get_sysattr_value() are UTF-8 encoded. */

        printf("Device Node Path: %s\n", udev_device_get_devnode(dev));

        printf("  VID/PID: %s %s\n",
          udev_device_get_sysattr_value(dev,"idVendor"),
          udev_device_get_sysattr_value(dev, "idProduct"));
        printf("  %s\n  %s\n",
          udev_device_get_sysattr_value(dev,"manufacturer"),
          udev_device_get_sysattr_value(dev,"product"));
        printf("  serial: %s\n",
          udev_device_get_sysattr_value(dev, "serial"));
        // need string compare here
        if(strcmp(udev_device_get_sysattr_value(dev,"manufacturer"),"FTDI")==0){
          printf("compare is equal...\n");
          cb = open(buf, O_RDWR | O_NOCTTY | O_NDELAY );
          if (cb < 0){
            printf("Unable to open mini-board device\n");
            exit(2);
          }
          udev_device_unref(dev);
          break; // free enumerate objects
        
        }
          udev_device_unref(dev);
      }
    }
      /* Free the enumerator object */
      udev_enumerate_unref(enumerate);
      udev_unref(udev);
  }
  // set port to no delay, return immediately if there is no data
  //  fcntl(cb, F_SETFL, FNDELAY);
  
  // send info to control
  n = 0;
  n = write(cb,":,\n",4); // Activate mini-board
  if ( n<0 ){
    printf("write() to device failed.\n");
  }else{
    printf("write() to a device occured. Was it the right device? \n");
  }
  
// now that we have a mini-board connected open a port for control input
  cl = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0 );
  if (cl < 0){
    printf("Unable to open control port\n");
    exit (3);
  }else{
    printf("..opened control port\n");
  }
  
// bind
 struct sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = INADDR_ANY;
  sa.sin_port = htons(7100);
  bind(cl,(struct sockaddr *) &sa,sizeof(sa));

// make the port non blocking
  fcntl(cl,F_SETFL,O_NONBLOCK | FNDELAY); 

// listen
  if (listen(cl,3) == 0) {
    printf("Listening...\n"); 
    // loop listening for the connection
    // if we have connetion -- verify 
    // then run connection loop
    // exit program if told to do so
    // exit  is a] command b] shutdown c] loss of power

// connect
    while(noexit){ // main loop : loop until exit command from control node or user
      // read and replies
      FD_ZERO(&ready);
      FD_SET(cb, &ready); // mini-board
      FD_SET(cp, &ready); // control-port
      if ( cp < 1 ){
        printf("Cp is %d. Cl is %d.\n",cp,cl);
        FD_SET(cl, &ready); // listen fd
      } else {
        printf("Cp is conntected.\n");
      }
//     printf("++cb=[%d] cl=[%d] cp=[%d] \n",cb,cl,cp);
      select(cl+2, &ready, NULL, NULL, NULL);
      // if there is something trying to make a connection
      if (FD_ISSET(cl, &ready)) {
        printf("cl was set\n");
// clear out pending data - no connectin allowed!!
        i = read(cl,buf,BUFSIZE);
        if (cp >0){
//          printf("Control port in use\n");
          write(cl,"Control port in use\n",20);
          FD_CLR(cl,&ready);
        } else {
          sosa = sizeof(sa);
          cp = accept(cl,(struct sockaddr *)&sa, &sosa );
          if( cp < 0 ){
            printf("Not successful connecting. cp=[%d]\n",cp);
          } else {
            printf("Should be good. cp=[%d]\n",cp);
            fcntl(cp,F_SETFL,O_NONBLOCK | FNDELAY);
          }
        }
      }  


// read the mini-board and send to appropriate connection 
      if (FD_ISSET(cb, &ready)) {
        // mini board has characters for us
        i = read(cb, buf, BUFSIZE);
        if (i > 0) {
          if ( cp > 0 ){
            write(cp, buf, i);
          }
          // do i need to act on the info received?
          // if the mini-board replies that it is waiting for :
          // then send ":" to restart

        }else{
         // printf("We were told stuff was here. what happened?\n");
        }
      } 

// read control port and send to mini-board
      if (FD_ISSET(cp, &ready)) {
        // control port has data for us
        i = read(cp, buf, BUFSIZE);
        if (i > 0) {
          write(cb, buf, i);
          // echo char back
          write(cp, buf, i);
          //  printf("Control read buffer is [%s]\n",buf);
          // do i need to act on the info received?
          if( buf[0] == '='){ // = close remote connection #61
            write(cp,"closing connection - initiated by you\n",38);
            close (cp);
            printf("closed connection cp=%d.\n",cp);
            cp = 0;
          }
          if (buf[0] == 'x'){ // quit on 'x'
            noexit = 0;
          }
          // if shut down code then noexit = 1
          FD_CLR(cp,&ready);
        }
      }


      if( noexit == 0 ){  // unending loop for now //command for exit is true)
        noexit = 0;
      }
    }  
  }else{
    printf("We were not listening!!!!\n");
    printf("Some error code could be avail, write the code fool! \n");
  }  
  // shutdown program

  if(cb >= 0){
    close(cb);
    printf("Closed cb\n");
  }
  if(cp >= 0){
    close(cp);
    printf("Closed cp\n");
  }
  if(cl >= 0){
    close(cl);
    printf("Closed cl\n");
  }

  return 0; // if I get here then everything should be OK.
}


