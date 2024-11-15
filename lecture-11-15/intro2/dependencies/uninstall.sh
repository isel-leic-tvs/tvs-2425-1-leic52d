#!/bin/bash

SYSD_DIR=/etc/systemd/system

rm -rf $SYSD_DIR/a.service $SYSD_DIR/b.service  


 
systemctl daemon-reload

