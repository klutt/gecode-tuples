#!/usr/bin/python

from subprocess import call
from sys import argv

call(["./int"]+argv[1:])
call(["./approx"]+argv[1:])
call(["./exact"]+argv[1:])
