#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
pthread_mutex_t lock;
// lock is a mutex variable which allows the server to lock the shared resource (winner variable) so that only one thread can access it at a time
int winner = 0;
// winner is an integer variable that keeps track of whether a client has already won the game. It starts off as 0, which means no client has won yet.
int random_number;
// random_number is the number that the clients will be trying to guess.
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *handle_client(void *sock_desc)
{
    int n, guess;
    int sock = *(int *)sock_desc;
    char buffer[256];
    /*handle_client is the function that runs on a separate thread for each client that connects to the server. It takes a void pointer as an argument, which is actually the socket descriptor for that specific client. Within the function, sock is set to the socket descriptor, n and guess are int variables used later in the code, buffer is a char array used to read and write data to/from the socket.*/

    while (1)
    {
        bzero(buffer, 256);
        n = read(sock, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        guess = atoi(buffer);
        if (guess == random_number)
        {
            pthread_mutex_lock(&lock);
            if (winner == 0)
            {
                n = write(sock, "Correct! You are the winner!", 29);
                if (n < 0)
                    error("ERROR writing to socket");
                winner = 1;
            }
            else
            {
                n = write(sock, "Correct! But, someone already won!", 33);
                if (n < 0)
                    error("ERROR writing to socket");
            }
            pthread_mutex_unlock(&lock);
            break;
            exit(1);
        }
        else if (guess < random_number)
        {
            n = write(sock, "You've Guessed Too low! Guess a lil higher", strlen("You've Guessed Too low! Guess a lil higher"));
            if (n < 0)
                error("ERROR writing to socket");
        }
        else
        {
            n = write(sock, "You've Guessed Too high! Guess a lil Lower", strlen("You've Guessed Too high! Guess a lil Lower"));
            if (n < 0)
                error("ERROR writing to socket");
        }
    }

    close(sock);
    return 0;
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    time_t t;
    srand((unsigned) time(&t));
    random_number = rand() % 100 + 1;
    printf("The Random Number is %d",random_number);
    fflush(stdout);
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread_id;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    

    pthread_mutex_init(&lock, NULL);

    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        if (pthread_create(&thread_id, NULL, handle_client, (void *)&newsockfd) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }

    pthread_mutex_destroy(&lock);

    return 0;
}
