#!/usr/bin/python

from subprocess import call
from sys import argv
import subprocess

def getOutput(command):
    _command = './' + command + ' ' + ' '.join(argv[1:]) + ' | grep -E "^<"'
#    print _command
    data = subprocess.Popen(_command, shell=True, stdout=subprocess.PIPE)
    ret = data.stdout.read().split('\n')
    data.wait()
    return ret[:len(ret)-1]
    
# call(["./int"]+argv[1:]+[str(' | grep -E "^<"')])
#print bajs
intp = getOutput('int')
exactp = getOutput('exact')
approxp = getOutput('approx')

print "Max solutions: " + str(max(len(intp), len(exactp), len(approxp)))
if all(x in exactp for x in approxp) and all(x in intp for x in exactp):
    print "   Ok"
else:
    print ' '.join(argv[1:])
#call(["./approx"]+argv[1:])
#call(["./exact"]+argv[1:])
