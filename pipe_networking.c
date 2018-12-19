#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo("WellKnownPipe", 0666);
  int fd = open("WellKnownPipe", O_RDONLY);
  char raf[256];
  read(fd, raf, 256);
  raf[5] = 0;
  
  
  printf("The message I recieved from the client is: %s\nI am proceeding to open a pipe of said name\n", raf);
  int fd2 = open(raf, O_WRONLY);
  //printf("fd2: %d, '%s'\n\n", fd2, raf);
  
  
  printf("Sending message to client: Hello There... Listening\n");
  write(fd2, "Hello There", 11);
  
  //fd = open("WellKnownPipe", O_RDONLY);
  //sleep(1);
  read(fd, raf, 256);
  printf("The message I recieved back from the client is: %s\n", raf);
  
  * to_client = fd;
  return 0;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  
  printf("Creating the private pipe\n"); 
  mkfifo("Faifu", 0666);
  
  
  printf("The message I'm sending to the server is: Faifu\n");
  int fd = open("WellKnownPipe", O_WRONLY);
  write(fd, "Faifu", 5);
  
  
  printf("Listening...\n");
  int fd2 = open("Faifu", O_RDONLY);
  char raf[256];
  read(fd2, raf, 256);
  
  printf("The message I received back from the server is: %s\nSending back: Handshake Complete\n", raf);
  
  write(fd, "Handshake Complete", 17);
  
  * to_server = fd2;
  return fd;
}
