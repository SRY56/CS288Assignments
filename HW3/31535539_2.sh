#!/bin/bash
# check argument
if [[ $# != 1 ]]; then
	echo "Required to only add 1 text file"
	echo "Example: $0 file.txt"
	exit 1
fi
file=$1
if [[ ! -f "$file" ]]; then
	echo "Error: '$file' not found"
	exit 1
fi
# Regex: MM/DD/YYYY or MM-DD-YYYY or MM.DD.YYYY
date_re='((0[1-9]|1[0-2])[\/.-](0[1-9]|[12][0-9]|3[01])[\/.-]((19|20)[0-9]{2}))'

pairs=() # holds elements line: YYYYMMDD

# read file line by line and extract all date substrings
while IFS= read -r line || [[ -n "$line" ]]; do
	line=${line%$'\r'}


	while IFS= read -r d; do
		d=${d%$'\r'}
		
		# detect delimiters
		if [[ "$d" == */* ]]; then 
			delim="/"
		elif [[ "$d" == *-* ]]; then
			delim="-"
		else
			delim="."
		fi
		# split the components
		month=$(cut -d "$delim" -f1 <<<"$d")
		day=$(cut -d "$delim" -f1 <<<"$d")
		year=$(cut -d "$delim" -f3 <<<"$d")

		key="${year}${month}${day}"
		pairs+=( "${key}|${d}" )
	done < <(grep -oE "$date_re" <<<"$line")
done < "$file"


printf '%s\n' "${pairs[@]}" | LC_ALL=C sort | cut -d'|' -f2
