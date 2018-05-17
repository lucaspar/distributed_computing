#!/bin/bash
##############################
# Setup Ansible in one machine
##############################

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root on a CentOS machine."
   exit 1
fi

yum install ansible
subscription-manager repos --enable rhel-7-server-ansible-2.5-rpms

vim /etc/ansible/ansible.cfg
# change to:
# [defaults]
# host_key_checking = False
