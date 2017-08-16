#!/bin/bash
### BEGIN INIT INFO
# Provides:          xen-br
# Required-Start:    
# Required-Stop: 
# Default-Start:     2 3 5
# Default-Stop:
# Short-Description: Add bridge
# Description: Add xen-bridge and assign ip address.
### END INIT INFO


killall dhclient
/etc/init.d/network-manager stop 
brctl addbr xenbr0
brctl addif xenbr0 eth0
ifconfig eth0 inet 0.0.0.0 promisc up 
dhclient xenbr0

