#!/bin/bash

# check if no arguments were given
if [ $# -eq 0 ]; then
	echo "Required to add atleas 1 number"
	echo "Example: $0 num1 num2 num3 ..."
	exit 1
fi

# store arguments in array
arr=("$@")
n=${#arr[@]}

# print original array
echo "Original array: ${arr[@]}"

# bubble sort impelementation
for ((i=0; i<n-1; i++)); do
	for ((j=0; j<n-i-1; j++)); do
		if [ ${arr[j]} -gt ${arr[$((j+1))]} ]; then
			# swap
			temp=${arr[j]}
			arr[$j]=${arr[$((j+1))]}
			arr[$((j+1))]=$temp
		fi
	done
done

# print sorted array
echo "Sorted array: ${arr[@]}"
