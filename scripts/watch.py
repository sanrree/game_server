import os
import sys
import time
import subprocess
from threading import Thread

fn = sys.argv[1]

c_file = ""


def compile_and_run():
    os.system('clear')
    os.system('echo loading...')

    try:
        command = ['c++', '-std=c++11', '-o', 'run', fn, '-lpthread']
        process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
        process.wait()
        print process.returncode
    except:
        return

    os.system('clear')
    os.system('./run')


def loop():
    global c_file
    while True:
        n_file = open(fn, "r").read()

        if (n_file != c_file):
            compile_and_run()
            c_file = n_file

        time.sleep(1)


def key_listener():
    key = ""
    while True:
        key = raw_input()
        if (key == 'r'):
            compile_and_run()
        #os.system('clear')


t1 = Thread(target=loop)
t2 = Thread(target=key_listener)

if os.path.exists(fn):
    t1.start()
    t2.start()
