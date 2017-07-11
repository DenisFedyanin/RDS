import sys, subprocess
subprocess.call("knapsack.exe %s %s" % (sys.argv[1],sys.argv[2]),shell=True)