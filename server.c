#include <stdlib.h>
#include <stdio.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <mysql/mysql.h>

#define DB_HOST "172.18.0.2"
#define DB_PORT 3306 
#define DB_USER "maisurah" // Updated MySQL username
#define DB_PASSWORD "patrix" // Updated MySQL password
#define DB_NAME "sir_db" // Updated MySQL database name
#define PORT 8082

// Function to handle incoming client connections
void onAccept(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ctx) {
    MYSQL *conn = (MYSQL *)ctx;

    // Handle incoming connection, read data, and perform database operations
    // Implement the logic for adding users, deleting users, etc.
    // Use MySQL connection 'conn' to interact with the database
    // ...

    // Close the connection after processing the client request
    close(fd);
}

int main() {
    struct event_base* base;
    struct evconnlistener* listener;
    struct sockaddr_in sin;
    int port = 8082; // Your desired port

    // Initialize the event base
    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize event base\n");
        return 1;
    }

    // Set up the server socket address
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);

    // Create a TCP listener to accept incoming connections
    listener = evconnlistener_new_bind(base, onAccept, (void *)conn, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        fprintf(stderr, "Could not create listener\n");
        return 1;
    }

    // Initialize the MySQL connection
    MYSQL* conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    // Connect to the MySQL database
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    // Enter the event loop
    event_base_dispatch(base);

    // Cleanup and close the MySQL connection
    mysql_close(conn);

    // Free resources
    evconnlistener_free(listener);
    event_base_free(base);

    return 0;
}
