#!/usr/bin/env python3
import os
import subprocess
import time

workerlist =[]

master = os.getpid()

questlist = ["And\n","For\n","My\n","To\n", "That\n"]

for i in range(0,5):
    if (master == os.getpid()):
        pid = os.fork();
        if(pid == 0):
            code = subprocess.Popen(["netcat", "127.0.0.1","4000"],
                            stdin=subprocess.PIPE).stdin
            code.write(bytes(questlist[i], 'ascii'))
            #code.write(bytes("\n",'ascii'))
            #time.sleep(1)
        else:
            workerlist.append(pid)
            #time.sleep(1)

if(master == os.getpid()):
    print(workerlist)
    for i in workerlist:
        os.waitpid(i,0)
