# PortScanner
This C++ ScanPorts.cpp code does the following tasks
- `portScan` function creates a socket, connects to the target host and port, and prints whether the port is open or closed. 
- `hostScan` function resolves the target host's IP address and hostname, and then spawns threads to scan the specified ports using the `portScan` function. 
- `main` function parses the command-line arguments and calls the `hostScan` function with the provided host and ports.

