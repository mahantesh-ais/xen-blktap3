#!/bin/bash
### Build the components which depend on blktap3.
make -C tools subdir-all-libxl
make -C tools subdir-all-helpers
make -C xen
