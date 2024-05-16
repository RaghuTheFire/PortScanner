#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

void portScan(std::string targethost, int targetport) 
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) 
  {
    std::cerr << "[-] Failed to create socket" << std::endl;
    return;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(targetport);

  if (inet_pton(AF_INET, targethost.c_str(), & server_addr.sin_addr) <= 0) 
  {
    std::cerr << "[-] Invalid address" << std::endl;
    close(sock);
    return;
  }

  if (connect(sock, (sockaddr * ) & server_addr, sizeof(server_addr)) == 0) 
  {
    std::cout << "[+] " << targetport << " => Port Open" << std::endl;
  } 
  else 
  {
    std::cout << "[-] " << targetport << " => Port Closed" << std::endl;
  }

  close(sock);
}

void hostScan(std::string targethost,const std::vector < int > & targetports) 
{
  struct hostent * host_info = gethostbyname(targethost.c_str());
  if (host_info == nullptr) 
  {
    std::cerr << "[-] Host Not Found: " << targethost << std::endl;
    return;
  }

  char * ip_addr = inet_ntoa( * (struct in_addr * ) host_info -> h_addr_list[0]);
  std::cout << "***************" << std::string(strlen(ip_addr), '*') << std::endl;
  std::cout << "[+] IP Address: " << ip_addr << std::endl;

  struct hostent * host_name = gethostbyaddr(host_info -> h_addr_list[0], sizeof(struct in_addr), AF_INET);
  if (host_name != nullptr) 
  {
    std::cout << "[+] IP Path: " << host_name -> h_name << std::endl;
    std::cout << "*************" << std::string(strlen(host_name -> h_name), '*') << std::endl;
  } 
  else 
  {
    std::cout << "[-] Path Not Found" << std::endl;
    std::cout << "***************" << std::string(strlen(ip_addr), '*') << std::endl;
  }

  std::vector < std::thread > threads;
  for (int port: targetports) 
  {
    threads.emplace_back(portScan, ip_addr, port);
  }

  for (auto & t: threads) 
  {
    t.join();
  }
}

int main(int argc, char * argv[]) 
{
  if (argc < 3) 
  {
    std::cerr << "Usage: " << argv[0] << " -H <Host Address> -p <Port Address>" << std::endl;
    return 1;
  }

  std::string targetHost;
  std::vector < int > targetPorts;

  for (int i = 1; i < argc; i++) 
  {
    std::string arg = argv[i];
    if (arg == "-H") 
    {
      if (i + 1 < argc) 
      {
        targetHost = argv[++i];
      } 
      else 
      {
        std::cerr << "Missing host address" << std::endl;
        return 1;
      }
    } 
    else 
      if (arg == "-p") 
      {
      if (i + 1 < argc) 
      {
        std::string portStr = argv[++i];
        size_t start = 0, end;
        while ((end = portStr.find(',', start)) != std::string::npos) {
          targetPorts.push_back(std::stoi(portStr.substr(start, end - start)));
          start = end + 1;
        }
        targetPorts.push_back(std::stoi(portStr.substr(start)));
      } 
      else 
      {
        std::cerr << "Missing port address" << std::endl;
        return 1;
      }
    }
  }

  if (targetHost.empty() || targetPorts.empty()) 
  {
    std::cerr << "Usage: " << argv[0] << " -H <Host Address> -p <Port Address>" << std::endl;
    return 1;
  }

  hostScan(targetHost, targetPorts);

  return 0;
}
