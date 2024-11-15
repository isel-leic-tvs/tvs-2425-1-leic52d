#!/bin/bash

SYSD_DIR=/etc/systemd/system

systemctl stop a.service
rm -rf $SYSD_DIR/a.service  

systemctl daemon-reload

