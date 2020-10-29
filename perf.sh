#!/bin/bash
rm top.log
while true ; do top -b -n 1 >> top.log && sleep 1; done
