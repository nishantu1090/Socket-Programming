#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <fcntl.h>
void child(int);
int main(int argc, char *argv[]){// E.g. 3: Server
	int sd, client, portNumber, status;
	socklen_t len;
	struct sockaddr_in servAdd;
	if(argc != 2){
		printf("Call model: %s <Port #>\n", argv[0]);
		exit(0);
	}
	if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("Cannot create socket\n");
		exit(1);
	}
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
	listen(sd, 5);

	while(1){
		client = accept(sd,(struct sockaddr *)NULL,NULL);
		printf("Got a client\n");
		if(!fork())//Child process 
		child(client);
		close(client);
		waitpid(0, &status, WNOHANG); // waitpid? 
	}
}//End main 


void child(int sd){
	char message[500];
	int n, pid;
	char pattern[100];
//	write(sd, "start chatting", 16);

	 /* reading client messages */
		int i = 1;
	while(1)
	{	
		
		if(n=read(sd,message,500)){
			printf("the value of n is:%d", n);
			message[n]='\0';
			if(i <= 1){
			strcpy(pattern, message);
		//	pattern = message;
				i=i+1;
	//			fprintf(stderr, "Pattern to find%s\n", pattern);
				char block[4]="hey\0";
				
				write(sd, block, strlen(block));	
			 	//write(sd, "hey\0",3);	
			}			
		
			else{
	//			fprintf(stderr, "message:%s", message);	
				//message[n]='\0';
				int file = open("temp.txt", O_CREAT|O_RDWR, 0777);
				int w = write(file, message, strlen(message));
				fprintf(stderr, "writ wrote %d bytes", w);
				break;
				
				
			}
		}
		
							
	}

	//forming command for grep
	char command[100] = "grep -w ";
	 strcat(command, pattern);
	 strcat(command, " temp.txt");
	 strcat(command, " > t2");
	char *p;
	
	//executing grep command and saving to a temp file
	system(command);	

	//reading grep result from file
	int file;
	file = open("t2", O_RDWR, 0777);
       	char grepResult[1000];
	int d = read(file, grepResult, 1000);
	fprintf(stderr, "The read read %d bytes", d);
	grepResult[d]='\0';
	fprintf(stderr, "The grep resutls are: %s\n", grepResult);
	close(file);
	
	//sending grep result to client
	write(sd, grepResult, strlen(grepResult));
	remove("t2");
	remove("temp.txt");
	fprintf(stderr, "Closing sd of client");
	close(sd);
	return;
	

		
}//End child

