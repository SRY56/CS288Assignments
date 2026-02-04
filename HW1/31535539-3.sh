#!/bin/bash

#Directory input and validation
echo "Enter Directory:"
read  dir
if [[ ! "$dir" ]]; then
	echo "Error: $dir is not a directory"
	exit 0
fi

cd "$dir" || { echo "Can't cd into $dir"; exit 1; }

echo ""
echo "Analyzing directory: $(pwd)"
echo ""

#Arrays and variables
names=()
sizes=()
sum=0
count=0

#Calculating Arrays
while read -r line; do
	squeezed=$(echo "$line" | tr -s ' ')
	size=$(echo "$line" | cut -d' ' -f5)
	name=$(echo "$line" | cut -d' ' -f9)
		
	names+=("$name")
	sizes+=("$size")
	sum=$(( sum + size ))
	count=$((count+1))
done < <(ls -l | tail -n +2)

if (( count == 0 )); then
	echo "This directory has no items"
	exit 0
fi

avg=$(( sum / count ))

echo "All items with sizes"
for ((i=0; i<count; i++)); do
	echo "- ${names[i]}: ${sizes[i]} bytes"
done

echo ""
echo "Average size: ${avg} bytes"
echo ""

echo "Items larger then (${avg} bytes)"
above=0
idx=1
for ((i=0; i<count; i++)) do
	if (( sizes[i] > avg )); then
		echo "${idx}. ${names[i]}: ${sizes[i]} bytes"
		above=$(( above + 1 ))
		idx=$(( idx + 1 ))
	fi
done

echo ""
echo "Total items above average: ${above} out of ${count}"