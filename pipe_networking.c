#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  //HANDSHAKE
  mkfifo("WellKnownPipe", 0666);
  int fd = open("WellKnownPipe", O_RDONLY);
  char raf[256];
  read(fd, raf, 256);
  //raf[5] = 0;

  //printf("The message I recieved from the client is: %s\nI am proceeding to open a pipe of said name\n", raf);
  int fd2 = open(raf, O_WRONLY);
  //printf("fd2: %d, '%s'\n\n", fd2, raf);

  //printf("Sending message to client: Hello There... Listening\n");
  write(fd2, "Hello There", 12);

  //fd = open("WellKnownPipe", O_RDONLY);
  //sleep(1);
  read(fd, raf, 256);
  //printf("The message I recieved back from the client is: %s\n", raf);
  if(!strcmp(raf, "Handshake Complete")){
    //Do stuff
    while(1){
      read(fd, raf, 256);
      printf("Got: '%s'\n", raf);
      strcat(raf, " - client 2k18");
      printf("Sending back: '%s'\n", raf);
      write(fd2, raf, strlen(raf) + 1);
    }
    
  }else{
    printf("Oops, server is unable to confirm, something went wrong...");
  }
  
  unlink("WellKnownPipe");
  * to_client = fd2;
  return fd;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  //HANDSHAKE
  //printf("Creating the private pipe\n");
  mkfifo("Fife", 0666);
  
  //printf("The message I'm sending to the server is: Faifu\n");
  int fd = open("WellKnownPipe", O_WRONLY);
  write(fd, "Fife", 6);

  //printf("Listening...\n");
  int fd2 = open("Fife", O_RDONLY);
  char raf[256];
  read(fd2, raf, 256);

  //printf("The message I received back from the server is: %s\nSending back: Handshake Complete\n", raf);
  if(!strcmp(raf, "Hello There")){
    write(fd, "Handshake Complete", 18);
    unlink("Fife");
    unlink("WellKnownPipe");
  }else{
    printf("Oops something went wrong, the server could not establish a connection");
  }
  
  //Do stuff
  while(1){
    printf("What would you like to say? ");
    fgets(raf, 256, stdin);
    raf[strlen(raf) - 1] = 0;
    write(fd, raf, 256);
    read(fd2, raf, 256);
    printf("\n%s\n", raf);
  }
  
  
  * to_server = fd2;
  return fd;
}
