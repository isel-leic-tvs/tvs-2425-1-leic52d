#!/bin/bash


if [ $UID != 0 ] ; then
	echo "must be called as superuser"
	exit 1
fi

ECHOD=./echod
ECHOD_SRC=./echo_srv.c

if [ ! -x $ECHOD ]; then
	if [ -f $ECHOD_SRC ]; then
		echo make before install!
	else
		echo wrong dir!
	fi
	exit 1
fi

ECHOD_DIR=/opt/echod
SYSTD_DIR=/etc/systemd/system


if [ ! -d $ECHOD_DIR ]; then
	if ! mkdir -p $ECHOD_DIR ; then
		echo Failed to create $ECHOD_DIR
		exit
	fi
fi	
chown isel $ECHOD_DIR
chgrp isel $ECHOD_DIR


cp $ECHOD $ECHOD_DIR

echo cp $ECHOD.service $SYSTD_DIR 
cp $ECHOD.service $SYSTD_DIR 

#force systemd update state
systemctl daemon-reload

echo echod install done!

