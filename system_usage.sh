#!/bin/bash

#
# system_usage.sh
# (c) 2022 Jonas Gunz <himself@jonasgunz.de>
# License: All rights reserved.
#

# USAGE:
# ./system_usage.sh [TTY [BAUD]]

readonly TTY=${1:-/dev/ttyUSB0}
readonly BAUD=${2:-9600}

readonly NUMCPU="$(nproc --all)" #"$(grep -c ^processor /proc/cpuinfo)"

readonly PWM_CPU=0
readonly PWM_RAM=1
readonly PWM_TMP=2

echo "Trying to set $TTY to $BAUD"
stty -F "$TTY" "$BAUD" -echo || exit 1
echo OK!

function cpu_usage() {
	read -r LOAD CATCH < /proc/loadavg
	
	local FLOATLOAD
	FLOATLOAD=$(echo "($LOAD/$NUMCPU)*100" | bc -l)
	local CPU_PERCENT="${FLOATLOAD%%.*}"
	
	echo "$CPU_PERCENT"

	unset LOAD CATCH
}

function ram_usage() {
	local TOTAL=0
	local USED=0
	local AVAIL=0
	local PERCENT_USED PERCENT_USED_INT
	
	while read -r line; do
		ARR=( $line ) # Splitting is intended!
		[ "${ARR[0]}" != "Mem:" ] && continue
	
		TOTAL=${ARR[1]}
		AVAIL=${ARR[6]}
		USED=$((TOTAL-AVAIL))
	
		break
	done <<< "$(free | tail -n -2)"
	
	PERCENT_USED=$(echo "scale=4; $USED / $TOTAL * 100 " | bc -l)
	PERCENT_USED=${PERCENT_USED%00}
	PERCENT_USED_INT=${PERCENT_USED%.*}

	echo "$PERCENT_USED_INT"
}

function cpu_temp() {
	local DATA SUM VAL
	DATA=( $(sensors | \
		grep "^Core" | \
		sed -n "s|^Core\s[0-9]\+:\s\++\([0-9]\+\.[0-9]\+\).*$|\1|p" ) )

	SUM=0
	for (( i=0; i<${#DATA[@]}; i++ )); do
		VAL=${DATA[$i]%.*}
		SUM=$((SUM+VAL))
	done

	echo $((SUM/${#DATA[@]}))
}

# $1: PWM NUM
# $2: Dutycycle
function pwm_fade() {
	local DUTY HEX
	DUTY=$(echo "($2 * 255)/100" | bc)
	HEX=$(printf "%x" "$DUTY")
	echo "f$1$HEX" > "$TTY"
}

while true; do
	pwm_fade $PWM_CPU "$(cpu_usage)"
	pwm_fade $PWM_RAM "$(ram_usage)"
	pwm_fade $PWM_TMP "$(cpu_temp)"

	sleep 10
done
