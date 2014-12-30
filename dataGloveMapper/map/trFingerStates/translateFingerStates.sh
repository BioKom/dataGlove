#!/bin/bash
#
# Copyright (C) 2014 Betti Oesterholz
#
# Author: Betti Oesterholz
#
#######################################################################
#
# This Script converts the given finger states int a data glove map.
#
# History:
#  08.11.2014  Oesterholz  created


INPUT_FILE=fingerStates.txt
OUTPUT_FILE=fingerStatesOut.txt

MODUS=10

TEXT_PREP_FUN="${MODUS};prepareKey;"
TEXT_KEY_FUN="${MODUS};key;"

REPEAT_DELAY="100"

#finger states
#thumb prepare
FINGER_1_S0=";  0; 499; 150"
#thumb execute
FINGER_1_S1=";500;1500; 800"

FINGER_2_S0=";750;1000; 850"
FINGER_2_S1=";400; 749; 550"
FINGER_2_S2=";  0; 399; 400"

FINGER_3_S0=";600; 800; 650"
FINGER_3_S1=";400; 599; 500"
FINGER_3_S2=";100; 399; 250"

FINGER_4_S0=";650; 900; 850"
FINGER_4_S1=";400; 649; 550"
FINGER_4_S2=";  0; 399; 200"

#pinky
FINGER_5_S0=";700; 900; 770"
FINGER_5_S1=";500; 699; 600"
FINGER_5_S2=";  0; 499; 250"


# Quaterion 2
QUATERION_1_S0=";;;;; 8000; 21999; 19000;;;;;;;;;"
QUATERION_1_S1=";;;;;0    ;  7999;  1000;;;;;;;;;"
QUATERION_1_S2=";;;;;22000; 31000; 25000;;;;;;;;;"


echo "Konverting file: ${INPUT_FILE}"
sed -e "s&\([0-9]\);\([0-9]\);\([0-9]\);\([0-9]\);\([^;]*\)-> *\([^;]*\) */ *\([^;]*\) */ *\([^;]*\) *&${TEXT_PREP_FUN}\6;${REPEAT_DELAY};0;\1;\2;\3;\4;0;\n${TEXT_KEY_FUN}\6;${REPEAT_DELAY};1;\1;\2;\3;\4;0;\n${TEXT_PREP_FUN}\7;${REPEAT_DELAY};0;\1;\2;\3;\4;1;\n${TEXT_KEY_FUN}\7;${REPEAT_DELAY};1;\1;\2;\3;\4;1;\n${TEXT_PREP_FUN}\8;${REPEAT_DELAY};0;\1;\2;\3;\4;2;\n${TEXT_KEY_FUN}\8;${REPEAT_DELAY};1;\1;\2;\3;\4;2;&"  ${INPUT_FILE} | \
sed -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);0;&\1;\2;\3;\4;\5;\6;\7;\8;\9;${QUATERION_1_S0}&" \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);1;&\1;\2;\3;\4;\5;\6;\7;\8;\9;${QUATERION_1_S1}&"  \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);2;&\1;\2;\3;\4;\5;\6;\7;\8;\9;${QUATERION_1_S2}&" | \
sed -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);0;\(.*\)&\1;\2;\3;\4;\5;\6;\7;\8;${FINGER_5_S0};\9&" \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);1;\(.*\)&\1;\2;\3;\4;\5;\6;\7;\8;${FINGER_5_S1};\9&"  \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);2;\(.*\)&\1;\2;\3;\4;\5;\6;\7;\8;${FINGER_5_S2};\9&" | \
sed -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);0;\(.*\)&\1;\2;\3;\4;\5;\6;\7;${FINGER_4_S0};\8&" \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);1;\(.*\)&\1;\2;\3;\4;\5;\6;\7;${FINGER_4_S1};\8&"  \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);2;\(.*\)&\1;\2;\3;\4;\5;\6;\7;${FINGER_4_S2};\8&" | \
sed -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);0;\(.*\)&\1;\2;\3;\4;\5;\6;${FINGER_3_S0};\7&" \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);1;\(.*\)&\1;\2;\3;\4;\5;\6;${FINGER_3_S1};\7&"  \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);2;\(.*\)&\1;\2;\3;\4;\5;\6;${FINGER_3_S2};\7&" | \
sed -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);0;\(.*\)&\1;\2;\3;\4;\5;${FINGER_2_S0};\6&" \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);1;\(.*\)&\1;\2;\3;\4;\5;${FINGER_2_S1};\6&"  \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);2;\(.*\)&\1;\2;\3;\4;\5;${FINGER_2_S2};\6&" | \
sed -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);0;\(.*\)&\1;\2;\3;\4;${FINGER_1_S0};\5&" \
    -e "s&\([^;]*\);\([^;]*\);\([^;]*\);\([^;]*\);1;\(.*\)&\1;\2;\3;\4;${FINGER_1_S1};\5&" | \
sed -e 's/ //g' > ${OUTPUT_FILE}

#replace fingers begin at back

#sed -e "s/\([0-9]\);\([0-9]\);\([0-9]\);\([0-9]\);/\1;\2;\3;\4\n/" -e "s/\"o/\\&ouml;/g"  ${INPUT_FILE} > ${OUTPUT_FILE}
#sed -e "s/\([0-9]\);\([0-9]\);\([0-9]\);\([0-9]\);\([^;]*\)->\([^;]*\)/\([^;]*\)/\([^;]*\)/\6\1;\2;\3;\4\n\7\1;\2;\3;\4\n\8\1;\2;\3;\4/" -e "s/\"o/\\&ouml;/g"  ${INPUT_FILE} > ${OUTPUT_FILE}











