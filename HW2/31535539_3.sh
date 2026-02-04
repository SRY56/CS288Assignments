#!/bin/bash

# validate argumant for directory
if [ $# -ne 1 ]; then
	echo "Make sure you only give one directory"
	echo "Example: $0 /home/user/testdir"
	exit 1
fi
root=$1
if [ ! -d "$root" ]; then
	echo "Error: $root is not a directory."
	exit 1
fi
root=$(cd "$root" && pwd)
echo "Traversing Directory: $root"

# base names and their first path using parallel arrays
baseName=()
paths=()

total=0
dups=0
deleted=0

# return index of basename in names[]
indexName() {
	local target="$1" i
	for i in "${!baseName[@]}"; do
		if [ "${baseName[$i]}" = "$target" ]; then
			echo "$i"
			return 0
		fi
	done
	echo "-1"
}

traverse() {
	local dir="$1" path base idx orig dup ans

	# include hidden files; skip '.' and '..'
	for path in "$dir"/* "$dir"/.*; do
		[ -e "$path" ] || continue
		base=$(basename -- "$path")
		[ "$base" = '.' ] || [ "$base" = ".." ] && continue

		if [ -d "$path" ]; then
			traverse "$path"
		elif [ -f "$path" ]; then
			total=$((total+1))
			idx=$(indexName "$base")
			if [ "$idx" -eq -1 ]; then
				baseName+=("$base")
				paths+=("$path")
			else
				dups=$((dups+1))
				orig="${paths[$idx]}"
				dup="$path"

				echo ""
				echo "Found duplicate filename: $base"
				echo "Original file: $orig"
				echo "Duplicate file: $dup"
				while :; do
					read -r -p "Do you want to remove $dup? (yes/no): " ans
					case "$ans" in
						yes)
							if rm -- "$dup"; then
								echo "File deleted: $dup"
								deleted=$((deleted+1))
							else
								echo "Failed to delete: $dup"
							fi
							break ;;
						no)
							echo "File kept: $dup"; 
							break;;
						*)
							echo "Please answer yes or no." ;;
					esac
				done
			fi
		fi
	done
}

traverse "$root"

echo ""
echo "Total files processed: $total"
echo "Duplicates files found: $dups"
echo "Files deleted: $deleted"
