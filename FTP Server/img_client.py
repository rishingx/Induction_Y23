import socket

# Create a TCP/IP socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('localhost', 1234))

# Receive the image size
image_size_bytes = client.recv(4)
image_size = int.from_bytes(image_size_bytes, byteorder='big')

# Receive the image data
image_data = b''
while len(image_data) < image_size:
    chunk = client.recv(image_size - len(image_data))
    if not chunk:
        break
    image_data += chunk

# Save the received image data to a file
with open('received_image.jpg', 'wb') as file:
    file.write(image_data)

client.close()
