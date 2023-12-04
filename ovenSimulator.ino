/**
 * @file ovenFreeRTOS.ino
 * @author Julie Anne (julie.cantillep@studerande.movant.se)
 * @brief This program demonstrates a basic framework for simulating
 * a microwave oven with multiple components like the heater, lamp, motor and door.
 * Each component is managed by the FreeRTOS library.
 * @version 0.1
 * @date 2023-12-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// Import FreeRTOS and Task
#include <FreeRTOS.h>
#include <task.h>

// An example of all the components connected to our circuit
// Note: Change the pins according to your setup
#define DOOR_SENSOR_PIN 1
#define HEATER_POWER 2
#define OVENLAMP 3
#define OVENMOTOR 4

typedef uint32_t TaskProfiler;
TaskProfiler heaterProfiler;
TaskProfiler lampProfiler;
TaskProfiler motorProfiler;
TaskProfiler doorProfiler;

bool enable_debug = true;

// Define tasks for each component
TaskHandle_t xTaskHeater, xTaskLamp, xTaskMotor, xTaskDoor;

// Create our Tasks
/*
BaseType_t xTaskCreate( TaskFunction_t pvTaskCode,
                        const char * const pcName,
                        configSTACK_DEPTH_TYPE usStackDepth,
                        void *pvParameters,
                        UBaseType_t uxPriority,
                        TaskHandle_t *pxCreatedTask
);*/

// Global variables
int heaterPower = 0;  // Current power of the microwave heater
int motorAngle = 0; // Current angle of the turntable motor (in degrees)
byte doorStatus = 0; // 0 for open, 1 for closed

// Create delay that we need in real time form as TickType_t
TickType_t xTickToWait = pdMS_TO_TICKS(500);

/* task functions */
void motorTaskFunction(void *pvParameters);
void printMessage(const char *message);
void doorTaskFunction(void *pvParameters);
void lampTaskFunction(void *pvParameters);
void heaterTaskFunction(void *pvParameters);
void selectSetup();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Create tasks
  xTaskCreate(heaterTaskFunction, "Heater Task", 128, NULL, 1, &xTaskHeater);

  // Start the FreeRTOS scheduler
  vTaskStartScheduler();

  // Prompt the user to select a pre-programmed setup
  selectSetup();

}

void loop() {
  // put your main code here, to run repeatedly:

}

// Task functions
void heaterTaskFunction(void *pvParameters)
{
  if(enable_debug) { Serial.println(F("Heater simulated")); }

  while (1)
  {
    Serial.print(F("[Heater Profiler] = "));
    Serial.println(heaterProfiler++);
    // Check if heaterPower and start/stop the heater accordingly
    // outer battery source, not from the arduino.
    if (heaterPower == 800)
    {
      // Run other tasks concurrently
      xTaskCreate(doorTaskFunction, "Door Task", 128, NULL, 1, &xTaskDoor);
      xTaskCreate(lampTaskFunction, "Lamp Task", 128, NULL, 1,  &xTaskLamp);
      xTaskCreate(motorTaskFunction, "Motor Task", 128, NULL, 1, &xTaskMotor);
      // print to lcd
      printMessage("Heater started (800W)");
      // Code to start the heater power
      // Here
    }
    else if (heaterPower == 400)
    {
      // Run other tasks concurrently
      vTaskResume(xTaskDoor);
      vTaskResume(xTaskLamp);
      vTaskResume(xTaskMotor);
      // print to lcd
      printMessage("Heater started (400W)");
      // logic algorithm here for the heater power
    }
    else
    {
      printMessage("Heater stopped");
      vTaskSuspend(xTaskDoor);
      vTaskSuspend(xTastLamp);
      vTaskSuspend(xTaskMotor);
      // logic here
    }

    // logic algorithm for turning on heater, lamp, motor & door
    vTaskDelay(xTickToWait);
  }

}

