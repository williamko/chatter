//
//  server.cpp
//  COSC 3360 Project 2
//
//  Created by William Ko on 11/20/16.
//  Copyright © 2016 Will Ko. All rights reserved.
//
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>

using namespace std;

string* userIDs = new string[1025];
string* publicKeys = new string[1025];

int userIDsCounter = 0;
int publicKeysCounter = 0;


int userNameSearch(string user) {
    for (int i = 0; i < userIDsCounter; i++) {
        if (userIDs[i] == user) {
            return i;
        }
    }
    
    return -1;
}

int main() {

    // Prompt user for a filename
    string filename;
    
    cout << "Enter a file name: ";
    
    cin >> filename; // Include .txt
    
    // Store the contents of the file into a table
    cout << "*** READING ***" << endl;
    
    // Prompt user for the Matrix input file name
    ifstream myFile;
    myFile.open(filename.c_str());
    
    string line;
    string uid;
    string pk;

    if (myFile.is_open()) {
        myFile >> uid;
        myFile >> pk;
        userIDs[userIDsCounter] = uid;
        publicKeys[publicKeysCounter] = pk;
        userIDsCounter++;
        publicKeysCounter++;
        
        while (getline(myFile,line)) {
            myFile >> uid;
            myFile >> pk;
            userIDs[userIDsCounter] = uid;
            publicKeys[publicKeysCounter] = pk;
            userIDsCounter++;
            publicKeysCounter++;
        }
        
        // FIX: last line is saved twice
        
        myFile.close();
    }
    
    // Add Terminate
    userIDs[userIDsCounter] = "Terminate";
    publicKeys[publicKeysCounter] = "Terminate";
    userIDsCounter++;
    publicKeysCounter++;
    
    
    int portNum;
    
    cout << "Enter a server port number: ";
    
    cin >> portNum;
    
    int client, server;
    bool isExit = false;
    
    int bufsize = 1024;
    char buffer[bufsize];
    
    struct sockaddr_in server_addr;
    socklen_t size;

    // Establish a stream socket connection
    client = socket(AF_INET, SOCK_STREAM, 0);
    cout << endl;
    
    if (client < 0) {
        cout << "Failed to establish socket..." << endl;
        exit(1);
    }
    
    cout << "Socket server successfully created!" << endl;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);
    
    // Bind the socket to the specified port number
    
    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) {
        cout << ">> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }
    
    size = sizeof(server_addr);
    cout << ">> Searching for clients..." << endl;
    
    // Perform a listen() to specify a maximum number of queued connection requests
    listen(client, 1);

    // Do an accept() that will let it wait for connection requests
    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);
    
    // first check if it is valid or not
    if (server < 0) {
        cout << ">> Error on accepting..." << endl;
    }
    
    while (server > 0) {
        strcpy(buffer, ">> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << ">> Sucessfully connected to client " << clientCount << endl;
        cout << "\n>> Enter # to terminate the connection\n" << endl;
        
        
        string un;
        string pubkey;
        int flag = 0;
        
        // Client says:
        // RECEIVE message from client
        do {
            recv(server, buffer, bufsize, 0);
            
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
            
            if (flag == 0) {
                un = buffer;
                flag = 1;
            }
            
            if (*buffer != '*') {
                
                pubkey = publicKeys[userNameSearch(buffer)];
                
                if (pubkey == "Terminate") {
                    isExit = true;
                    cout << "Connection terminated." << endl;

                }
                
                cout << "User name requested from client is: " << buffer << endl;
                send(server, pubkey.data(), bufsize, 0);

            } else {
        
                send(server, &("*"), bufsize, 0);
            }
            
            flag = 0;
            // Specify the length of each message using '*' (non printable character)
        } while (!isExit);


        // Close when "Terminated"
        cout << "\n\n>> Connection terminated." << endl;
        close(server);
        isExit = false;
        exit(1);
    }
    
    close(client);

    return 0;
}
