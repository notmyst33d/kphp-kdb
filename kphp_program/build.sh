#!/usr/bin/bash
set -xe
../kphp2cpp -f ../KPHP/functions.txt -d . main.php
scons -j$(nproc --all)
