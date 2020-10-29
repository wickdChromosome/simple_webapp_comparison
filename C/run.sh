#!/bin/bash
sh ../perf.sh & ./tmp/c_webtest -p 80 
kill $(top -b -n 1 | grep ' sh' | cut -f3 -d' ')
