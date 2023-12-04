# FreeRTOS Oven Simulator

## Overview
This project implements a microwave oven simulator using C++ and Arduino code, leveraging the FreeRTOS library for task management. The simulator includes components such as a heater, lamp, motor, and door, each managed by separate FreeRTOS tasks. The project provides a basic framework for simulating the interactions between these components.

## Features
* __Heater Simulation:__ Simulates a microwave heater with adjustable power levels (800W, 400W).

* __Lamp Simulation:__ Controls a lamp based on heating status and door position.

* __Motor Simulation:__ Simulates a turntable motor with adjustable rotation angles.

* __Door Simulation:__ Simulates the open/close status of the oven door.

* __Pre-Programmed Setups:__ Allows users to select and execute predefined heating setups.

## Components and Pins
* __Door Sensor Pin:__ 1

* __Heater Power Pin:__ 2

* __Oven Lamp Pin:__ 3

* __Oven Motor Pin:__ 4

Note: Adjust pin assignments according to your specific hardware setup.

## Tasks and Profiling
* __Heater Task:__ Manages the microwave heater's power levels and initiates concurrent tasks.

* __Lamp Task:__ Controls the lamp based on heating status and door position.

* __Motor Task:__ Simulates the rotation of the turntable motor at different angles.

* __Door Task:__ Simulates the open/close status of the oven door.

## Task Profiling
Task profiling is implemented for each component to monitor their execution.

## Setup and Operation
1. Upload the code to your Arduino board.
2. Open the Serial Monitor to observe the simulation output.
3. Follow on-screen prompts to select pre-programmed heating setups.


## Pre-Programmed Setups
1. __Defrosting Meat (5 minutes at 800W):__ Initiates heating at 800W for 5 minutes.
2. __Defrosting Vegetables (1 minute at 400W):__ Initiates heating at 400W for 1 minute.
3. __General Heating (30 seconds at 800W):__ Initiates heating at 800W for 30 seconds.

## Debugging
To enable or disable debug mode, modify the enable_debug variable in the code.

```
bool enable_debug = true;  // Set to true to enable debugging, false to disable
```