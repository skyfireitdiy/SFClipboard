#!/bin/bash
echo "$1" | sudo -S "$2" &
/bin/rm -f $0
