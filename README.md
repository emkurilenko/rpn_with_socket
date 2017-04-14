# rpn_with_socket
Laboratory work. Using Sockets. The task "Calculating a mathematical expression with brackets".  Protocol of interaction TCP. Use pthread/Ubuntu

makeFile.sh server

	#!/bin/bash
	g++ -pthread -o server.out server.cpp	
	./server.out
  
makeFile.sh client

	#!/bin/bash
	g++ -o client.out server.cpp
	./client.out localhost
