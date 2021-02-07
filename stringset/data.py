#!/usr/bin/env python3
import random

my_file = open("some.txt", "r")
liststr = my_file.readlines()
for i in range(0,len(liststr)):
    tempstr = random.randint(0,2147483648)
    liststr[i] =  liststr[i][0:len(liststr[i])-1] +\
        ' ' + str(tempstr)  + '\n'
    print(liststr[i])
my_file.close()
with open("data.txt", "w") as new_file:
    new_file.writelines(liststr)
