#!/bin/bash
#################################################
# Adds all computers in network and to hosts file
#################################################

# network mapping - list all open port 22 that match the IP range
map() {
    nmap -p 22 192.168.0.104/32 -oG -
}

# match only "open" ports
format() {
    awk '/open/{print "client" i++ " " $2}'
}

# test command
test() {
    xargs -i echo './hosts.sh add {}'
}

# add them to /etc/hosts
run() {
    xargs -l bash -c './hosts.sh add $0 $1'
}

# map | format | test
map | format | run;

less hosts
