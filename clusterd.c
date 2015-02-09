
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


#define BUFSIZE 1024

int main(int argc, char **argv){
  int n = 0;
  char buf[BUFSIZE];
  char bufo[BUFSIZE];
  int cb = 0;
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
    cb = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY );
    if(cb < 0){
      printf("Unable to open device\n");
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
      // copy %s to bufo
      strncpy(bufo,udev_device_get_devnode(dev),BUFSIZE);
      printf("Device Node Path(2): %s\n",bufo);

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
          cb = open(bufo, O_RDWR | O_NOCTTY | O_NDELAY );
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
  }
  while(noexit){ // main loop : loop until exit command from control node or user
    // read and replies
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
          printf("Read buffer is [%s]\n",buf);
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


