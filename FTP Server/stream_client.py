import socket
import cv2
import numpy as np

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('localhost', 4444))

while True:
    # Receive the size of the JPEG frame
    frame_size_bytes = client.recv(8)
    frame_size = int.from_bytes(frame_size_bytes, byteorder='big')

    # Receive the JPEG frame data
    frame_data = b''
    while len(frame_data) < frame_size:
        chunk = client.recv(frame_size - len(frame_data))
        if not chunk:
            break
        frame_data += chunk

    # Decode the JPEG frame
    frame = cv2.imdecode(np.frombuffer(frame_data, dtype=np.uint8), cv2.IMREAD_COLOR)

    # Display the frame
    cv2.imshow('Client - Webcam Stream', frame)

    # Exit loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

client.close()
cv2.destroyAllWindows()
