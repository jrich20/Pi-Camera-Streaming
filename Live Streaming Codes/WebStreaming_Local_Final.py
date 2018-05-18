#!/usr/bin/python 

import paramiko 
import sys 
import os
import string 
import threading
import time
import select
from datetime import datetime
import webbrowser

outlock = threading.Lock()
def workon(host,command):
    print(host)
    ssh = paramiko.SSHClient() 
    print('client created ' + str(host))
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    print('set missing key policy ' + str(host))
    ssh.connect(host, username='pi', password='biomech1')
    print('connected ' + str(host))
    print ('sending file')
    print('sent')
    time.sleep(1) 
    print('slept')
    stdin, stdout, stderr = ssh.exec_command(command)
	
    with outlock: 
        print(stdout.readlines())
        
def main(): 
    hosts = []
    address = 217
    while address < 219:
        hosts.append('192.168.0.' + str(address))
        address += 1
        
    #hosts = ['192.168.0.218', '192.168.0.219', '192.168.0.217']
    
    threads = []
    command = 'python3 WebStreaming'
    #command = 'sudo restart now'
    #command = 'sudo shutdown now'
    for host in hosts: 
        thread = threading.Thread(target=workon, args=(host,command))
        thread.start() 
        threads.append(thread)
    for t in threads: 
        t.join
    time.sleep(5)
    webbrowser.open('file:///C:/Users/jamis/OneDrive/Desktop/Live%20Streaming%20Codes/WebStreaming_iFrames_Final.html')
    #webbrowser.open('http://192.168.0.220:8000/index.html')
    #webbrowser.open('http://192.168.0.218:8000/index.html')
   # webbrowser.open('http://192.168.0.219:8000/index.html')
    #webbrowser.open('http://192.168.0.217:8000/index.html')
    
main()

    
