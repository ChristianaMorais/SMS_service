Socket explanation
-------------------------------------------------------------------
The call to the function ‘socket()’ creates an UN-named socket inside the kernel and returns an integer known as socket descriptor.
This function takes domain/family as its first argument. For Internet family of IPv4 addresses we use AF_INET.
The second argument ‘SOCK_STREAM’ specifies that the transport layer protocol that we want should be reliable ie it should have acknowledgement techniques. For example : TCP
The third argument is generally left zero to let the kernel decide the default protocol to use for this connection. For connection oriented reliable connections, the default protocol used is TCP.
The call to the function ‘bind()’ assigns the details specified in the structure ‘serv_addr’ to the socket created in the step above. The details include, the family/domain, the interface to listen on(in case the system has multiple interfaces to network) and the port on which the server will wait for the client requests to come.
The call to the function ‘listen()’ with second argument as ’10’ specifies maximum number of client connections that server will queue for this listening socket.
After the call to listen(), this socket becomes a fully functional listening socket.
In the call to accept(), the server is put to sleep and when for an incoming client request, the three way TCP handshake* is complete, the function accept () wakes up and returns the socket descriptor representing the client socket.
The call to accept() is run in an infinite loop so that the server is always running and the delay or sleep of 1 sec ensures that this server does not eat up all of your CPU processing.
As soon as server gets a request from client, it prepares the date and time and writes on the client socket through the descriptor returned by accept().
------------------------------------------------------------------