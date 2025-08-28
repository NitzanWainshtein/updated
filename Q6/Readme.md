# Q6: Euler Circuit Server

Server that receives requests for Euler circuit analysis on graphs.

## Prerequisites

Make sure Q1toQ4 is built first:
```bash
cd ../Q1toQ4
make
cd ../q6
```

## Build

```bash
make
```

## Run

### Server
```bash
./euler_server [port]
# Default port: 8080
```

### Client
```bash
./euler_client [host] [port]
# Default: localhost 8080
```

### Alternative Connection
```bash
telnet localhost 8080
```

## Usage

### Graph Format
```
vertices edges edge1 edge2 ...
```

Edge format: `u-v` (e.g., `0-1`)

### Examples
```
3 3 0-1 1-2 2-0     # Triangle (Eulerian)
4 4 0-1 1-2 2-3 3-0 # Square (Eulerian)  
3 2 0-1 1-2         # Path (Non-Eulerian)
```

### Commands
- `help` - Show format
- `quit` - Exit

## Server Response

For each graph, server returns:
- Vertex and edge count
- Connectivity status
- Euler circuit: YES/NO
- Circuit path (if exists and short)

## Files

- `server_main.cpp` - Multi-threaded Euler server
- `client.cpp` - Simple client
- `Makefile` - Build system (uses Graph from ../Q1toQ4)