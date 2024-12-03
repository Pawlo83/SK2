import socket
import time
import cv2
import numpy as np
import time

# Tworzenie gniazda TCP
tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Adres i port serwera
server_address = ("127.0.0.1", 12345)
tcp_socket.connect(server_address)
print(f"Connected to server at {server_address}")

try:
    try:
        cap = cv2.VideoCapture(3)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 360)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 202)

        while (cap.isOpened()):
            ret, frame = cap.read()
            byteImage = cv2.imencode('.jpg', frame)[1].tobytes()
            message = byteImage
            #cv2.imshow("Captured Frame", frame)
            #cv2.waitKey(0)
            bytes_sent=0
            while(bytes_sent < len(message)):
                new_message=message[bytes_sent:]
                bytes_sent += tcp_socket.send(new_message)
            tcp_socket.send("\n".encode('utf-8'))

            response = tcp_socket.recv(100000)
            nparr = np.frombuffer(response, np.uint8)
            image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
            cv2.imshow("Captured Image", image)

            cv2.waitKey(int(100/4))

        # nparr = np.frombuffer(byteImage, np.uint8)
        # image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        # conn.write(byteImage)

        #cv2.imshow("Captured Frame", frame)
        # cv2.imshow("Captured Image",image)
        #cv2.waitKey(0)


    finally:
        cap.release()

    #response = tcp_socket.recv(20000)
    #nparr = np.frombuffer(response, np.uint8)
    #image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
    #cv2.imshow("Captured Image", image)
    #cv2.waitKey(0)
    # print(f"Received response from server: {response.decode('utf-8')}")

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    # Zamknięcie połączenia
    tcp_socket.close()
    print("Connection closed")
