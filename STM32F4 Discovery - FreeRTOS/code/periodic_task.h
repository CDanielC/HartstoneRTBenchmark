/*! \file periodic_task.h
    \brief Periodic Tast Implementation
	
	@author Daniel Casini, Emiliano Palermiti, Matteo Pampana
*/
#ifndef PERIODIC_MACRO_H
#define PERIODIC_MACRO_H

/* PERIODIC MACRO */

/* TASK STRUCTURE */
/*
 * void vTask (void * pvparameters){
 *
 * 		INIT_PERIODIC()
 *
 * 		START_PERIODIC()
 *
 * 			task_body
 *
 * 		WAIT_FOR_NEXT_PERIOD()
 *
 * }
 */


/*! \def	CEILING(x,y)
    \brief	Computes the ceiling
*/
#define CEILING(x,y) ((x == 0)? 0 : (1 + ((x - 1) / y)))

/*! \def	INIT_PERIODIC()
    \brief	Initializes the periodic behavior
*/
#define INIT_PERIODIC() \
			uint8_t index = *((uint8_t*)pvParameters); \
			deadline_miss[index] = 0; \
			deadline_met[index] = 0; \
			portTickType xAct,xResp,xCompl; \
			portTickType xPeriod = period[index]; \
			xAct = start;

/*! \def	START_PERIODIC()
    \brief	Starts the periodic behavior
*/
#define START_PERIODIC()\
			while(1){

/*! \def	WAIT_FOR_NEXT_PERIOD()
    \brief	Implementation of the benchmark deadline behavior
*/
#define WAIT_FOR_NEXT_PERIOD() \
				vTaskSuspendAll(); \
				xCompl = xTaskGetTickCount(); \
				xResp = xCompl - xAct; \
				xResp = ((xResp == 0)?1:xResp); \
				xAct = xAct + CEILING(xResp, xPeriod) * xPeriod; \
				if(CEILING(xResp,xPeriod) == 1) \
					deadline_met[index]++; \
				else \
					deadline_miss[index]++; \
				xTaskResumeAll(); \
				vTaskDelayUntil(&xCompl, xAct - xCompl); \
			}

/******************/

#endif /* PERIODIC_MACRO_H */
