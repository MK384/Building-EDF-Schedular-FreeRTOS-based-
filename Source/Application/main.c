/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 *
 * Main.c also creates a task called "Check".  This only executes every three
 * seconds but has the highest priority so is guaranteed to get processor time.
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is
 * incremented each time the task successfully completes its function.  Should
 * any error occur within such a task the count is permanently halted.  The
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL ((unsigned char)0x01)

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE ((unsigned long)115200)

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware(void);
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------------\
								System Tasks Requirments
\-----------------------------------------------------------------*/

const size_t xMessageBufferSizeBytes = 25;

MessageBufferHandle_t xMessageBuffer1, xMessageBuffer2, xMessageBuffer3;

/*-----------------------------------------------------------------\
								System Tasks Implementation
\-----------------------------------------------------------------*/

void Button_1_Monitor(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 50;
	BaseType_t xWasDelayed;

	char *pvTxData;
	pinState_t B1_state = PIN_IS_LOW;

	// SET TASK TAG FOR RACING PURPOSE
	vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)1);
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	// Initialise the message buffer
	xMessageBuffer1 = xMessageBufferCreate(xMessageBufferSizeBytes);
	/* The parameter value is expected to be 1 as 1 is passed in the
	pvParameters value in the call to xTaskCreate() below. */
	configASSERT(((uint32_t)pvParameters) == 1);

	for (;;)
	{
		/* Task code goes here. */

		if (GPIO_read(PORT_0, PIN0) != B1_state)

		{
			B1_state = GPIO_read(PORT_0, PIN0);

			if (B1_state == PIN_IS_HIGH)
			{
				pvTxData = "Rising Edge on Buton 1";
				xMessageBufferSend(xMessageBuffer1,
								   (void *)pvTxData,
								   xMessageBufferSizeBytes,
								   (TickType_t)0);
			}
			else
			{
				pvTxData = "Falling Edge on Buton 1";
				xMessageBufferSend(xMessageBuffer1,
								   (void *)pvTxData,
								   xMessageBufferSizeBytes,
								   (TickType_t)0);
			}
		}

		xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void Button_2_Monitor(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 50;
	BaseType_t xWasDelayed;

	char *pvTxData;
	pinState_t B2_state = PIN_IS_LOW;

	// SET TASK TAG FOR RACING PURPOSE
	vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)2);
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	// Initialise the message buffer
	xMessageBuffer1 = xMessageBufferCreate(xMessageBufferSizeBytes);
	/* The parameter value is expected to be 1 as 1 is passed in the
	pvParameters value in the call to xTaskCreate() below. */
	configASSERT(((uint32_t)pvParameters) == 1);

	for (;;)
	{
		/* Task code goes here. */

		if (GPIO_read(PORT_0, PIN1) != B2_state)

		{
			B2_state = GPIO_read(PORT_0, PIN1);

			if (B2_state == PIN_IS_HIGH)
			{
				pvTxData = "Rising Edge on Buton 2";
				xMessageBufferSend(xMessageBuffer2,
								   (void *)pvTxData,
								   xMessageBufferSizeBytes,
								   (TickType_t)0);
			}
			else
			{
				pvTxData = "Falling Edge on Buton 2";
				xMessageBufferSend(xMessageBuffer2,
								   (void *)pvTxData,
								   xMessageBufferSizeBytes,
								   (TickType_t)0);
			}
		}

		xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void Periodic_Transmitter(void *pvParameters)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 100;
	BaseType_t xWasDelayed;

	char *pvTxData;

	// SET TASK TAG FOR RACING PURPOSE
	vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)3);
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	// Initialise the message buffer
	xMessageBuffer3 = xMessageBufferCreate(xMessageBufferSizeBytes);

	/* The parameter value is expected to be 1 as 1 is passed in the
	pvParameters value in the call to xTaskCreate() below. */
	configASSERT(((uint32_t)pvParameters) == 1);

	for (;;)
	{
		/* Task code goes here. */

		pvTxData = "This is Periodic String";
		xMessageBufferSend(xMessageBuffer3,
						   (void *)pvTxData,
						   xMessageBufferSizeBytes,
						   (TickType_t)0);

		xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void Uart_Receiver(void *pvParameters)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;
	BaseType_t xWasDelayed;

	uint8_t ucRxData[xMessageBufferSizeBytes];
	size_t xReceivedBytes;

	// SET TASK TAG FOR RACING PURPOSE
	vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)4);
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	/* The parameter value is expected to be 1 as 1 is passed in the
	pvParameters value in the call to xTaskCreate() below. */
	configASSERT(((uint32_t)pvParameters) == 1);

	for (;;)
	{
		/* Task code goes here. */
		xReceivedBytes = xMessageBufferReceive(xMessageBuffer1,
											   (void *)ucRxData,
											   xMessageBufferSizeBytes,
											   (TickType_t)0);
		if (xReceivedBytes > 0)
		{
			vSerialPutString((const signed char *)ucRxData, xReceivedBytes);
		}
		xReceivedBytes = xMessageBufferReceive(xMessageBuffer2,
											   (void *)ucRxData,
											   xMessageBufferSizeBytes,
											   (TickType_t)0);
		if (xReceivedBytes > 0)
		{
			vSerialPutString((const signed char *)ucRxData, xReceivedBytes);
		}
		xReceivedBytes = xMessageBufferReceive(xMessageBuffer3,
											   (void *)ucRxData,
											   xMessageBufferSizeBytes,
											   (TickType_t)0);
		if (xReceivedBytes > 0)
		{
			vSerialPutString((const signed char *)ucRxData, xReceivedBytes);
		}

		xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void Load_1_Simulation(void *pvParameters)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10;
	BaseType_t xWasDelayed;

	// SET TASK TAG FOR RACING PURPOSE
	vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)5);
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	/* The parameter value is expected to be 1 as 1 is passed in the
	pvParameters value in the call to xTaskCreate() below. */
	configASSERT(((uint32_t)pvParameters) == 1);

	for (;;)
	{
		/* Task code goes here. */
		int i;
		for (i = 0; i < 33333; i++)
		{
			__asm("NOP");
		}

		xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void Load_2_Simulation(void *pvParameters)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 100;
	BaseType_t xWasDelayed;

	// SET TASK TAG FOR RACING PURPOSE
	vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)6);

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	/* The parameter value is expected to be 1 as 1 is passed in the
	pvParameters value in the call to xTaskCreate() below. */
	configASSERT(((uint32_t)pvParameters) == 1);

	for (;;)
	{
		/* Task code goes here. */
		int i;
		for (i = 0; i < 80000; i++)
		{
			__asm("NOP");
		}
		xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

/*--------------------------------------------------------------------*/
/*
@BRIEF defines for the tasks periods
*/

#define xTsk_B1_period ((uint32_t)50)
#define xTsk_B2_period ((uint32_t)50)
#define xTsk_Tx_period ((uint32_t)100)
#define xTsk_UART_period ((uint32_t)20)
#define xTsk_Load1_period ((uint32_t)10)
#define xTsk_Load2_period ((uint32_t)100)

/*
@BRIEF declare tasks handlers
*/

TaskHandle_t xTsk_1_Handler = NULL;
TaskHandle_t xTsk_2_Handler = NULL;
TaskHandle_t xTsk_3_Handler = NULL;
TaskHandle_t xTsk_4_Handler = NULL;
TaskHandle_t xTsk_5_Handler = NULL;
TaskHandle_t xTsk_6_Handler = NULL;
/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler.
 */
int main(void)
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	xTaskPeriodicCreate(
		Load_1_Simulation,	/* Function that implements the task. */
		"LOAD_1",			/* Text name for the task. */
		96,					/* Stack size in words, not bytes. */
		(void *)1,			/* Parameter passed into the task. */
		1,					/* Priority at which the task is created. */
		&xTsk_5_Handler,	/* Used to pass out the created task's handle. */
		xTsk_Load1_period); /* Task period to calculate task deadline*/

	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for (;;)
		;
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware(void)
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();

	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/
