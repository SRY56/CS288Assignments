#!/bin/bash

set -eou pipefail

# Navigation to directory
read -rp "Enter a directory path: " dir
if [[ ! -d "$dir" ]]; then
	echo "Error: '$dir' either does not exist or not a directory"
	exit 1
fi

cd "$dir" || { echo "Error: Failed to cd into '$dir'."; exit 1;}
echo "successfully navigated to $(pwd)"

#array of regular files in current directory
build_file_list(){
	files=()
	for f in *; do
		[[ -f "$f" ]] && files+=("$f")
	done
}

#render the menu
print_menu() {
	echo ""
	echo "Files in Directory:"
	local i=1
	for f in "${files[@]}"; do
		printf "%d) %s\n" "$i" "$f"
		((i++))
	done
	printf "%d) Exit\n" "$i"
}

#file content display
show_file () {
	local file="$1"
	local total
	total=$(wc -l < "$file")
	(( total < 0 )) && total=0
	
	if (( total == 0 )); then
		echo "=== File is empty ==="
		return
	fi
	
	local end="$total"
	while :; do
		#determine the 10 line window to show
		local start=$(( end - 9 ))
		(( start < 1 )) && start=1
		local count=$(( end - start + 1 ))
		total=$(( end - 9 ))
		echo ""
		echo "=== ${file}: lines ${start}-${end} of ${end} ==="
		
		#print specific range
		head -n "$end" "$file" | tail -n "$count" | nl -ba -w1 -s' ' | while read -r lineno line; do
			echo "[line ${total}]: ${line}"
			total=$(( total + 1))
		done

		if (( start == 1 )); then
			echo ""
			echo "You've reached the beginning of the file"
			break
		fi

		#ask user if they want to see more 
		while :; do
			echo ""
			read -rp "Would you like to see more? (yes/no): " answer
			case "${answer,,}" in
				yes)
					end=$(( start - 1 ))
					break
					;;
				no)
					return
					;;
				*)
					echo "Please answer yes or no"
					;;
			esac
		done
	done
}

#File selection menu and exit
while :; do
	build_file_list
	if (("${#files[@]}" == 0)); then
		echo "No reqular files found in $(pwd)."
		echo "Goodbye"
		exit 0
	fi
	
	print_menu
	echo
	read -rp "Select a file (enter number): " choice
	
	#validate input
	if [[ ! "$choice" =~ ^[0-9]+$ ]]; then
		echo "Invalid input: please enter a number."
		continue
	fi

	#calculate bounds
	exit_index=$(( ${#files[@]} + 1 ))

	if (( choice == exit_index )); then
		echo "Thank you for using the File Viewer. Goodbye!"
		exit 0
	elif (( choice >= 1 && choice <= ${#files[@]} )); then
		selected="${files[$((choice-1))]}"
		show_file "$selected"
	else
		echo "Invalid selection: choose a number between 1 and $exit_index"
	fi
done
