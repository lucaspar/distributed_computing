# Step-by-step

### System Requirements

Three (2 servers + 1 client) or more machines forming an internal network with:
- CentOS 7.
- Root access;

### Enable remote access in machines

**Pick a "primary" machine**:
`a.` Manually run `openssh.sh`;
`b.` Check the remote access with `ssh`;
**If remote access works:**
`c.` Install Ansible (see `ansible.sh`);
`d.` Replicate the image to the other machines OR do steps `a` and `b` manually in every node.

### Update hosts

`a.` Get all IPs in local network (run `netw.sh`);
`b.` Update Ansible hosts in `/etc/ansible/hosts`:
> Being:
>   - `10.0.0.1` or `10.0.0.2` the primary machine;
>   - Any number of `gluster-clients`:
>   - 2 `gluster-servers`;

```
[gluster-servers]
10.0.0.1
10.0.0.2

[gluster-clients]
10.0.0.3
10.0.0.4
10.0.0.5
10.0.0.6
10.0.0.7
```

`c.` Update `playbook1.yml` with hosts' IPs.
`d.` Update `auth.allow` IP mask in `playbook3.5.yml` to match all `gluster-clients`.

### Deploy

Run all playbooks in order (1, 2, 3.0, 3.5, 4) stopping if anything goes wrong.

```bash
ansible-playbook playbook1.yml -u root
```
