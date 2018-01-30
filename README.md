# Gas-Flow-Meter

The project is using fan and optical RPM meter to define rotation speed. The speed has been converted using externaly calibrated TSI 4040. It's currently capable to measure 50 - 250 lpm. For highest values more sophisticated amplifier or more sensitive photoresistor/diode to be introduced.

Files for 3D Printing at CAD section.

Gerbers and DipTrace for PCB at PCB section

Source code for AttinyX4A is at Firmware section

Device overview:
<img src="https://github.com/Misha91/Gas-Flow-Meter/blob/master/Pics/IMG_1112.jpg?raw=true" style="max-width:100%;">

PCB
<img src="https://github.com/Misha91/Gas-Flow-Meter/blob/master/Pics/IMG_1367.JPG?raw=true">

BoM for 1.3v:

uC	Attiny44A

U1, U4	AO3400

U2	HT7130-3

1400	NCP1400asn50t1g

OA	AD8628

L1	18uH, CR32
D1	M3

C1, C6, C8	-

C2, C3, C4, C5	100n

C7	50u

J7, J8	18p /20p

R1	10k

R2	75k

R3, R7	1k

R4	330

R5, R6	100k

R8, R11	51k

R9	100

R10	4,7k

