#!/usr/bin/env python
import os
import sys

cmdstr = 'geneplot "inp=('
with open("all_bckgd_plots.txt") as f:
    inps = [line.strip() for line in f]
for inp in inps:
    cmdstr = cmdstr + inp
    if inps.index(inp) == len(inps) - 1:
        cmdstr = cmdstr + ")"
    else:
        cmdstr = cmdstr + ","
out = sys.argv[1]
cmdstr = cmdstr + " out=" + out + '"'
print(cmdstr)
os.system(cmdstr)
