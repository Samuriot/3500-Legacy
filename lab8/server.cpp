/**
 * This code creates a server socket and binds it to port 8080.
 * It then listens for incoming connections and accepts the first
 * connection that comes in. It sends a "Hello, client!" message
 * to the client and receives a message from the client.
 */

#include <iostream>
using namespace std;

//needed for socket API
#include <sys/socket.h> // Used for Socket API
#include <netinet/in.h> // Used for socketaddr_in
#include <unistd.h>     // Used to close socket


int main() {
    // create a socket
    // IPV4,TCP,IP
    int server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // specify the server address and port
    struct sockaddr_in server_socket_address;
    server_socket_address.sin_family = AF_INET;
    server_socket_address.sin_addr.s_addr = INADDR_ANY;
    server_socket_address.sin_port = htons(8080);

    // bind the socket to the server address and port
    struct sockaddr *server_addr_arg = (struct sockaddr *) &server_socket_address;
    bind(server_socket_descriptor, server_addr_arg, sizeof(server_socket_address));

    // listen for incoming connection can queue up to 3 incoming request
    listen(server_socket_descriptor, 3);

    //////////////// NOW SERVER IS IN PASSIVE OPEN STATE /////////////////////////
    cout << "Server started" << endl;
    cout << "Listening on port " << 8080 << endl;

    // accept incoming connections
    cout << "finding clients" << endl;
    int client_socket_descriptor;
    struct sockaddr_in client_socket_address;
    struct sockaddr *client_addr_arg = (struct sockaddr *) &client_socket_address;
    socklen_t client_addr_len = sizeof(client_socket_address);
    client_socket_descriptor = accept(server_socket_descriptor, client_addr_arg, &client_addr_len);
    cout << "client found" << endl;

    // send data to the client
    string message = "Hello, client!\0";
    const char *message_arg = message.c_str();
    send(client_socket_descriptor, message_arg, message.length(), 0);

    // receive data from the client
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE] = {0}; // Create a buffer of size 1024 Bytes and default it to 0.
    recv(client_socket_descriptor, buffer, BUFFER_SIZE, 0);

    // as long as zero at the end the operator knows when to stop
    cout << "Client message: " << buffer << endl;

    close(server_socket_descriptor);
    close(client_socket_descriptor);

}
