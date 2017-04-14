#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>   
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <functional> 

int count = 0;
using namespace std;

void *connection_handler(void *socked_desc);

int main(int argc, char *argv[]){
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc == -1){
        cout << "Could not create socket"<<endl;
        return 1;
    }

    cout << "Socket created" << endl;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if(bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0){
        perror("bind failed. Error");
        return 1;
    } 
	else{
    	cout << "BIND done" << endl;
	}
    listen(socket_desc,3);

    cout << "Waiting for incoming connection..." << endl;
    cout << endl;
    c = sizeof(struct sockaddr_in);
    pthread_t pthread_id;
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client,(socklen_t*)&c))){
        cout << "Connection accepted" << endl;
		count++;
		cout << "Client: ["<< count<<"]"<<endl;
        if(pthread_create(&pthread_id,NULL,connection_handler,(void*)&client_sock)<0){
            perror("Could not create thread");
            return 1;
        }
        cout << endl;
    }

    if(client_sock < 0){
        perror("accept failed");
    }
	
}

bool check_num(char ch) {
	if (ch >= '0' && ch <= '9') {
		return true;
	}
	else
		return false;
}

bool delim(char c) {
	return c == ' ';
}

bool is_op(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

int priority(char op) {
	if (op < 0)
		return 4; // op == -'+' || op == -'-'
	return
		op == '+' || op == '-' ? 1 :
		op == '*' || op == '/' || op == '%' ? 2 :
		-1;
}

void process_op(vector<double>& st, char op) {
	if (op < 0) {
		double l = st.back();  st.pop_back();
		switch (-op) {
			case '+':  st.push_back(l);  break;
			case '-':  st.push_back(-l);  break;
		}
	}
	else {
		double r = st.back();  st.pop_back();
		double l = st.back();  st.pop_back();
		switch (op) {
			case '+':  st.push_back(l + r);  break;
			case '-':  st.push_back(l - r);  break;
			case '*':  st.push_back(l * r);  break;
			case '/':  st.push_back(l / r);  break;
		}
	}
}

double ReversePolNotation(string & s) {
	bool may_unary = true;
	vector<double> st;
	vector<char> op;
	string result;
		for (size_t i = 0; i<s.length(); ++i)
	
		if (!delim(s[i]))
			if (s[i] == '(') {
				op.push_back('(');
				may_unary = true;
			}
			else if (s[i] == ')') {
				while (op.back() != '(')
					process_op(st, op.back()), op.pop_back();
				op.pop_back();
				may_unary = false;
			}
			else if (is_op(s[i])) {
				char curop = s[i];
				if (may_unary)  curop = -curop;
				while (!op.empty() && (
					curop >= 0 && priority(op.back()) >= priority(curop)
					|| curop < 0 && priority(op.back()) > priority(curop))
					)
					process_op(st, op.back()), op.pop_back();
				op.push_back(curop);
				may_unary = true;
			}
			else {
				string operand;
				while (i < s.length() && isalnum(s[i]))
					operand += s[i++];
					--i;
				if (isdigit(operand[0]))
					st.push_back(atoi(operand.c_str()));
				may_unary = false;
			}
			while (!op.empty())
				process_op(st, op.back()), op.pop_back();
		return st.back();
}

bool check_enter(string &str) {
	str = '(' + str + ')';
	int counter_left = 0;
	int counter_right = 0;
	int counter_operands = 0;
	int counter_operator =0;
	bool check = true;
	for (int i = 0; i < str.length(); i++)
	{
		if (check_num(str[i])) {
			counter_operands++;
			continue;
		}
			if (str[i] == '(') {
				counter_left++;
				if (str[i + 1] == ')' || str[i + 1] == '*' || str[i + 1] == '/') {
					check = false;
					break;
				}
			}
			else
				if (str[i] == ')'){
					counter_right++;
					if(check_num(str[i+1])){
						check = false;
						break;
					}
				}
			else
				if (is_op(str[i])) {
					counter_operator++;
					if (check_num(str[i - 1]) && check_num(str[i + 1]) || str[i - 1] == '(' || str[i + 1] == '(' || str[i - 1] == ')')
						continue;
					else {
						check = false;
						break;
					}
				}
		else
				check = false;
	}

	if (counter_left != counter_right) {
		check = false;
	}
	if (counter_operands < 1) {
		check = false;
	}
	if(counter_operator == 0){
		check = false;
	}
	return check;
}

void *connection_handler(void *socked_desc){

    int sock = *(int*)socked_desc;
    int read_size;
    char bufread[1024];
    string bufferstring;
	char res[1024];
	double result;
	fflush(stdout);

    while((read_size = recv(sock,bufread,1024,0)) > 0 ){
        bufferstring += bufread;
		cout <<"Input: " <<bufferstring<<endl;
		if(check_enter(bufferstring)){
			result = ReversePolNotation(bufferstring);
			sprintf(res,"%f",result);
			cout << "Mathematical expression: "<<bufferstring<<endl;
			cout << "Result: "<< res <<endl;
			cout<<endl;
		}
		else{
			strcpy(res,"Input Error");
		}
		   bufferstring.clear();
           send(sock, res, sizeof(res), 0);   
		   fflush(stdout);
    }

        if(read_size == 0){
            cout << "Client disconnected"<<endl;
			count--;
            fflush(stdout);
            close(sock);
        }
        else if(read_size == -1){
            perror("recv failed");
        }
}
