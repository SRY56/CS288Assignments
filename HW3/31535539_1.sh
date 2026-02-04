#!/bin/bash

# Argument and setup checks
if [[ $# -ne 1 ]]; then
	echo "Required to only add 1 text file"
	echo "Example: $0 file.txt"
	exit 1
fi

root="$1"
if [ ! -d "$root" ]; then
	echo "Error: '$root' is nor a directory"
	exit 1
fi

script_dir="$(cd -- "$(dirname -- "{BASH_SOURCE[0]}")" && pwd)"
output_file="$script_dir/unique_emails.txt"
tmp_file="$(mktemp)"
trap 'rm -f "$tmp_file"' EXIT

email_reg='^[A-Za-z][A-Za-z0-9._-]*@[A-Za-z0-9.-]+\.[A-Za-z]+$'

traverse() {
	dir="$1"
	for entry in "$dir"/*; do
		[ -e "$entry" ] || continue # skip if nothing matches (handles empty dirrectories)
		if [ -d "$entry" ]; then
			traverse "$entry"
		elif [ -f "$entry" ] && [[ "$entry" == *.txt ]]; then
			while IFS= read -r line; do 
				if printf '%s\n' "$line" | grep -qE "$email_reg"; then
					 printf '%s\n' "$line" >> "$tmp_file"
				fi
			done < "$entry"
		fi
		
	done
}

traverse "$root"

sort -u "$tmp_file" > "$output_file"

while IFS= read -r email; do
	echo "$email"
done < "$output_file"
