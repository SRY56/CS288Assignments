#!/bin/bash
play_game() {
	#Number Quessing Game
	echo "Welcome to Bash Number Guessing Game!"
	echo "Think of number between 1 and 50"

	#Getting a Random Number
	target=$(( RANDOM % 50 + 1 ))
	attempts=7
	attempts2=1
	echo "you have $attempts attempts to guess it!"
	echo ""

	# loop for the game
	while [ $attempts -gt 0 ]
	do
		read -p "Enter your guess: " guess

		if  [[ "$guess" == *[!0-9]* ]]; then
		       echo "Invalid input! Please enter a number between 1 and 50"
	    elif [ "$guess" -lt 1 ] || [ "$guess" -gt 50 ]; then
			echo "Invalid input! Please Enter a number between 1 and 50"
		elif [ "$guess" -eq "$target" ]; then
			echo "congratulations! You  guessed the number $target in $attempts2 attempts!"
			return
		elif [ "$guess" -lt "$target" ]; then
		       echo "Too low! Try a higher number."
		else
 			echo "Too High! Try a lower number."		
		fi
	
		attempts=$((attempts-1))
		echo "Attempts remaining: $attempts"
		echo ""
		attempts2=$((attempts2+1))
	done

	echo "Game over! The number was $target. better luck next time."
}

while true
do
	play_game
	echo ""
	read -p "would you like to play again? (yes/no): " choice
	if [[ "$choice" != "yes" ]]; then
		echo "Thanks for playing!"
		break
	fi
done
