#!/usr/bin/python

from subprocess import call
from sys import argv
import subprocess

def getOutput(command):
    _command = './' + command + ' ' + ' '.join(argv[1:])
#    print _command
    data = subprocess.Popen(_command, shell=True, stdout=subprocess.PIPE)
    ret = data.stdout.read().split('\n')
    data.wait()
    return ret[:len(ret)-1]
    
intp = getOutput('int')
exactp = getOutput('exact')
approxp = getOutput('approx')

#print "Max solutions: " + str(max(len(intp), len(exactp), len(approxp)))
#if all(x in exactp for x in approxp) and all(x in intp for x in exactp):
#    print "   Ok"
#else:
#    print ' '.join(argv[1:])

seeni = set()
uniqi = [x for x in intp if x not in seeni and not seeni.add(x)]
seene = set()
uniqe = [x for x in exactp if x not in seene and not seene.add(x)]
seena = set()
uniqa = [x for x in approxp if x not in seena and not seena.add(x)]

if(len(uniqi) != len(seeni)):
    print "int has duplicates " + str(len(uniqi)) + " vs " + str(len(seeni))
else:
    print "int has " + str(len(seeni)) + " entries"
if(len(uniqe) != len(seene)):
    print "exact has duplicates" + str(len(uniqe)) + " vs " + str(len(seene))
else:
    print "exact has " + str(len(seene)) + " entries"
if(len(uniqa) != len(seena)):
    print "approx has duplicates" + str(len(uniqa)) + " vs " + str(len(seena))
else:
    print "approx has " + str(len(seena)) + " entries"

#call(["./approx"]+argv[1:])
#call(["./exact"]+argv[1:])
