# PortScanner
This C++ code for a Scannig the ports. 
The `portScan` function creates a socket, connects to the target host and port, and prints whether the port is open or closed. 
The `hostScan` function resolves the target host's IP address and hostname, and then spawns threads to scan the specified ports using the `portScan` function. 
The `main` function parses the command-line arguments and calls the `hostScan` function with the provided host and ports.

