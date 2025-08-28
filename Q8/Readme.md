# Q8: Leader-Follower Threading Pattern Server

Multi-threaded server implementing the Leader-Follower pattern that executes all 4 algorithms on each graph request.

## Prerequisites

```bash
cd ../Q1toQ4 && make
cd ../Q7 && make
cd ../Q8
```

## Build

```bash
make
```

## Run

### Server
```bash
./lf_server [port] [workers]
# Default: port=8082, workers=4
```

### Client
```bash
./lf_client [host] [port]
# Default: localhost 8082
```

## Leader-Follower Pattern Implementation

### How It Works
1. **Only ONE worker acts as Leader** at any time
2. **Leader accepts incoming requests** from the request queue
3. **Leader immediately promotes a new Leader** before processing
4. **Former Leader processes the request as Follower**
5. **All other workers wait as Followers** until promoted

### Key Features
- True Leader-Follower pattern (not thread pool)
- Leader promotions logged with statistics
- Only one thread accepts work at a time
- Immediate leader handoff for better concurrency

## Algorithm Execution (Q8 Requirement)

Each graph request triggers execution of **ALL 4 algorithms**:

1. **Euler Circuit** - Finds Euler circuit or proves none exists
2. **Max Clique** - Finds maximum clique size
3. **MST Weight** - Calculates minimum spanning tree weight  
4. **Hamilton Circuit** - Finds Hamilton circuit or proves none exists

## Usage

### Graph Format
```
vertices edges edge1 edge2 ...
```

### Examples
```bash
# Triangle graph
3 3 0-1 1-2 2-0

# Square graph  
4 4 0-1 1-2 2-3 3-0

# Complete graph K4
4 6 0-1 0-2 0-3 1-2 1-3 2-3
```

### Commands
- `help` - Show format and usage
- `stats` - Show Leader-Follower statistics
- `quit` - Exit connection

### Expected Output
Each graph input returns results from all 4 algorithms:

```
=== ALL 4 ALGORITHMS RESULTS (Q8) ===
Graph: 3 vertices, 3 edges

1. EULER CIRCUIT: YES - Circuit: 0->1->2->0 (25us)
2. MAX CLIQUE: Size: 3 - Vertices: {0,1,2} (18us)
3. MST WEIGHT: Weight: 2 (Edges: 2) (12us)
4. HAMILTON CIRCUIT: YES - Circuit: 0->1->2->0 (8us)

Total execution time: 63us
=====================================
```

## Threading Model

### Leader-Follower vs Thread Pool
- **Thread Pool**: All threads compete for work
- **Leader-Follower**: Only ONE thread (Leader) accepts work at a time

### Benefits
- Eliminates contention on accept queue
- Better cache locality
- Clear responsibility model
- Predictable leader promotions

## Statistics Tracking

Use `stats` command to see:
- Total connections handled
- Total requests processed  
- Number of leader promotions
- Current active leader
- Number of worker threads

## Performance Notes

- All 4 algorithms execute sequentially for each request
- Leader promotion happens immediately for better concurrency
- Individual algorithm timing plus total execution time reported
- Suitable for moderate load with complex processing per request

## Files

- `server_main.cpp` - Main server program
- `LFServer.h/cpp` - Leader-Follower pattern implementation
- `client.cpp` - Test client
- `Makefile` - Build system (reuses algorithms from Q7)