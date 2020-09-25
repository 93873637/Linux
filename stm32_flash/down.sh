#!/bin/bash
for a in {1..30}
do
    sudo ./build/stm32_flash -t 0 -f ./LitePile_Irda.bin -p /dev/ttyUSB0 -b 0
	sleep 120
	echo current count:$a
done
