import os
import sys
import subprocess
import shutil
import time
import argparse
import re








# Path: build/devtest.py


proc = "./irc"




while True:
    subprocess.call([proc])
    color = "\033[1;32;40m"
    print(color + "Restarting in 3 second..." + "\033[0m")
    time.sleep(3)
