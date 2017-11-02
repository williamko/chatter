//
//  hash.h
//  COSC 2430 Project 4
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
#include <netdb.h>

using namespace std;

int main() {
    string hostname;
    int portNum;

    cout << "Enter a server host name (must be \"localhost\"): ";

    cin >> hostname;
    
    while (hostname != "localhost") {
        cout << "Enter a server host name: ";
        cin >> hostname;
        
    }

    cout << "Enter a server port number: ";

    cin >> portNum;
    
    
    int client;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    struct sockaddr_in server_addr;
    
    cout << endl;
    
    // Create socket connection

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        cout << "Failed to establish socket..." << endl;

        exit(1);
    }
    

    cout << "Socket client successfully created!" << endl;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    // Connect to socket
    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
        cout << ">> Connecting to server port number: " << portNum << endl;
    }
    
    cout << ">> Awaiting confirmation from the server..." << endl;
    
    recv(client, buffer, bufsize, 0);
    
    cout << ">> Connection confirmed!";
    
    cout << endl;
    
    cout << ">> Enter # to terminate the connection\n" << endl;
    
    // Client will request username first
    string un;
    string t;
    int flag = 0;
    
    do {
        
        cout << "Enter a user name: ";
        // 1. SEND user name request to server
        do {

            cin >> buffer;
            t = buffer;
            
            if (t == "Terminate") {
                cout << "\n>> Connection terminated." << endl;
                close(client);
                exit(1);
            }
            
            
            if (flag == 0) {
                un = buffer;
                flag = 1;
            }
            
            send(client, buffer, bufsize, 0);
            
            if (*buffer == '#') {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
            
            
        } while (*buffer != 42);
        
        flag = 0;

        
        // 4. RECEIVE public key from server
        do {
            recv(client, buffer, bufsize, 0);
            if (*buffer != '*') {
                cout << "The public key for user " << un << " is " << buffer << endl;
            }
            
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
            
        } while (*buffer != 42);
        
        cout << endl;
        
    } while (!isExit);
    
    // Close when "Terminated"
    cout << "\n>> Connection terminated." << endl;
    close(client);
    exit(1);
    
    return 0;
}
