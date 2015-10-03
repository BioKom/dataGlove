#!/bin/bash
#
# Copyright (C) 2015 Betti Oesterholz
#
# Author: Betti Oesterholz
#
#######################################################################
#
# This Script creates a map for the mouse movments.
#
# History:
#  08.11.2014  Oesterholz  created


OUTPUT_FILE=mouseMoveStatesOut.txt

MODUS=20

REPEAT_DELAY="128"

#finger states
#thumb prepare
FINGER_1_S0=";  0; 499; 150"
#thumb execute (mouse move on)
FINGER_1_S1=";500;1500; 800"

#* center hoole of 200 width
#* move finger one step -> mouse move = mouse move * factor
#* grid 50 div; factor = 1.3
#* min=0; center=450; max=1000

FINGER_2[0]=";   0;  24;  12"
FINGER_2[1]=";  25;  49;  37"
FINGER_2[2]=";  50;  74;  62"
FINGER_2[3]=";  75;  99;  87"

FINGER_2[4]="; 100; 124; 112"
FINGER_2[5]="; 125; 149; 137"
FINGER_2[6]="; 150; 174; 162"
FINGER_2[7]="; 175; 199; 187"

FINGER_2[8]="; 200; 224; 212"
FINGER_2[9]="; 225; 249; 237"
FINGER_2[10]="; 250; 274; 262"
FINGER_2[11]="; 275; 299; 287"

FINGER_2[12]="; 300; 324; 312"
FINGER_2[13]="; 325; 349; 337"

FINGER_2[14]="; 350; 549; 450"

FINGER_2[15]="; 550; 574; 562"
FINGER_2[16]="; 575; 599; 587"

FINGER_2[17]="; 600; 624; 612"
FINGER_2[18]="; 625; 649; 637"
FINGER_2[19]="; 650; 674; 662"
FINGER_2[20]="; 675; 699; 687"

FINGER_2[21]="; 700; 724; 712"
FINGER_2[22]="; 725; 749; 737"
FINGER_2[23]="; 750; 774; 762"
FINGER_2[24]="; 775; 799; 787"

FINGER_2[25]="; 800; 824; 812"
FINGER_2[26]="; 825; 849; 837"
FINGER_2[27]="; 850; 874; 862"
FINGER_2[28]="; 875; 899; 887"

FINGER_2[29]="; 900; 924; 912"
FINGER_2[30]="; 925; 949; 937"
FINGER_2[31]="; 950; 974; 962"
FINGER_2[32]="; 975; 999; 987"



BASE_MOVE=1
MOVE_FACTOR=1.2


COUNT_FINGER_STATES=15
FINGER_INDEXES=      " 0  1  2  3  4  5  6  7  8  9 10 11 12 13  14  15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32"

#TODO
#TODO if have commen divisor -> reduce repeat delay
#TODO put in array MOUSE_MOVE_FOR[ INDEX ]
MOUSE_MOVE_FOR_INDEX="28 24 21 18 15 12 10  8  6  5  4  3  2  1   0   1  2  3  4  5  6  8 10 12 14 18 21 24 28 32 36 40 44"

FINGERE_2_CENTER=14
FINGERE_3_CENTER=14

# copy finger 2 states for finger 3
for Finger2Index in ${FINGER_INDEXES}; do
	FINGER_3[${Finger2Index}]=${FINGER_2[${Finger2Index}]}
done



evaluatePoints()
{
	if [ $3 -eq $4 ]
	then
		#center
		echo "0"
		return 0
	fi
	#TODO
	DISTANCE_TO_CENTER=0
	let "DISTANCE_TO_CENTER = $3 - $4"
	#abs( DISTANCE_TO_CENTER )
	if [ ${DISTANCE_TO_CENTER} -lt 0 ]
	then
	#TODO negative Zahl
		let "DISTANCE_TO_CENTER=1-${DISTANCE_TO_CENTER}"
		echo -`echo "(${2}^${DISTANCE_TO_CENTER}*$1) / 1" | bc`
		return 0
	fi
	
	let "DISTANCE_TO_CENTER=${DISTANCE_TO_CENTER}-1"
	echo `echo "(${2}^${DISTANCE_TO_CENTER} * $1) / 1" | bc`
	return 0
}


echo -n > ${OUTPUT_FILE}

for Finger2Index in ${FINGER_INDEXES}; do
	for Finger3Index in ${FINGER_INDEXES}; do
		#TODO
		MOVE_X=$(evaluatePoints ${BASE_MOVE} ${MOVE_FACTOR} ${Finger2Index} ${FINGERE_2_CENTER})
		MOVE_Y=$(evaluatePoints ${BASE_MOVE} ${MOVE_FACTOR} ${Finger3Index} ${FINGERE_3_CENTER})
		if [ ${MOVE_X} -ne 0 ] || [ ${MOVE_Y} -ne 0 ]
		then
			#write mous move map to file
			echo "$MODUS;mouse;MOUSE_MOVE ${MOVE_X} ${MOVE_Y};${REPEAT_DELAY};${FINGER_1_S1};${FINGER_2[${Finger2Index}]};${FINGER_3[${Finger3Index}]};;700;1000;800;;;;;;;;;; 8000; 21999; 19000;;;;;;;;;" >> ${OUTPUT_FILE}
		fi
	done
done










