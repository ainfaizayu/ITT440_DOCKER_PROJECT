#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8082
#define SERVER_IP "172.18.0.2"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    char option[256];
    char request[512];
    char response[256];

    while (1) {
        printf("\nOptions:\n");
        printf("1. Add user\n");
        printf("2. Delete user\n");
        printf("3. Show Database\n");
        printf("4. End Connection\n");

        printf("\nEnter option: ");
        fgets(option, sizeof(option), stdin);

        option[strcspn(option, "\n")] = '\0';

        if (strcmp(option, "1") == 0) {
            char username[256];
            char pointsStr[256];

            printf("Enter the username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';

            printf("Enter the points: ");
            fgets(pointsStr, sizeof(pointsStr), stdin);
            pointsStr[strcspn(pointsStr, "\n")] = '\0';

            int len = snprintf(request, sizeof(request), "ADD_USER %s %s", username, pointsStr);
            if (len < 0 || len >= sizeof(request)) {
                printf("Request exceeds buffer size. Aborting.\n");
                break;
            }
            send(sock, request, len, 0);

            recv(sock, response, sizeof(response), 0);
            printf("Server response: %s\n", response);

            printf("Do you want to continue the connection? (y/n): ");
            fgets(option, sizeof(option), stdin);
            option[strcspn(option, "\n")] = '\0';

            if (strcmp(option, "n") == 0 || strcmp(option, "N") == 0) {
                int len = snprintf(request, sizeof(request), "END_CONN");
                if (len < 0 || len >= sizeof(request)) {
                    printf("Request exceeds buffer size. Aborting.\n");
                    break;
                }
                send(sock, request, len, 0);
                break;
            }

        } else if (strcmp(option, "2") == 0) {
            char username[256];

            printf("Enter the username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';

            int len = snprintf(request, sizeof(request), "DELETE_USER %s", username);
            if (len < 0 || len >= sizeof(request)) {
                printf("Request exceeds buffer size. Aborting.\n");
                break;
            }
            send(sock, request, len, 0);

            recv(sock, response, sizeof(response), 0);
            printf("Server response: %s\n", response);

            printf("Do you want to continue the connection? (y/n): ");
            fgets(option, sizeof(option), stdin);
            option[strcspn(option, "\n")] = '\0';

            if (strcmp(option, "n") == 0 || strcmp(option, "N") == 0) {
                int len = snprintf(request, sizeof(request), "END_CONN");
                if (len < 0 || len >= sizeof(request)) {
                    printf("Request exceeds buffer size. Aborting.\n");
                    break;
                }
                send(sock, request, len, 0);
                break;
            }

        } else if (strcmp(option, "3") == 0) {
            int len = snprintf(request, sizeof(request), "SHOW_DB");
            if (len < 0 || len >= sizeof(request)) {
                printf("Request exceeds buffer size. Aborting.\n");
                break;
            }
            send(sock, request, len, 0);

            printf("\nServer response:\n");
            while (1) {
                recv(sock, response, sizeof(response), 0);
                if (strcmp(response, "Database contents sent") == 0) {
                    break;
                }
                printf("%s\n", response);
            }

            printf("Do you want to continue the connection? (y/n): ");
            fgets(option, sizeof(option), stdin);
            option[strcspn(option, "\n")] = '\0';

            if (strcmp(option, "n") == 0 || strcmp(option, "N") == 0) {
                int len = snprintf(request, sizeof(request), "END_CONN");
                if (len < 0 || len >= sizeof(request)) {
                    printf("Request exceeds buffer size. Aborting.\n");
                    break;
                }
                send(sock, request, len, 0);
                break;
            }
        } else if (strcmp(option, "4") == 0) {
            int len = snprintf(request, sizeof(request), "END_CONN");
            if (len < 0 || len >= sizeof(request)) {
                printf("Request exceeds buffer size. Aborting.\n");
                break;
            }
            send(sock, request, len, 0);
            break;
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }

    close(sock);

    return 0;
}

