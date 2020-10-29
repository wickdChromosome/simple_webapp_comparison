#!/bin/bash
sh ../perf.sh & node server.js
kill $(top -b -n 1 | grep ' sh' | cut -f3 -d' ')

