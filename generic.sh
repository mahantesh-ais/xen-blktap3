#!/bin/bash
### Build all the dependencies for blktap3
make -C tools subdir-all-include
make -C tools subdir-all-libs
make -C tools subdir-all-libxc
make -C tools subdir-all-flask
make -C tools subdir-all-xenstore
make -C tools subdir-all-misc
make -C tools subdir-all-hotplug
make -C tools subdir-all-xentrace
make -C tools subdir-all-xenmon
make -C tools subdir-all-console
make -C tools subdir-all-xenstat
make -C tools subdir-all-firmware
