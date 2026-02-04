#!/bin/bash

# accepting atleast 3 filenames
if [ $# -lt 3 ]; then
	echo "Required to add atleast 3 file names"
	echo "Example: $0 file1.txt file2.txt file3.txt ..."
	exit 1
fi

# Timestamped backup directory
time=$(date +%y%m%d)
backup_dir="backup_${time}"
mkdir -p "$backup_dir" || { echo "Error: can't create $backup_dir"; exit 1; }

# create/empty log
log="$backup_dir/backup.log"
: > "$log"

# verify, copy, and rename each file
for f in "$@"; do
	if [ ! -f "$f" ]; then
		echo "Warning: '$f' doesn't exist. Skipping"
		echo "Skip: $f (not found)" >> "$log"
		continue
	fi

	name=$(basename "$f")
	# renames: example file1.txt --> file.txt_yyyymmdd
	new="${name}_${time}"
	cp -p -- "$f" "$backup_dir/$new"

	echo "Backup: $base -> $new" >> "$log"
done

# final message
echo ""
echo "Backup process if complete."
echo "Location of backup directory: $(pwd)/$backup_dir"
