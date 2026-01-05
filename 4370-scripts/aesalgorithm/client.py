import aesencryption as aes # encryption algorithm
import sys # retrieves cmd line arguments
import socket as sock # for socket connections 
import os # to separate name from extension

host = 'localhost'
port = 51390
fname = sys.argv[1] # name of file user has put in command line argument
fnameroot, fnameextension = os.path.splitext(fname) # separating name from extension. file name stored in fname and extension in fnameextension
# if the file is called 'message.txt', fnameroot = 'message' and fnameextension = '.txt'

with sock.socket(sock.AF_INET, sock.SOCK_STREAM) as s:
    s.connect((host, port))# connects to server
    print(f"Client connected to server on {host}:{port}")

    encryptedfname = f"{fnameroot}_encrypted.aes" # name of encrypted file
    aes.encrypt_file(fname, encryptedfname, aes.key) # encrypts file and stores encrypted file in encryptedfname
    

    fileToSend = encryptedfname.encode() # encodes encryptedfname to send to server

    s.sendall(fileToSend) # sends name of encryptedfname to server

    with open(encryptedfname, 'rb') as f: # reads encryptedfname and sends to server
        while True:
            data = f.read(1024)
            if not data:
                break
            s.sendall(data)
    print("Encrypted file sent to server.")
    s.close()
