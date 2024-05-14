import socket
import cv2

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('localhost', 4444))
server.listen()

print("Server is listening on localhost:4444")

while True:
    client, address = server.accept()
    print(f"Connection received from {address}")

    # Initialize the webcam
    webcam = cv2.VideoCapture(0)

    # Read frames from the webcam and send them over the socket
    while True:
        ret, frame = webcam.read()
        if not ret:
            break
        
        # Display the frame locally
        cv2.imshow('Server - Webcam Stream', frame)

        # Encode the frame as JPEG
        encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
        _, jpeg_frame = cv2.imencode('.jpg', frame, encode_param)

        # Send the size of the JPEG frame first
        client.sendall(len(jpeg_frame).to_bytes(8, byteorder='big'))
        # Send the JPEG frame data
        client.sendall(jpeg_frame.tobytes())

        # Exit loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    webcam.release()
    cv2.destroyAllWindows()
    client.close()


