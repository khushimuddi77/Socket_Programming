# Socket_Programming
Implementing a multiplayer game called GUESS THE INTEGER with the help of socket programming.
In a Multiplayer/Multi-client number guessing game using socket programming, multiple clients connect to a server via sockets and attempt to guess a randomly generated number. The server keeps track of the clients guesses and sends messages to each client indicating whether their guess is too high, too low, or correct. The first client to guess the correct number wins the game. The game can be implemented using various programming languages such as C/C++.
Algorithm
Server side:
1. Generate a random number between 1 and 100(inclusive).
2. Create a socket and bind it to a specific address andport.
3. Listen for incoming connections on the socket.
4. Initialize an empty dictionary to keep track of connected clients and their current guesses.
5. Enter a loop to continuously listen for new connections.
6. Receive the client's guess Compare the client's guess to the randomly generated number.
7. If the guess is incorrect, send a message to the client indicating whether their guess was too high or too low.
8. If the guess is correct, send a message to the client
indicating that they have won the game, disconnect
the client and break the loop.
9. Continue the loop to listen for new connections.
    
B.Client side:
1. Create a socket and connect it to the server's address and port.
2. Continuously prompt the user for a guess. Send the user's guess to the server.
3. Receive feedback from the server indicating whether the guess was too high, too low, or correct
4. If the guess was correct, print a message indicating that the user has won the game or if somebody else has won the game and exit the program.
5. If the guess was incorrect, print the feedback received from the server and prompt the user for another guess.
6. Repeat steps 2 to 6 until the user guesses the correct number or the connection to the server is closed

