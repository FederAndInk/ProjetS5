#!/bin/sh
./arm_simulator --gdb-port 6666 --trace-registers --trace-memory --trace-state &
sleep 1
./build/src/ArmRun --host localhost --service 6666 ./Examples_loader/build/ex1
