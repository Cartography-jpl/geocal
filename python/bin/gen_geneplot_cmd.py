#!/usr/bin/env python
import sys
import os
f=open("all_bckgd_plots.txt")
cmdstr = "geneplot \"inp=("
inps=[]
for line in f:
   inps.append(line.strip())
f.close()
for inp in inps:
   cmdstr=cmdstr+inp
   if inps.index(inp) == len(inps)-1:
      cmdstr=cmdstr+")"
   else:
      cmdstr=cmdstr+","
out=sys.argv[1]
cmdstr=cmdstr+ " out="+out+"\""
print cmdstr
os.system(cmdstr)

