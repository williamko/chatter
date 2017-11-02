1. Compile server.cpp: g++ server.cpp -o server
2. Compile client. cpp: g++ client.cpp -o client
3. Run ./server in one window
4. Run ./client in separate window
5. Set up the server program first:
	- entering the filename. (i.e. “input1.txt”) 
	- enter a server port number. (i.e. “2468)
6. Then set up the client program: 
	- enter the server host name (must be “localhost”)
	- enter the server port number (must be same one used in server^)
7. Enter a username in the client window followed by a (space) and a ‘*’ which is the non printable character that specifies the length of each message.
	[For example]
	- Enter user name: jorge@uh.edu *
				       ^note the space

8. Enter ‘#’ to terminate the connection

