/*! \file globals.h
    \brief Hartstone Benchmark global variables and defines

	@author Daniel Casini, Emiliano Palermiti, Matteo Pampana
*/

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "ee.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "cpu/common/inc/ee_stdint.h"
#include "base_serial.h"

#include "cpu/pic30/inc/ee_irqstub.h"

/*! \def	__WITH_GUI
    \brief	Macro that specifies the output format.
*/
#define __WITH_GUI


/*! \def	EXPERIMENT_1
    \brief	Constant that represents the first experiment.
*/
#define EXPERIMENT_1	0

/*! \def	EXPERIMENT_2
    \brief	Constant that represents the second experiment.
*/
#define EXPERIMENT_2	1

/*! \def	EXPERIMENT_3
    \brief	Constant that represents the third experiment.
*/
#define EXPERIMENT_3	2

/*! \def	EXPERIMENT_4
    \brief	Constant that represents the fourth experiment.
*/
#define EXPERIMENT_4	3

/*! \def	FIRST_EXP
    \brief	Constant that specify from which experiment the benchmark has to start.
*/
#define FIRST_EXP		EXPERIMENT_1

/*! \def	RAW_SPEED_MS
    \brief	Duration of the Raw Speed Test, in ms
*/
#define RAW_SPEED_MS	10000

/*! \def	N_TASK
    \brief	Number of tasks
*/
#define	N_TASK			5

/*! \def	MAX_NEWTASKS
    \brief	Number of additional tasks to handle EXPERIMENT_4
*/
#define	MAX_NEWTASKS	14

/*! \def	TEST_DURATION
    \brief	Duration of each test
*/
#define TEST_DURATION	10

/*! \def	PRETEST_LAG
    \brief	Delay between activation of tasks and the start of the test
*/
#define	PRETEST_LAG		5

/*! \def	T1_KWPP
    \brief	Baseline kwpp for task1
*/
#define	T1_KWPP			256

/*! \def	T2_KWPP
    \brief	Baseline kwpp for task2
*/
#define	T2_KWPP			128

/*! \def	T3_KWPP
    \brief	Baseline kwpp for task3
*/
#define	T3_KWPP			64

/*! \def	T4_KWPP
    \brief	Baseline kwpp for task4
*/
#define	T4_KWPP			32

/*! \def	T5_KWPP
    \brief	Baseline kwpp for task5
*/
#define	T5_KWPP			16

/*! \def	T1_FREQ
    \brief	Baseline frequency for task1
*/
#define	T1_FREQ			2.0

/*! \def	T2_FREQ
    \brief	Baseline frequency for task2
*/
#define	T2_FREQ			4.0

/*! \def	T3_FREQ
    \brief	Baseline frequency for task3
*/
#define	T3_FREQ			8.0

/*! \def	T4_FREQ
    \brief	Baseline frequency for task4
*/
#define	T4_FREQ			16.0

/*! \def	T5_FREQ
    \brief	Baseline frequency for task5
*/
#define	T5_FREQ			32.0

/*! \def	E3_ADDKWPP
    \brief	Amount of KWPP added for each Experiment 3 Test.
*/
#define	E3_ADDKWPP		2

/*! \def	TERMINATION_OK
    \brief	Code for normal termination.
*/
#define	TERMINATION_OK	1

/*! \def	ERROR_CREAT
    \brief	Error code to indicate an error in the creation of a new task.
*/
#define	ERROR_CREAT		2

/*! \def	ERROR_DEL
    \brief	Error code to indicate an error removing a task.
*/
#define	ERROR_DEL		3 //only FreeRTOS

/*! \def	ERROR_PER
    \brief	Error code to indicate when a task has a period smaller than the resolution of the calendar clock.
*/
#define	ERROR_PER		4

/*!
 *  \brief taskStat structure: it contains the tasks' attributes
 *  useful to execute the tests and experiments in the benchmark.
 */
typedef struct _statistics
{
	uint16_t met;		//!< number of met deadlines
	uint16_t miss;		//!< number of missed deadlines
	uint16_t skip;		//!< number of skipped deadlines

	uint16_t period;	//!< length of the period
	float freq;			//!< frequency of task's activation
	float util;			//!< percentage of utilization
	float kwips;		//!< kilowhetstone instruction per second
	uint16_t	kwpp;	//!< kilowhetstone instruction per period
	uint8_t hasMissed;	//!< flag that indicates if the task has missed a deadline
	uint8_t smallperiod;//!< the ERIKA alarm that activates the task
	AlarmType taskAlarm;//!< flag that indicates whether the task has a period smaller than the clock calendar resolution


} taskStat;

/*!
 * \var		const uint16_t initKWPP
 * \brief	Initial configuration of tasks' load
 */
extern const uint16_t initKWPP[N_TASK + MAX_NEWTASKS];

/*!
 * \var		const float initFreq
 * \brief	Initial configuration of tasks' frequency
 */
extern const float initFreq[N_TASK + MAX_NEWTASKS];

/*!
 * \var		const AlarmType initAlarms
 * \brief	Initial configuration of tasks' alarm
 */
extern const AlarmType initAlarms[N_TASK + MAX_NEWTASKS];

/*!
 * \var		taskStat taskSet
 * \brief	Array that contains the task set.
 */
extern taskStat taskSet[N_TASK+MAX_NEWTASKS];

/*!
 * \var		uint8_t terminateExperiment
 * \brief	Flag that indicates the termination of an experiment.
 */
extern volatile uint8_t terminateExperiment;


extern	TickType testTickStart, testDuration;


/*!
 * \var		uint8_t testStarted
 * \brief	Flag that indicates that a test has started.
 */
extern	volatile uint8_t testStarted;


/*!
 * \var		float kwips_rawspeed
 * \brief	The rawspeed.
 */
extern	float kwips_rawspeed;


/*!
 * \var		float step_size
 * \brief	The step_size relative to an experiment.
 */
extern	float step_size;

/*!
 * \var		uint8_t stop
 * \brief	Flag that indicates the stop condition of a test.
 */
extern	volatile uint8_t stop;

/**
  * @brief  Initialize the taskStat structure
  *
  *
  * @param  max_util: the max utilization achievable expressed by the rawspeed
  * @param  alarms: set of alarms
  * @param	initFreq: set of baseline frequency of task activation
  * @param	initKWPP: set of baseline KWPP
  *
  * @retval None
  */
void	initState(float max_util, const AlarmType *alarms, const float *initFreq, const uint16_t *initKWPP);

#include "nutsbolts.h"
#include "whetstone.h"

#endif /* GLOBALS_H_ */
