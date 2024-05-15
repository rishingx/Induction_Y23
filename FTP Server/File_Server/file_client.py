import sys
import socket
import threading


# === GLOBALLY START THE CLIENT === #


# Create a socket object for the client
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

IP_ADDR = socket.gethostbyname(socket.gethostname())
port = 1238

# Connect to the server
client.connect((IP_ADDR,port))

# Expect welcome message
print(client.recv(1024).decode())

# === SENDING AND RECEIVING FILES === #

def send_file(file_path, client):
    with open(file_path, 'rb') as file:
        content = file.read()
        client.send(content)

def recv_file(file_path, client):
    pass


def message_parser(message):
    words = message.split(" ")
    return words[0], words[1:]


# Function to send messages to the server
def send_message():
    try:
        message = input("ftp> ")  # Take input from the user
        command, arguments = message_parser(message)

        client.send(message.encode())  # Send the message to the server along with the nickname

        if command == "STOR":
            file_path = arguments[0]
            print(file_path)
            send_file(file_path, client)

        if command == "QUIT":
            client.close()
            sys.exit(0)
        
    except Exception as e:
        print("An error occurred!", e)

# Function to receive messages from the server
def receive_message():
    try:
        message = client.recv(1024).decode()  # Receive messages from the server
        print(message)  # Print the received messages

    except Exception as e:
        print("An error occurred!", e)


if __name__ == "__main__":

    # Loop until a valid username, password pair is entered
    while True:
        username = input("Username: ")
        client.send(("USER " + username).encode())
        reply = client.recv(1024).decode()

        if reply == "PASS?":

            password = input("Password: ")
            client.send(password.encode())

            reply = client.recv(1024).decode()
            print(reply)

            if reply == "SUCCESS":
                break
            else:
                print(reply)

        else:
            print(reply)



    while True:
        send_message()
        receive_message()