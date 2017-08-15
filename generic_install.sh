#!/bin/bash
make -C tools subdir-install-include
make -C tools subdir-install-libs
make -C tools subdir-install-libxc
make -C tools subdir-install-flask
make -C tools subdir-install-xenstore
make -C tools subdir-install-misc
make -C tools subdir-install-hotplug
make -C tools subdir-install-xentrace
make -C tools subdir-install-xenmon
make -C tools subdir-install-console
make -C tools subdir-install-xenstat
make -C tools subdir-install-firmware
