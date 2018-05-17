#!/bin/bash
##############################
# Setup Ansible in one machine
##############################

sudo yum install ansible
sudo subscription-manager repos --enable rhel-7-server-ansible-2.5-rpms

vim /etc/ansible/ansible.cfg
# change to:
# [defaults]
# host_key_checking = False