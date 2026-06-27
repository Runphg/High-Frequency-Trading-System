# High-Frequency-Trading-System
A high-performance parallel processing pipeline for real-time tick data using Kafka, C++/OpenMP, and MySQL.

## 1. Project Overview
This project implements an end-to-end parallel and distributed software system for **Event-Driven Financial Simulation**. It evaluates the real-time Value at Risk (VaR) of a portfolio using Monte Carlo simulations. The system integrates Apache Kafka for distributed message streaming and OpenMP for parallel computation, satisfying the requirements for high-performance computing.

## 2. Prerequisites
* **OS:** macOS (Apple Silicon M4) or Linux
* **Dependencies:**
  * Docker Desktop & Docker Compose
  * Homebrew (for macOS)
  * C++17 Compiler (AppleClang or GCC)
  * `cmake`, `librdkafka`, `mysql-client`, `libomp`

## 3. Build Instructions (빌드 방법) 
1. Install required C++ libraries via Homebrew:
   ```bash
   brew install cmake librdkafka mysql-client libomp
2. Build the C++ Worker Engine using CMake:
   ```bash
    mkdir build && cd build
    cmake ..
    make

## 4. Execution Instructions (실행 방법)
1. Start the Distributed Infrastructure:
    Boot up Kafka and MySQL using Docker Compose in the project root directory.
   ```bash
   docker compose up -d

2. Start the C++ Worker Engine:
    Run the compiled worker in the build directory.
    ```bash
   ./worker

3. Start the Python Tick Producer (Market Simulator):
    In a new terminal window, start generating real-time market data.
    ```bash
   python3 scripts/producer.py

## 5. Experimental Procedures (성능 평가 실험 방법)
To reproduce the quantitative Thread Scaling experiments, the worker executable accepts three arguments: [Thread Count], [Batch Size], and [Iterations].  
Run the following commands sequentially to observe performance scaling:
    ```bash
    ./worker 1 5000 20
    ./worker 2 5000 20
    ./worker 4 5000 20
    ./worker 8 5000 20
    ./worker 10 5000 20
    ./worker 16 5000 20
    
The average processing time (ms) per batch will be logged in the terminal and stored in the MySQL tick_summary table for further analysis.