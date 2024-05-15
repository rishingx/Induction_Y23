# === IMPORTING === #

import os
import sys
import socket
import threading
import subprocess
import configparser

# === GLOBAL VARIABLES === #

# Variables to store username & config file of current user
curr_config = ""

# Variable to store current working directory
curr_wd = "./"
config = configparser.ConfigParser()

# === UTILITY FUNCTIONS === #

def message_parser(message):
    words = message.split(" ")
    return words[0], words[1:]

def get_config_file(username):
    config_file = "./user_configs/" + username + ".ini"
    return config_file

def check_pass(username, password):
    config_file = get_config_file(username)
    config.read(config_file)

    if config['credentials']['password'] == password:
        return True
    else:
        return False

def check_ban(username):
    config_file = get_config_file(username)
    config.read(config_file)

    if config['security']['banned'] == "true":
        return True
    elif config['security']['banned'] == "false":
        return False

def check_root(username):
    config_file = get_config_file(username)
    config.read(config_file)

    if config['security']['root'] == "true":
        return True
    else:
        return False

# === SENDING AND RECEIVING FILES === #

def send_file(file_path, client):
    pass

def recv_file(file_path, client):
    print("Will store the chunks at:", file_path)
    with open(file_path, 'wb') as file:
        content = client.recv(1024)
        file.write(content)


# === GLOBALLY START THE SERVER === #

# Create a socket object for the server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

IP_ADDR = socket.gethostbyname(socket.gethostname())
port = 1238

# Bind the server to listen on port 1234
server.bind((IP_ADDR,port))

# Start listening for incoming connections
server.listen()

# Print a message indicating that the server is listening
print(f"Server is listening on {IP_ADDR}:{port}")

# === FUNCTIONS === #

def USER(arg_list, client):
    username = arg_list[0]
    config_file = get_config_file(username)

    if (os.path.exists(config_file)):

        client.send("PASS?".encode())
        password = client.recv(1024).decode()
        arg_list.append(password)
        return PASS(arg_list, client)

    else:
        return "User does not exist!\n"

def PASS(arg_list, client):
    username, password = arg_list

    if check_pass(username, password):
        if check_ban(username):
            return "User is banned!\n"
        return "SUCCESS"
    else:
        return "Invalid Password!\n"

def LIST(arg_list, client):
    result = subprocess.run(['ls', '-l', curr_wd], stdout=subprocess.PIPE)
    return result.stdout.decode()

def RETR(arg_list, client):
    return "file sent"

def STOR(arg_list, client):
    file_name = arg_list[0].split("/")[-1]
    file_path = curr_wd + file_name
    recv_file(file_path, client)
    return "file recieved"

def QUIT(arg_list, client):
    client.close()
    return "Quit Successful"

def ADDUSER(arg_list, client):
    username, password = arg_list
    new_config_file = get_config_file(username)
    
    if (os.path.exists(new_config_file)):
        return "User already exists!\n"
    
    new_config = configparser.ConfigParser()
    new_config['credentials'] = {'username': username, 'password': password}
    new_config['security'] = {'root': 'false', 'banned': 'false'}

    with open(get_config_file(username), 'w') as config_file:
        new_config.write(config_file)

    return f"Added user {username}!\n"

def DELUSER(arg_list, client):
    username = arg_list[0]
    config_file = get_config_file(username)

    if (os.path.exists(config_file)):
        os.remove(config_file)
        return f"Deleted user {username}!\n"
    
    else:
        return f"User {username} does not exist!\n"

def BAN(arg_list, client):
    username = arg_list[0]
    
    config_file_path = get_config_file(username)
    config.read(config_file_path)
    config['security']['banned'] = 'true'

    with open(config_file_path, 'w') as config_file:
        config.write(config_file)
    
    return f"Banned user {username}!\n"

def UNBAN(arg_list, client):
    username = arg_list[0]

    config_file_path = get_config_file(username)
    config.read(config_file_path)
    config['security']['banned'] = 'false'

    with open(config_file_path, 'w') as config_file:
        config.write(config_file)
    
    return f"Unbanned user {username}!\n"

# Command strings to Python functions
command_to_functions = {
        "USER":     USER,
        "PASS":     PASS,
        "LIST":     LIST,
        "RETR":     RETR,
        "STOR":     STOR,
        "QUIT":     QUIT,
        "ADDUSER":  ADDUSER,
        "DELUSER":  DELUSER,
        "BAN":      BAN,
        "UNBAN":    UNBAN
        }

root_commands = ["ADDUSER", "DELUSER", "BAN", "UNBAN"]

# Function to accept incoming client connections
def accept_client():
    while True:
        # Accept a new client connection
        client, address = server.accept()
        print(f"\nConnection received from {address}")
        print(f"Active Connection : {threading.active_count()}")

        # Send a welcome message to the client
        client.send("\nYou are connected to the server".encode())

        # Start a new thread to handle the client
        t = threading.Thread(target=handle_client, args=(client,))
        t.start()

# Function to handle messages from a specific client
def handle_client(client):

    curr_user = ""
    message = client.recv(1024).decode()
    command, arguments = message_parser(message)

    if command == "USER":
        feedback = USER(arguments, client)
        if feedback == "SUCCESS":
            curr_user = arguments[0]
        client.send(feedback.encode())

    while curr_user != "":
        try:
            # Receive a message from the client
            message = client.recv(1024).decode()
            print(curr_user, ":", message)
            command, arguments = message_parser(message)
            
            if command in command_to_functions:
                if (command in root_commands) and not(check_root(curr_user)):
                    feedback = "You need root privileges!\n"
                else:
                    feedback = command_to_functions[command](arguments, client)
            else:
                feedback = "Invalid command"

            client.send(feedback.encode())

        except Exception as e:
            # If an error occurs, remove the client and their nickname, then close the connection
            print("Error:", e)
            client.close()
            print(f"Active Connection : {threading.active_count()}")
            return


# Start accepting client connections
if __name__ == "__main__":

    # Start accepting clients
    accept_client()

