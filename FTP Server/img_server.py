import socket

# Create a TCP/IP socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('localhost', 1234))
server.listen(5)

print("Server is listening on localhost:1234")

while True:
    client, address = server.accept()
    print(f"Connection received from {address}")

    # Open the image file in binary mode
    with open('test.jpeg', 'rb') as file:
        # Read the image data
        image_data = file.read()

    # Send the image size first
    client.sendall(len(image_data).to_bytes(4, byteorder='big'))
    # Send the image data
    client.sendall(image_data)

    client.close()
