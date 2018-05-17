##################################################
#               IGNORE THIS FILE                 #
# unless you had problems deploying with Ansible #
##################################################

# Based on:
# https://www.howtoforge.com/tutorial/high-availability-storage-with-glusterfs-on-centos-7/

## STEP 1 - update hosts files

#nano /etc/hosts

#127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
#192.168.0.100   server1.example.com     server1
#192.168.0.101   server2.example.com     server2
#192.168.0.102   client1.example.com     client1

#::1         localhost localhost.localdomain localhost6 localhost6.localdomain6

###################################################
## STEP 2 - server1.example.com/server2.example.com/client1.example.com

rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY*

yum -y install epel-release
yum -y install yum-priorities

nano /etc/yum.repos.d/epel.repo
# add "priority=10" in [epel section]
#sed -i '/priority/c\priority=10' /etc/yum.repos.d/epel.repo

yum -y update

###################################################
## STEP 3 - server1.example.com/server2.example.com

yum -y install centos-release-gluster

wget https://buildlogs.centos.org/centos/7/storage/x86_64/gluster-4.0/userspace-rcu-0.10.0-3.el7.x86_64.rpm
rpm -i userspace-rcu-0.10.0-3.el7.x86_64.rpm

yum install -y glusterfs-server

systemctl enable glusterd.service
systemctl start glusterd.service

glusterfsd --version

###################################################
## STEP 3.5 - server1.example.com

gluster peer probe server2.example.com
gluster peer status

gluster volume create testvol replica 2 transport tcp server1.example.com:/data server2.example.com:/data force
gluster volume start testvol
gluster volume info

# client1.example.com (= 192.168.1.102)
#gluster volume set testvol auth.allow 192.168.1.102
gluster volume set testvol auth.allow 192.168.*.*
gluster volume info

###################################################
## STEP 4 - client1.example.com

yum -y install glusterfs-client
mkdir /mnt/glusterfs

mount.glusterfs server1.example.com:/testvol /mnt/glusterfs
mount | grep -C 2 gluster
df -h | grep gluster

echo "/usr/sbin/mount.glusterfs server1.example.com:/testvol /mnt/glusterfs" >> /etc/rc.local

```