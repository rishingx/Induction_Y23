import socket
import threading

# Create a socket object for the server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

IP_ADDR = socket.gethostbyname(socket.gethostname())
port = 1234

# Bind the server to listen on port 1234
server.bind((IP_ADDR,port))

# Start listening for incoming connections
server.listen()

# Print a message indicating that the server is listening
print(f"Server is listening on {IP_ADDR}:{port}")

# Lists to keep track of connected clients and their nicknames
clients = []
nicknames = []

# Function to broadcast messages to all clients except the sender
def broadcast(message, sender_client):
    for client in clients:
        if client != sender_client:
            client.send(message.encode('utf-8'))

# Function to accept incoming client connections
def accept_client():
    while True:
        # Accept a new client connection
        client, address = server.accept()
        print(f"\nConnection received from {address}")

        # Ask the client for their nickname
        client.send("NICK?".encode())
        nick = client.recv(1024).decode()

        # Add the client and their nickname to the lists
        clients.append(client)
        nicknames.append(nick)
        print(f"\nNickname of the client is {nick}")
        print(f"Active Connection : {threading.active_count()}")

        # Send a welcome message to the client
        client.send("\nYou are connected to the server".encode())

        # Broadcast that the client has joined the chat to all other clients
        broadcast(f"\n{nick} has joined the chat.", client)

        # Start a new thread to handle the client
        t = threading.Thread(target=handle_client, args=(client,))
        t.start()

# Function to handle messages from a specific client
def handle_client(client):
    while True:
        try:
            # Receive a message from the client
            message = client.recv(1024).decode()
            # Broadcast the message to all other clients
            broadcast(message, client)
        except:
            # If an error occurs, remove the client and their nickname, then close the connection
            index = clients.index(client)
            clients.remove(client)
            client.close()
            nickname = nicknames[index]
            broadcast(f"{nickname} has left the chat.".encode(), client)
            nicknames.remove(nickname)
            print(f"Active Connection : {threading.active_count()}")
            break

# Start accepting client connections
if __name__ == "__main__":
    accept_client()
