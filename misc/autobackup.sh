#!/bin/bash

# The directory to backup
PDataDir="/home/pi/photomaton2/saved_images";

# logging file
logfile=$PDataDir"/backup.log"

# To setup this script, get the UUID of your device with:
# ls -l /dev/disk/by-uuid/

# Use the UUID to add in /etc/fstab: (fat32)
# UUID=<UUID> /home/pi/mount-backup1 vfat user,noauto 0 0

# This script is a backup daemon for the photobooth
backup_paths="\
/home/pi/mount-backup1 \
/home/pi/mount-backup2"

write_log()
{
	mkdir -p $PDataDir
	echo "$1" >> $logfile;
}

run_backup()
{
	if [[ 1 != $# ]];
	then
		write_log "Error: Backup failed";
		return -1;
	fi
	dirname=$(date +%Y%m%d%H%M%S);
	mkdir $1/$dirname;
	if [[ 0 != $? ]];
	then
		write_log "Error: Cannot create backup directory";
		return -1;
	fi
	write_log "Backup started";
	cp -r $PDataDir/* $1/$dirname;
	if [[ 0 != $? ]];
	then
		write_log "Error: copy failed";
		return -1;
	else
		write_log "Backup success";
	fi
	return 0;
}

for bpath in $backup_paths;
do
	# bpath contains the backup path, try to mount it
	mount $bpath 2>/dev/null;
	if [[ 0 == $? ]];
	then
		# Mounted, backup the files
		run_backup $bpath;
		# Unmount and write_log
		umount $bpath;
		write_log "Device can now safely be unplugged";
	fi
done
