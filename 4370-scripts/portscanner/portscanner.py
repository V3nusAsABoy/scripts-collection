import socket
from scapy.all import *

port = input("Enter port: ") # prompts user to enter port
ip = input("Enter ip address: ") # prompts user to enter ip address

try:

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # defines socket
    sock.settimeout(1)  # sets timeout for connection attempt
    
    result = sock.connect_ex((ip, int(port))) # connection attempt

    if result == 0:
        print(f"Port {port} on {ip}: OPEN")
            
    else:
        print(f"Port {port} on {ip}: CLOSED")
    
except Exception as e:
    print(f"Error checking port {port}: {e}")
    
finally:
    sock.close() # closes socket


try:
    
    syn_packet = IP(dst=ip)/TCP(dport=int(port), flags='S') # crafts SYN packet
    response = sr1(syn_packet, timeout=1, verbose=0) # sends packet and awaits response

    if response and response.haslayer(TCP) and response.getlayer(TCP).flags == 0x12: # determines if there is SYN-ACK response
        ttl = response.ttl
        # determines probably OS based on ttl
        if 120 <= ttl <= 128:
            print("Probable OS: Windows")
        elif 50 <= ttl <= 64:
            print("Probable OS: Linux/macOS/Unix-like")
        elif ttl == 255:
            print("Probable OS: Network Device (e.g., Router)")
        else:
            print("OS cannot be determined with high confidence.")
    else:
        print(f"No SYN-ACK response from {ip}")
except Exception as e:
    print(f"Error during fingerprinting: {e}")

        
