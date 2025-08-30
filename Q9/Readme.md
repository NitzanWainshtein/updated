# Q9: Pipeline Server with Active Objects

Multi-threaded server implementing a 4-stage pipeline pattern using Active Objects that executes all 4 algorithms on each graph request.

## Prerequisites

```bash
cd ../Q1toQ4 && make
cd ../Q7 && make
cd ../Q9
```

## Build

```bash
make
```

## Run

### Server
```bash
./pipeline_server [port]
# Default: port=8083
```

### Client
```bash
./pipeline_client [host] [port]
# Default: localhost 8083
```

## Pipeline Architecture Implementation

### 4-Stage Pipeline with Active Objects
1. **Stage 1: Request Receiver** - Parse and validate input format
2. **Stage 2: Graph Processor** - Additional graph validation and preparation
3. **Stage 3: Algorithm Executor** - Execute all 4 algorithms asynchronously
4. **Stage 4: Response Formatter** - Format results and send response

### Key Features
- Each stage runs in its own Active Object (separate thread)
- Asynchronous processing through all 4 stages
- Requests flow through pipeline: Request → Graph → Algorithms → Response
- Statistics tracking for each pipeline stage
- Error handling at each stage with proper logging

## Algorithm Execution (Q9 Requirement)

Each graph request triggers execution of **ALL 4 algorithms** in Stage 3:

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
- `help` - Show format and pipeline info
- `stats` - Show pipeline stage statistics
- `quit` - Exit connection

### Expected Output
Each graph input returns results from all 4 algorithms processed through the pipeline:

```
=== PIPELINE RESULTS (4 Active Objects) ===
Graph: 3 vertices, 3 edges

1. EULER: YES - Circuit: 0->1->2->0 (25us)
2. CLIQUE: Size: 3 - Vertices: {0,1,2} (18us)
3. MST: Weight: 2 (Edges: 2) (12us)
4. HAMILTON: YES - Circuit: 0->1->2->0 (8us)

Processed through 4-stage pipeline
========================================
```

## Active Object Pattern

### Implementation Details
- Each Active Object has its own thread and task queue
- Tasks are enqueued and processed asynchronously
- Thread-safe queue operations with condition variables
- Graceful shutdown and error handling

### Pipeline Flow
```
Client Request → Stage 1 → Stage 2 → Stage 3 → Stage 4 → Client Response
```

## Statistics Tracking

Use `stats` command to see:
- Total requests processed
- Pipeline errors encountered
- Tasks processed by each stage
- Current queue sizes for each stage

Example stats output:
```
=== Pipeline Statistics ===
Total Requests: 15
Pipeline Errors: 0
Stage 1 (Request): 15 processed, 0 queued
Stage 2 (Graph): 15 processed, 0 queued
Stage 3 (Algorithms): 15 processed, 1 queued
Stage 4 (Response): 14 processed, 0 queued
===========================
```

## Performance Notes

- All 4 algorithms execute in Stage 3 for each request
- Pipeline allows overlapping of different stages
- Active Objects enable true asynchronous processing
- Individual algorithm timing plus total pipeline time reported
- Suitable for high-throughput processing with complex per-request operations

## Files

- `server_main.cpp` - Main server program with signal handling
- `PipelineServer.h/cpp` - 4-stage pipeline implementation
- `ActiveObject.h/cpp` - Active Object pattern implementation
- `client.cpp` - Test client
- `Makefile` - Build system (reuses algorithms from Q7)

## Threading Model

### Pipeline vs Thread Pool
- **Thread Pool**: Workers compete for tasks from shared queue
- **Pipeline**: Tasks flow through dedicated stages in sequence
- **Active Objects**: Each stage has dedicated thread and queue

### Benefits
- Better resource utilization through pipelining
- Predictable processing flow
- Stage-specific optimizations possible
- Clear separation of concerns
- Excellent for streaming/continuous processing