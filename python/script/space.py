from os import popen
print(popen("df -h | grep /dev/sda1").read())
