import socket
import mysql.connector
from datetime import datetime, timedelta
import time

# MySQL Database Configuration
db_config = {
    'host': 'mysql_dockerproject',  # Use the service name defined in Docker Compose
    'user': 'maisurah',
    'password': 'patrix',
    'database': 'sir_db',
}

def get_user_points(user_id):
    # Establish a new connection for each query to avoid timeout issues
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()

    # Fetch user's points from the database
    cursor.execute('SELECT points, datetime_stamp FROM mytable WHERE user_attribute = %s', (user_id,))
    result = cursor.fetchone()
    if result:
        points, timestamp = result
        response = f"User {user_id}: Points = {points}, Last Updated: {timestamp}"
    else:
        response = f"User {user_id} not found in the database."

    cursor.close()
    conn.close()

    return response

def update_user_points(user_id, points):
    # Establish a new connection for each query to avoid timeout issues
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()

    # Update user's points in the database
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    cursor.execute('UPDATE mytable SET points = %s, datetime_stamp = %s WHERE user_attribute = %s', (points, timestamp, user_id))
    conn.commit()

    cursor.close()
    conn.close()

def handle_client_connection(client_socket):
    # Receive user_id and request from the client
    user_id = client_socket.recv(BUFFER_SIZE).decode()
    request = client_socket.recv(BUFFER_SIZE).decode()

    # Handle the request
    if request == 'get_points':
        response = get_user_points(user_id)
    elif request.startswith('add_points'):
        _, points_to_add = request.split(':')
        points_to_add = int(points_to_add)
        conn = mysql.connector.connect(**db_config)
        cursor = conn.cursor()
        cursor.execute('SELECT points FROM mytable WHERE user_attribute = %s', (user_id,))
        current_points = cursor.fetchone()
        cursor.close()
        conn.close()
        if current_points:
            current_points = current_points[0]
            new_points = current_points + points_to_add
            update_user_points(user_id, new_points)
            response = f"Points added. New Points = {new_points}"
        else:
            response = f"User {user_id} not found in the database."
    else:
        response = "Invalid request."

    # Send the response back to the client
    client_socket.send(response.encode())
    client_socket.close()

# Server Configuration
HOST = '0.0.0.0'
PORT = 56789
BUFFER_SIZE = 1024

# Main Server Loop
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(5)
print(f"Server listening on {HOST}:{PORT}")

while True:
    client_socket, client_address = server_socket.accept()
    print(f"Connection from {client_address}")
    handle_client_connection(client_socket)

