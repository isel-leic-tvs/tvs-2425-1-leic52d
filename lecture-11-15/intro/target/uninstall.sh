#!/bin/bash

SYSD_DIR=/etc/systemd/system

rm -rf $SYSD_DIR/a.service $SYSD_DIR/b.service $SYSD_DIR/test.target \
    $SYSD_DIR/stop.target

rm -rf $SYSD_DIR/test.target.wants 
 
systemctl daemon-reload

