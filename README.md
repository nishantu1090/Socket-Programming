# Socket-Programming

This application can be used to do distribute pattern search in two files. The client program can search a particular pattern in File-1 and server program can search the same pattern in File-2. <br />
<br />
The client program establishes a connection with the server, sends the pattern and File-2 to it. Server does a grep on the received File and sends back the search results to client. Meanwhile, client does a grep on File-1 and combine the search results from the server with its own search results. Finally client displays the combined results to the user. <br />
<br />
Server does the search of pattern in File in a separate process, so that it does not block. When a connection is established with any client, it creates a new process to handle that connection and goes back, waiting for more connections. <br />

## To run the program
Compile the server code with the below command:<br />
 ```gcc server.c -0 server```<br/>
 Run the server with the below command: <br />
 ```./server <port-number>```<br />
 
 Similarly compile the client program(dgrep.c) with the below command:<br />
 ```gcc dgrep.c -o client```<br />
 
 Run the client program with the below command: <br />
 ```./client <pattern> <File-1 name> <File-2 name>```<br/>
 
 You will get an output that combines the pattern search results on the two files.


