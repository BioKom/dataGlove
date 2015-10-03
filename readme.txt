=Data glove=

This project is for handling a data glove.

Works for: DGTech VHand 3.0
Tested with: DGTech VHand 3.0 on SuSu Linux

It contains some modules:
* the data glove communication layer: for the communication with the data glove
* the data glove manager: for handling a data glove


==Compile==

> make test
or
> make test > out_make.txt 2>&1; tail -n8 out_make.txt


==Run==

* run as Superuser (else the application can no connect to the data glove device)

> su
> ionice -c1 -n1 nice -n-20 bin/test_startDataGlove

or
> su
> nice -n-20 bin/test_startDataGlove



==Data glove communication layer==

folder: dataGloveDGTechVHandCommunication

Note:
* root permissions requred

This module is for the bottom layer communication with the DGTech VHand (3.0) data glove.
It handles the messages from and to the data glove.


==Data glove manager==

folder: dataGloveManager


==Data glove mapper==

folder: dataGloveMapper

This program maps data glove input (gestures) into functionality (like type
a key or mouse move left).


===Correction of data glove values===

For some values (e.g. Quaterion) the message value to hand position
mapping changes (e.g. no tilt is one time 0 and the other 2000 ).
The interval corrections are for these cases.
The correction objects can have a lower and a upper border.
If a value is lower / higher than the lower / upper border of the
correction object the correction value will be adapted, so that the
value is the lower / upper border.
Every given value will be corrected with the correction value.

correction of data glove values examples:
* "FINGER_1 MIN 0" -> if lower 0 -> all FINGER_1 values + delta so that values are bigger or equal 0
* "FINGER_1 MAX 1000" -> if higher 1000 -> all FINGER_1 values + delta so that values are lower or equal 1000






