import os
import sys
import time
import subprocess
from pynput.keyboard import Key, Listener
from threading import Thread

fn = sys.argv[1]

c_file = ""

def compile_and_run():
	os.system('clear')
	os.system('echo loading...')

	try:
		out = subprocess.check_output(['c++', '-o','run', fn])
	except:
		return

	os.system('clear')
	os.system('./run')


def loop():
	global c_file
	while True:
		n_file = open(fn, "r").read()

		if(n_file != c_file):
			compile_and_run()
			c_file = n_file

		time.sleep(1)

def key_listener():
	def on_release(key):
		if(key == Key.enter):
			compile_and_run()
		elif hasattr(key, 'char'):
			if key.char == 'r':
				compile_and_run()

	with Listener(on_release=on_release) as listener:
		listener.join()

t1 = Thread(target = loop)
t2 = Thread(target = key_listener)

if os.path.exists(fn):
	t1.start()
	t2.start()

