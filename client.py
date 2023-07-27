import socket

# Server Configuration
HOST = 'py_server_dockerproject'  # Use the service name defined in Docker Compose
PORT = 56789
BUFFER_SIZE = 1024

def get_points(user_id):
    # Establish a connection to the server
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((HOST, PORT))

    # Send user_id and request to the server
    client_socket.send(user_id.encode())
    client_socket.send('get_points'.encode())

    # Receive and decode the response from the server
    response = client_socket.recv(BUFFER_SIZE).decode()

    # Close the connection
    client_socket.close()

    return response

def add_points(user_id, points_to_add):
    # Establish a connection to the server
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((HOST, PORT))

    # Send user_id and request to the server
    request = f'add_points:{points_to_add}'
    client_socket.send(user_id.encode())
    client_socket.send(request.encode())

    # Receive and decode the response from the server
    response = client_socket.recv(BUFFER_SIZE).decode()

    # Close the connection
    client_socket.close()

    return response

# Sample usage of the client functions
if __name__ == '__main__':
    user_id = input("Enter user ID: ")
    points = int(input("Enter points to add: "))

    response = get_points(user_id)
    print(response)

    response = add_points(user_id, points)
    print(response)
