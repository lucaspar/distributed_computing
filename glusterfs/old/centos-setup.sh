# Install dependencies
yum search centos-release-gluster
yum install -y centos-release-gluster40
yum install -y glusterfs gluster-cli glusterfs-libs glusterfs-server

# Create volumes
pvcreate /dev/vdb                       # create physical volume
vgcreate vg_gluster /dev/vdb            # create volume group
lvcreate -L 5G -n brick1 vg_gluster     # create 1st logical volume (brick #2)
lvcreate -L 5G -n brick2 vg_gluster     # create 2nd logical volume (brick #1)


# Start service
systemctl enable glusterd.service
systemctl start glusterd.service

# Configure firewall
firewall-cmd --zone=public --add-port=24007-24008/tcp --permanent
firewall-cmd --reload

# Connect the second GlusterFS node and create a trusted pool (storage cluster)
# Run on master:
#gluster peer probe gluster2.example.com

gluster peer status