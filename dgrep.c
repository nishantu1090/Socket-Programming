#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include<fcntl.h>
int main(int argc, char *argv[]){//E.g. 3 Client
	char message[255];
	int sid, portNumber, pid, n;
	socklen_t len;
	char * IP;
	struct sockaddr_in servAdd;
	/*if(argc != 3){
		printf("Call model: %s <IP> <Port #>\n", argv[0]);
		exit(0);
	}*/
	if((sid=socket(AF_INET, SOCK_STREAM, 0))<0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	servAdd.sin_family = AF_INET;
	printf("Enter server IP:");
	scanf("%s", IP); 
	//IP="127.0.0.1";
	
	printf("Enter server port number:");
	scanf("%d", &portNumber);
	//portNumber=1804;
	servAdd.sin_port = htons((uint16_t)portNumber);

//The inet_pton() function converts an Internet address in its standard text format into its numeric binary form
	if(inet_pton(AF_INET,IP ,&servAdd.sin_addr)<0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	
	    
		
	
	if( access(argv[2],F_OK) != 0 ) {
		fprintf(stderr, "%s file does not exist! Exiting!", argv[2]);
		exit(0);
	}	
	if(access(argv[3], F_OK) != 0){
		fprintf(stderr, "%s file does not exist! Exiting!", argv[3]);
		exit(0);
	}
	int f2 = open(argv[3], O_RDONLY);
	char txt[500];
	n = read(f2,txt,500);	
	//txt[n]='\0';
	//printf("%sfile 2 contents are:",txt); 
	
	//connecting to server
	if(connect(sid, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}
	
	char * pattern = argv[1];
	pattern[strlen(pattern)]='\0';
	//printf("The length of argv[1] is %d", strlen(pattern));	
	pattern[strlen(pattern)] = '\0';

	//sending pattern to server
	write(sid,pattern,strlen(pattern));
	
	
	char p[4];	
	txt[strlen(txt)] = '\0';
	read(sid, p, 4);
	
	

	//sending file to server
 	int l = write(sid, txt, strlen(txt));	
	
	//Running grep one file 1
	char command[50] = " grep -w ";
	strcat(command, argv[1]);
	strcat(command, " ");
	strcat(command, argv[2]);
	strcat(command, "> ClientResult.txt");
	system(command);	
	
	//Reading server result
	char serverResult[500];
 	//read(sid, p, 4);	
	int z = read(sid, serverResult, 500);
	serverResult[z] = '\0';
	int server = open("ServerResult.txt", O_CREAT|O_RDWR, 0777);
	write(server, serverResult,strlen(serverResult));
	close(server);
	close(sid);
	char fileNameClient[256];
	strcpy(fileNameClient, argv[2]);
	char fileNameServer[256];
	strcpy(fileNameServer, argv[3]);
	printResults(fileNameClient, "ClientResult.txt", pattern);
	printResults(fileNameServer, "ServerResult.txt", pattern);
 	remove("ServerResult.txt");
	remove("ClientResult.txt");	
}

void printResults(char * name, char * tempFile, char * pattern){
	char const* const fileName = tempFile;
	char line[256];
	char result[1000];
	FILE* file = fopen(fileName, "r");	
	char  nameCopy[50];
	strcpy(nameCopy, name);
	strcat(nameCopy, " : ");
	strcpy(name, nameCopy);	
	while (fgets(line, sizeof(line), file)) {
    		strcat(name, line);
		char * token = strtok(name, " ");
   		reset();
   		while( token != NULL ) {
		      if(strcmp(token, pattern) == 0)
 			red();			      
		      printf( "%s ", token ); //printing each token
		      token = strtok(NULL, " ");
		      reset();
   		}
   
		
		strcpy(name, nameCopy);
    	}
	fclose(file);
	return;
}

void red () {
  printf("\033[1;31m");
}
void reset () {
  printf("\033[0m");
}



