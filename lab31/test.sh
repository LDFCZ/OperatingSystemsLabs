#!/bin/bash
gcc client.c -o c

./c 192.168.1.124 < test_input.txt & #1
./c 192.168.1.124 < test_input.txt & #2
./c 192.168.1.124 < test_input.txt & #3
./c 192.168.1.124 < test_input.txt & #4
./c 192.168.1.124 < test_input.txt & #5
./c 192.168.1.124 < test_input.txt & #6
./c 192.168.1.124 < test_input.txt & #7
./c 192.168.1.124 < test_input.txt & #8
./c 192.168.1.124 < test_input.txt & #9
./c 192.168.1.124 < test_input.txt & #10
./c 192.168.1.124 < test_input.txt   #11