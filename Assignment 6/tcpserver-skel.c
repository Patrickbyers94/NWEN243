#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <ctype.h>
  
#define bufsize 1024

void eatZombies(int n){
  // This function removes the zombie process state left
  // after the child exits. Learn about this in NWEN 301!

  wait3(NULL,WNOHANG,NULL); // Nom Nom 
}

int main(int argc, char *argv[]){
  int sock, length, msgsock, status;
  int count;
  int max = 0;
  struct sockaddr_in server;
  char buf[bufsize];
  printf("Starting \n");
	
  // for forking, and cleaning up zombie child state afterwards
  // You must not change this code. 

  pid_t id;       
  signal(SIGCHLD, &eatZombies);
 
  // OK, NWEN 243 code starts here. 

  // Create a socket (see Lab 2) - it is exactly the same for a server!
   sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0){
  printf(stderr, "Failed to create the socket \n");
  return(-1);}
	printf("socket created \n");
  // Next, create the socket addressing structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1])); // this time 1st arg is port#

  // Next you need to BIND your socket.  
  if(bind(sock,(struct sockaddr*) &server,(socklen_t) sizeof(server))<0){
  printf( "failed to bind connection %s to %s which is of length %s \n",sock);
  close(sock);
  return(-1);}
  printf("LISTENING \n");

  
  // Right, now we are in the main server loop.
  // YOUR CODE HERE   

  // Start by listening
	if(listen(sock,5)<0){
	printf( "Failed to start post listening \n");
	close(sock);
	return(-1);
	}
  printf("SITTING NOT DOING MUCH. \n");

  while(1){
    msgsock = accept(sock,(struct sockaddr*) &server,(socklen_t *) &length);
    if(msgsock<0){
		printf(stderr, "Failed to accept connection on %s . \n", (struct sockaddr*) &server);
		close(msgsock);
		count--;
		return(-1);    
    }
    else{
    	//printf("accpeted connection \n");
    count++;
    }
    
	if(count>max){
	max = count;
	printf("maxclients = %d \n",max);}

    // the next call makes a new child process that will actually handle the client.
    id = fork();
   //printf("id is %d \n",id);
    if(id == 0){
    if(read(msgsock,buf,bufsize)<0){
    	printf(stderr, "Failed to read the data \n");
    	close(msgsock);
    	count--;
    	return(-1);
    }
	for (char *p = buf ; *p ; *p = toupper(*p), p++);
	if(write(msgsock,buf,bufsize)<0){
	printf(stderr, "Failed to write \n");
	close(msgsock);
	count--;
	return(-1);
}
    }
	 else if(id>0){
	 continue;
	 }
	 else {
	 	printf(stderr,"We have encountered an error in processing \n");
	 	close(sock);
	 	return (-1);	 
	 }
    // when id == 0, this is the child and needs to do the work for the server. 
    // when if > 0, this is the parent, and it should just loop around,
    // when id < 0, we had an error.

    // Your code here for the child process, that will read from the connection socket, process the data 
    // write back to the socket, and then close and finally call exit(0) when done.

    // Note:  make sure the parent process (id > 0) does not execute this code, but immediately loops back
    // around (via the while) to get another connection request.

  }
close(sock);
exit(0);
}

