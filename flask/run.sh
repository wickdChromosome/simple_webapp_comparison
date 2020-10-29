#!/bin/bash
sh ../perf.sh & gunicorn -b 161.35.122.119:80 server:app
kill $(top -b -n 1 | grep ' sh' | cut -f3 -d' ')