void lampTaskFunction(void *pvParameters)
{
  if(enable_debug) { Serial.println(F("Lamp simulated")); }
  bool isHeating = false;
  bool wasDoorOpen = false;
  
  while (1)
  {
    Serial.print(F("[Lamp Profiler] = "));
    Serial.println(lampProfiler++);

    // Check heating status and door status
    if (heaterPower == 400 || heaterPower == 800)
    { 
      // if door is open
      if (doorStatus == 0)
      {
        wasDoorOpen = true;
        isHeating = false;
        // lamp logic algorithm here if door is open
        printMessage("Light on. Door open (Not heating)");
        vTaskResume(xTaskLamp);
      }
      // if door is closed
      else if (doorStatus == 1)
      {
        wasDoorOpen = false;
        isHeating = true;
        // lamp logic algorithm here if door is closed
        printMessage("Light on (Heating)");
        vTaskResume(xTaskLamp);
      }
    }
    else {
      // if no heater power is set (400W or 800W)
      isHeating = false;
      if (doorStatus == 0)
      {
        // but door is open, leave lamp on
        wasDoorOpen = true;
        vTaskResume(xTaskLamp);
        printMessage("Door open. No Heater Power set. Light on.");
        // Code here for logic algorithm
      }
      // if door is closed
      else if (doorStatus == 1)
      {
        wasDoorOpen = false;
        vTaskSuspend(xTaskLamp);
        printMessage("Door closed. No Heater Power set. Light off.");
        // Code here for logic algorithm
      }

    }
    vTaskDelay(xTickToWait);

  }

}

void doorTaskFunction(void *pvParameters)
{
  if(enable_debug) { Serial.println(F("Door simulated")); }
  while(1)
  {
    Serial.print(F("[Door Profiler] = "));
    Serial.println(doorProfiler++);
    // Simulate reading the door status
    if (doorStatus == 0)
    {
      printMessage("Door is open");
    }
    else
    {
      printMessage("Door is closed");
    }
    vTaskDelay(xTickToWait);
  }
}

void motorTaskFunction(void *pvParameters)
{
  if(enable_debug) { Serial.println(F("Motor simulated")); }
  while(1)
  {
    Serial.print(F("[Motor Profiler] = "));
    Serial.println(motorProfiler++);
    // Check desired motor angle and perform rotation
    if (motorAngle == 30)
    {
      printMessage("Motor round 30 degrees");
      vTaskResume(xTaskMotor);
    }
    else if (motorAngle == 90)
    {
      printMessage("Motor round 90 degrees");
      vTaskResume(xTaskMotor);
    }
    else if (motorAngle == 360)
    {
      printMessage("Motor round 360 degrees");
      vTaskResume(xTaskMotor);
    }
    else if (motorAngle == 0)
    {
      printMessage("Motor round 0 degrees");
      vTaskResume(xTaskMotor);
    }

    vTaskDelay(xTickToWait);
  }
}

// Function to print messages to Serial terminal
void printMessage(const char *message)
{
  Serial.println(F(message));
}

// Menu for selecting pre-programmed setups
void selectSetup()
{
  Serial.println(F("Select a pre-programmed setup: "));
  Serial.println(F("1. Defrosting Meat (5 minutes at 800W"));
  Serial.println(F("2. Defrosting Vegetables (1 minute at 400W)"));
  Serial.println(F("3. General heating (30 seconds at 800W)"));

  while (!Serial.available())
  {
    // Wait for user input
  }

  // store user input in this int variable
  int setupChoice = Serial.parseInt();

  switch (setupChoice)
  {
    case 1:
      heaterPower = 800;
      Serial.println(F("Defrosting for 5 minutes"));
      // apply logic here
      vTaskDelay(pdMS_TO_TICKS(5 * 60 * 1000)); // 5 minutes
      heaterPower = 0; // Turn off the heater
      vTaskDelete(xTaskHeater); // Delete the heater task
      // more logic code if needed
      break;

    case 2:
      heaterPower = 400;
      Serial.println(F("Defrosting for 1 minute"));
      // apply logic here
      vTaskDelay(pdMS_TO_TICKS(1 * 60 * 1000)); // 1 minute
      heaterPower = 0;
      vTaskDelete(xTaskHeater);
      break;

    case 3:
      heaterPower = 800;
      Serial.println(F("Heating for 30 seconds"))
      // apply logic here
      vTaskDelay(pdMS_TO_TICKS(30 * 1000)); // 30 seconds
      heaterPower = 0;
      vTaskDelete(xTaskHeater);
      break;
    
    default:
      Serial.println(F("Invalid choice."));
      break;

  }
}