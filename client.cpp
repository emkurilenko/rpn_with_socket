#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define SERVPORT 8888
#define SERVER "The_server_name_or_IP"

using namespace std;

const char C_TEXT[]= "\033[1;32;40m";
const char C_ERROR[]= "\033[1;31;40m";
const char C_CLEAR[]= "\033[1;0m";
const char green[] = "\033[0;40;32m";

void del_space(string &str) {
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ') {
			str[i] = '_';
		}
	}
}

int main(int argc, char *argv[])
{
    char buf[1024];
    char server[255];
    string mess;
    struct hostent *hostp;
  
    int sock;

    struct sockaddr_in addr;

   ;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    else{
        cout <<green<< "->Client-socket() OK"<<C_CLEAR<<endl;
    }

    if(argc > 1){
        strcpy(server,argv[1]);
        cout <<green<< "->Connecting to the "<<server<<" , port "<<SERVPORT<<" ..."<<C_CLEAR<<endl;
    }
    else
    strcpy(server, SERVER);
    memset(&addr, 0x00, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVPORT); 
    //addr.sin_addr.s_addr=inet_addr("192.168.1.3");
    if((addr.sin_addr.s_addr = inet_addr(server))== (unsigned long)INADDR_NONE){
        hostp = gethostbyname(server);
        if(hostp == (struct hostent *)NULL){
            cout <<C_ERROR<< "->Host not found -->"<<C_CLEAR<<endl;
            cout<<endl;
            cout <<C_ERROR<< "\t\t---This is a client program---"<<C_CLEAR<<endl;
            cout <<C_ERROR<< "\tCommand usage: "<<argv[0]<<" <server name or IP>\n"<<C_CLEAR<<endl;
        //    printf("\t\t---This is a client program---\n");
         //   printf("\tCommand usage: %s <server name or IP>\n", argv[0]);
            cout<<endl;
            close(sock);
            exit(-1);   
        }
        memcpy(&addr.sin_addr, hostp->h_addr, sizeof(addr.sin_addr));
    }


    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        cout <<C_ERROR<< "->Client-connect() error"<<C_CLEAR<<endl;
        close(sock);
        exit(2);
    }
    else{
        cout <<green<< "->Client-connect() error"<<C_CLEAR<<endl;
        cout << "Enter the exit to disconnect from the server"<<endl;
        cout << endl;
    }
    
    while(1){
        const char* message;
        int send_int = 0;
        cout << "Enter: ";
        getline(cin,mess);
        del_space(mess);
        message = mess.c_str();
        if(strcmp(message,"exit")==0){
            break;
        }
        send(sock, message, 1024, 0);
         fflush(stdout);
         mess.clear();
        recv(sock, buf, 1024, 0);
        if(!strcmp(buf,"Input Error")){
            cout<<C_ERROR<<buf<<C_CLEAR<<endl;
        }
        else{
             cout<<"Result: "<<green<<buf<<C_CLEAR<<endl;
        }
        cout<<endl;
    }
    close(sock);

    return 0;
}
