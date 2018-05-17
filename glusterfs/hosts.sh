#!/bin/bash
#################################################
# Dynamically add or remove hosts from $hostsFile
#################################################

# if [ $# -ne 3 ]; then
#    echo -e "\n\tUSAGE:\t$hostsFile.sh <add|remove> <HOSTNAME> <IP>\n"
#    exit 1
# fi

# Path to custom hosts file
hostsFile="./hosts"

# Hostname to add/remove.
hostname=$2

# IP address for host
ip=$3

echo "HOSTNAME=$2 IP=$3"

yell() { echo "$0: $*" >&2; }
die() { yell "$*"; exit 111; }
try() { "$@" || die "cannot $*"; }

remove() {
    if [ -n "$(grep -P "[[:space:]]$hostname" $hostsFile)" ]; then
        echo "$hostname found in $hostsFile. Removing now...";
        sed -ie "/[[:space:]]$hostname/d" "$hostsFile";
    else
        yell "$hostname was not found in $hostsFile";
    fi
}

add() {
    if [ -n "$(grep -P "[[:space:]]$hostname" $hostsFile)" ]; then
        yell "$hostname already exists.";
    else
        # echo "Adding $ip to $hostsFile...";
        try printf "%s\t%s\n" "$ip" "$hostname" | tee -a "$hostsFile" &> /dev/null;

        if [ -n "$(grep $hostname $hostsFile)" ]; then
            echo "$hostname ($ip) was added succesfully.";
        else
            die "Failed to add $hostname ($ip)";
        fi
    fi
}

$@