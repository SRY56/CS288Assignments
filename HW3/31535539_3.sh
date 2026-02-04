#!/bin/bash

# Accept arguments
if [[ $# -ne 1 ]]; then
        echo "Required to only add 1 text file"
        echo "Example: $0 file.txt"
        exit 1
fi
file=$1
if [[ ! -f "$file" ]]; then
        echo "Error: '$file' not found"
        exit 1
fi

# current date
cur_year=2025
today_md=$(date +%m%d)
today_md=$((10#$today_md))

# extract necessary files
while IFS= read -r line; do
	# extract fields and trime spaces
	name=$(echo "$line" | cut -d',' -f1 | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')
	dob=$(echo "$line" | cut -d',' -f2 | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')
	city=$(echo "$line" | cut -d',' -f3 | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')

	# keep cities with two or more words
	echo "$city" | grep -Eq '^[[:alpha:]-]+([[:space:]]+[[:alpha:]-]+)+$' || continue

	# extract YYYY-MM-DD
	y=$(printf '%s' "$dob" | cut -d- -f1)
	m=$(printf '%s' "$dob" | cut -d- -f2)
	d=$(printf '%s' "$dob" | cut -d- -f3)
	
	# compute age
	age=$(( cur_year - 10#$y ))
	birth_md=$(( 10#$m * 100 + 10#$d ))
	if [ $today_md -lt $birth_md ]; then
		age=$(( age - 1 ))
	fi

	printf '%s\t%s\n' "$age" "$name"
done < "$file" | sort -nr -k1,1 | while IFS=$'\t' read -r age name; do
		printf '%s, Age: %d\n' "$name" "$age"
	done
