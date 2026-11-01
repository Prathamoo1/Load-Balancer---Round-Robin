
# Round Robin Load Balancer (C++)

A simple TCP-based Load Balancer implemented in C++ using the **Round Robin Scheduling Algorithm**.

This project demonstrates how incoming client requests can be distributed across multiple backend servers in a sequential, rotating manner using socket programming.

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Project Structure](#project-structure)
- [Technologies Used](#technologies-used)
- [Prerequisites](#prerequisites)
- [Installation and Setup](#installation-and-setup)
- [Compilation](#compilation)
- [Running the Project](#running-the-project)
- [Testing the Load Balancer](#testing-the-load-balancer)
- [Expected Output](#expected-output)
- [Troubleshooting](#troubleshooting)
- [Limitations](#limitations)

## Overview

A load balancer distributes incoming network requests across multiple backend servers instead of sending every request to a single server.

This project implements a basic TCP load balancer using the **Round Robin algorithm**.

The load balancer listens for client connections on port `8080` and forwards requests alternately to two backend servers:

- Server 1: Port `9001`
- Server 2: Port `9002`

The backend server processes the request and sends a response back through the load balancer to the client.

## How It Works

The project follows a simple client-server architecture.

### Architecture

```text
                 CLIENT
                    |
                    |
              Port 8080
                    |
                    v
           +------------------+
           |  LOAD BALANCER   |
           |                  |
           |  Round Robin     |
           |  Scheduling      |
           +------------------+
                    |
           +--------+--------+
           |                 |
           v                 v
    +-------------+   +-------------+
    |  SERVER 1   |   |  SERVER 2   |
    |  Port 9001  |   |  Port 9002  |
    +-------------+   +-------------+
           |                 |
           +--------+--------+
                    |
                    v
             CLIENT RESPONSE
```

### Round Robin Algorithm

The load balancer maintains a counter to keep track of which backend server should receive the next request.

The selection process is:

1. The first request is forwarded to Server 1.
2. The second request is forwarded to Server 2.
3. The third request is forwarded to Server 1.
4. The fourth request is forwarded to Server 2.

This process continues in a repeating cycle.

**Example:**

| Client Request | Selected Server |
|---|---|
| Request 1 | Server 1 |
| Request 2 | Server 2 |
| Request 3 | Server 1 |
| Request 4 | Server 2 |
| Request 5 | Server 1 |
| Request 6 | Server 2 |

## Project Structure

```text
Load-Balancer---Round-Robin/
│
├── lb.cpp          # Main load balancer implementation
├── ser1.cpp        # Backend Server 1 (Port 9001)
├── ser2.cpp        # Backend Server 2 (Port 9002)
├── cli1.cpp        # Client implementation (Windows/Winsock)
├── server.cpp      # Separate HTTP server implementation
│
└── README.md       # Project documentation
```

### File Description

| File | Description |
|---|---|
| `lb.cpp` | Accepts client connections, selects a backend server using Round Robin, forwards requests, and returns responses. |
| `ser1.cpp` | Implements the first TCP backend server on port 9001. |
| `ser2.cpp` | Implements the second TCP backend server on port 9002. |
| `cli1.cpp` | Contains a Windows-based client implementation using Winsock. |
| `server.cpp` | Contains a separate HTTP server implementation and is not required for the basic TCP demonstration. |

## Technologies Used

- **C++** – Core programming language.
- **TCP/IP** – Communication between clients, load balancer, and backend servers.
- **Socket Programming** – Establishing network connections and transferring data.
- **Round Robin Algorithm** – Distributing incoming requests across backend servers.
- **Linux / POSIX Sockets** – Used by the load balancer and backend servers.

## Prerequisites

For the Linux-based TCP setup, you need:

- Ubuntu Linux or Windows Subsystem for Linux (WSL).
- GCC/G++ compiler.
- Git.
- Python 3 (for the optional testing client).

### Install dependencies

On Ubuntu or WSL, open a terminal and run:

```bash
sudo apt update
sudo apt install g++ git python3 -y
```

If you are using Windows and do not have WSL installed, open PowerShell as Administrator and execute:

```powershell
wsl --install -d Ubuntu
```

Restart your system if prompted and launch Ubuntu.

## Installation and Setup

### 1. Clone the repository

```bash
git clone https://github.com/Prathamoo1/Load-Balancer---Round-Robin.git
```

### 2. Navigate to the project directory

```bash
cd Load-Balancer---Round-Robin
```

### 3. Verify the project files

```bash
ls
```

You should see the following source files:

```text
README.md
lb.cpp
ser1.cpp
ser2.cpp
cli1.cpp
server.cpp
```

## Compilation

Compile the three components required for the TCP load balancing demonstration.

### Compile Backend Server 1

```bash
g++ ser1.cpp -o server1
```

### Compile Backend Server 2

```bash
g++ ser2.cpp -o server2
```

### Compile the Load Balancer

```bash
g++ lb.cpp -o loadbalancer
```

After successful compilation, the following executables will be created:

```text
server1
server2
loadbalancer
```

**Note:** `cli1.cpp` is a Windows-specific client using Winsock. It is not required for the Python-based testing method described below. `server.cpp` is also not part of this basic TCP execution flow.

## Running the Project

The backend servers and load balancer must run simultaneously in separate terminal windows.

Open **four terminal windows** in Ubuntu/WSL.

### Terminal 1: Start Server 1

Navigate to the project directory and execute:

```bash
./server1
```

Expected output:

```text
Server 1 running on port 9001
```

### Terminal 2: Start Server 2

Navigate to the project directory and execute:

```bash
./server2
```

Expected output:

```text
Server 2 running on port 9002
```

### Terminal 3: Start the Load Balancer

Navigate to the project directory and execute:

```bash
./loadbalancer
```

Expected output:

```text
C++ LOAD BALANCER STARTED
Listening on port 8080...
```

The load balancer is now ready to accept client connections.

### Terminal 4: Run the Client

You can use the Python testing client below to send multiple requests to the load balancer.

## Testing the Load Balancer

Run the following Python script in the fourth terminal:

```bash
python3 - <<'PY'
import socket

for i in range(1, 7):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.connect(("127.0.0.1", 8080))

    message = f"Request {i}"
    s.sendall(message.encode())

    response = s.recv(4096).decode()

    print(f"{message} -> {response}")

    s.close()
PY
```

This script:

1. Creates a TCP socket.
2. Connects to the load balancer on port `8080`.
3. Sends six requests, one at a time.
4. Receives and displays the response.
5. Closes the connection after each request.

## Expected Output

### Client Terminal

The client should display responses similar to:

```text
Request 1 -> Hello from Server 1
Request 2 -> Hello from Server 2
Request 3 -> Hello from Server 1
Request 4 -> Hello from Server 2
Request 5 -> Hello from Server 1
Request 6 -> Hello from Server 2
```

### Load Balancer Terminal

The load balancer terminal will display logs similar to:

```text
C++ LOAD BALANCER STARTED
Listening on port 8080...

Client connected
Forwarding to server on port 9001
Request: Request 1
Response received from server
Request completed.

Client connected
Forwarding to server on port 9002
Request: Request 2
Response received from server
Request completed.
```

### Backend Server Terminals

Server 1:

```text
Server 1 running on port 9001
Server 1 received: Request 1
Server 1 received: Request 3
Server 1 received: Request 5
```

Server 2:

```text
Server 2 running on port 9002
Server 2 received: Request 2
Server 2 received: Request 4
Server 2 received: Request 6
```

If the requests alternate between the two servers, the Round Robin scheduling mechanism is working as expected.

## Troubleshooting

### 1. Address already in use

**Error:**

```text
Bind failed: Address already in use
```

**Solution:**

Another process may already be using the required port.

Check the port using:

```bash
sudo lsof -i :8080
```

Replace `8080` with `9001` or `9002` to check the backend ports.

Stop the previous process if it is no longer needed.

### 2. Backend server unavailable

**Error:**

```text
Backend server unavailable
```

**Solution:**

Make sure both backend servers are running before starting the load balancer.

Check that Server 1 is listening on port `9001` and Server 2 on port `9002`.

### 3. Connection refused

**Error:**

```text
Connection refused
```

**Solution:**

- Verify that the load balancer is running.
- Check that it is listening on port `8080`.
- Ensure the client is connecting to `127.0.0.1:8080`.

### 4. Compilation errors

Make sure that:

- G++ is installed.
- You are executing commands from the project directory.
- You are using Ubuntu/WSL for the POSIX socket implementations.

## Limitations

This project is intended as an educational demonstration of TCP socket programming and Round Robin load balancing.

Current limitations include:

- Supports two backend servers.
- Handles client connections sequentially rather than concurrently.
- Does not implement health checks or automatic server recovery.
- Does not include dynamic backend server registration.
- Uses a basic TCP request-response mechanism rather than a complete production HTTP proxy.

## Learning Outcomes

By exploring this project, you can understand:

- The fundamentals of TCP socket programming.
- Client-server communication.
- How a load balancer distributes incoming requests.
- The working of the Round Robin scheduling algorithm.
- Communication between multiple backend servers.
- How to compile and execute multi-component C++ network applications.

---

**If you find this project useful for learning socket programming and load balancing, feel free to explore, modify, and improve it.**
