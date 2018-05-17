#!/bin/bash
##################################
# Setup root ssh access in servers
##################################

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root on a CentOS machine."
   exit 1
fi

# Install ssh if not already installed
yum -y install openssh-server openssh-clients
chkconfig sshd on
service sshd start

# Enable ssh login as root
sed -i '/#PermitRootLogin yes/c\PermitRootLogin yes' /etc/ssh/sshd_config

# Open port ssh
firewall-cmd --zone=public --add-port=22/tcp --permanent
firewall-cmd --reload
iptables-save | grep 22

# Restart service
service sshd restart
