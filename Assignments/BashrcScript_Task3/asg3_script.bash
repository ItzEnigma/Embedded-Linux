#!/bin/bash

echo "Running Script ...\n"

cd ~/

if [ -a .bashrc ]
 then
    echo "HELLO="$HOSTNAME"" >> .bashrc
    echo "LOCAL=$(whoami)">>.bashrc
    gnome-terminal & disown
else
    echo "Not Found!\n"
fi
