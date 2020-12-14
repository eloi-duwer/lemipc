#!/bin/sh

if [ "$#" -ne 3 ]; then
	echo "Usage: $0 <board_size> <nb_teams> <nb_players_per_team>"
	exit 1
fi

nb_players=$(($2*$3))

path=$(dirname "$0")

echo "Starting host program..."
"$path/lemipc" "$1" "$2" "$3" &

#Sleep is used to slow the program down, or else
#All lemipc instances are launched at the same time,
#And the host doesn't have the time to instantiate the ressources
#For the other instances, creating conflicts between the programs
sleep 0.01s
i="1"
while [ "$i" -ne "$nb_players" ]
do
	echo "Starting new player program, team = $(($i/$3+1))"
	"$path/lemipc" "$(($i/$3+1))" &
	sleep 0.01s
	i=$(($i+1))
done
