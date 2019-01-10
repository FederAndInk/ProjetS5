#!/bin/sh
./arm_simulator --gdb-port 6666 --trace-registers --trace-memory --trace-state &
sleep 1
./build/src/ArmRunnerExample --debug src/ARM_runner_example.c --host localhost --service 6666 ./Examples_loader/build/ex1.o
