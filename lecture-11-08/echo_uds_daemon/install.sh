#!/bin/bash

ECHOD_EXE=echod
ECHOD_SRC=./echo_srv.c

if [ ! -x ./$ECHOD_EXE ]; then
	if [ ! -f ./$ECHOD_SRC ]; then
		echo wrong directory!
	else
		echo make before install!
	fi
	exit
fi

ECHOD_DIR=/opt/echod
ECHOD_PATH=$ECHOD_DIR/$ECHOD_EXE

mkdir -p $ECHOD_DIR

if [ ! -d $ECHOD_DIR ]; then
	echo Failed to create $ECHOD_DIR
	exit
fi	

cp $ECHOD_EXE $ECHOD_DIR

#run server
$ECHOD_PATH
sleep 3
chown isel $ECHOD_DIR
chgrp isel $ECHOD_DIR
chown isel $ECHOD_DIR/*
chgrp isel $ECHOD_DIR/*


echo "echod install done!"

