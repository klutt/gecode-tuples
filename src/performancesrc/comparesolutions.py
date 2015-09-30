#!/usr/bin/python

from subprocess import call
from sys import argv
import subprocess

def getOutput(command, arg):
    _command = './' + command + ' ' + 'dummy dummy' + ' '.join(arg[1:]) + ' | grep -E "^([0-9] ){10}"'
#    print _command
    data = subprocess.Popen(_command, shell=True, stdout=subprocess.PIPE)
    ret = data.stdout.read().split('\n')
    data.wait()
    return ret[:len(ret)-1]
    

def main(arg):
    intp = getOutput('int', arg)
    exactp = getOutput('exact', arg)
    approxp = getOutput('approx', arg)
    if len(arg) < 6:
        print "Error: compare solutions: wrong number of arguments"
        return 0
    print "Max solutions: " + str(max(len(intp), len(exactp), len(approxp)))
    if all(x in exactp for x in approxp) and all(x in intp for x in exactp):
        print "   Ok"
        return 1
    else:
        print ' '.join(argv[1:])
        return 0


if __name__ == "__main__":
    main(argv)
