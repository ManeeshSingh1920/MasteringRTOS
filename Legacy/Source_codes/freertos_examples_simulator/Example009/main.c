/*
    FreeRTOS V6.0.5 - Copyright (C) 2009 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"


/* The task function. */
void vTaskFunction( void *pvParameters );

/* A variable that is incremented by the idle task hook function. */
static unsigned long ulIdleCycleCount = 0UL;

/* Define the strings that will be passed in as the task parameters.  These are
defined const and off the stack to ensure they remain valid when the tasks are
executing. */
const char *pcTextForTask1 = "Task 1 is running, ulIdleCycleCount = ";
const char *pcTextForTask2 = "Task 2 is running, ulIdleCycleCount = ";


void vAssertCalled( unsigned long ulLine, const char * const pcFileName );

/*-----------------------------------------------------------*/

int main( void )
{
	/* Create the first task at priority 1... */
	xTaskCreate( vTaskFunction, "Task 1", 240, (void*)pcTextForTask1, 1, NULL );

	/* ... and the second task at priority 2.  The priority is the second to
	last parameter. */
	xTaskCreate( vTaskFunction, "Task 2", 240, (void*)pcTextForTask2, 2, NULL );

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();

	for( ;; );
}
/*-----------------------------------------------------------*/

void vTaskFunction( void *pvParameters )
{
char *pcTaskName;

	/* The string to print out is passed in via the parameter.  Cast this to a
	character pointer. */
	pcTaskName = ( char * ) pvParameters;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task AND the number of times ulIdleCycleCount
        has been incremented. */
		printf( pcTaskName); printf( "%d\n",ulIdleCycleCount );

		/* Delay for a period.  This time we use a call to vTaskDelay() which
		puts the task into the Blocked state until the delay period has expired.
		The delay period is specified in 'ticks'. */
		vTaskDelay( 250 / portTICK_RATE_MS );
	}
}
/*-----------------------------------------------------------*/

/* This code segment is compiled out because it doesnt apply for simulator mode */
#if 0
static void prvEnterSleepModeUsingWFI(void)
{
	PMC->PMC_FSMR &= (uint32_t)~PMC_FSMR_LPM;
	SCB->SCR &= (uint32_t)~SCR_SLEEPDEEP;
	__WFI();
}


static void prvEnterSleepModeUsingWFE(void)
{
	PMC->PMC_FSMR &= (uint32_t)~PMC_FSMR_LPM;
	SCB->SCR &= (uint32_t)~SCR_SLEEPDEEP;
	__WFE();
}

#endif

/* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
and return void. */
void vApplicationIdleHook( void )
{
	/* This hook function does nothing but increment a counter. */
	ulIdleCycleCount++;

/* The below code segment is compiled out because it doesnt apply for simulator mode */
#if 0
	/* lets enter sleep mode here, CPU clock will be off */
	pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
	//prvEnterSleepModeUsingWFI();
#endif

	/* For simulator mode just printing the value of ulIdleCycleCount */
	printf("Idle hook function called ulIdleCycleCount = %ld\n ",ulIdleCycleCount );

}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}

static portBASE_TYPE xTraceRunning = pdFALSE;
void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
static portBASE_TYPE xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Parameters are not used. */
	( void ) ulLine;
	( void ) pcFileName;

 	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if( xPrinted == pdFALSE )
		{
			xPrinted = pdTRUE;
			if( xTraceRunning == pdTRUE )
			{
			//	vTraceStop();
				//prvSaveTraceFile();
			}
		}

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
			__asm volatile( "NOP" );
			__asm volatile( "NOP" );
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/