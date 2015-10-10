#!/usr/bin/python

from subprocess import call
import comparesolutions

def main():
    for i in range(1,10):
        ret = comparesolutions.main(['./comparesolutions', str(i), '10', '3', '3', '5'])
        if ret==0:
            raise Exception('Compare solutions failed')
    return 1

if __name__ == "__main__":
    main()
