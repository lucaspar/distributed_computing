FROM centos:latest

# Install dependencies
RUN yum search centos-release-gluster
RUN yum install -y centos-release-gluster40
RUN yum install -y glusterfs gluster-cli glusterfs-libs glusterfs-server

# Create volumes
CMD pvcreate /dev/vdb                       # create physical volume
CMD vgcreate vg_gluster /dev/vdb            # create volume group
CMD lvcreate -L 5G -n brick1 vg_gluster     # create 1st logical volume (brick #2)
CMD lvcreate -L 5G -n brick2 vg_gluster     # create 2nd logical volume (brick #1)

# Start service
CMD systemctl enable glusterd.service
CMD systemctl start glusterd.service

# Configure firewall
CMD firewall-cmd --zone=public --add-port=24007-24008/tcp --permanent
CMD firewall-cmd --reload

# Connect the second GlusterFS node and create a trusted pool (storage cluster)
# Run on master:
#gluster peer probe gluster2.example.com

CMD gluster peer status