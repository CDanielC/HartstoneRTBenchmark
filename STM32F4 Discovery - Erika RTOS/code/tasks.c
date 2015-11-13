/*! \file tasks.c
    \brief Hartstone Benchmark Implementation
	
	@author Daniel Casini, Emiliano Palermiti, Matteo Pampana
*/

#include "globals.h"

#ifdef SKIP_1PERIOD
	#define TASK_CODE(INDEXTASK, INDEXARRAY) \
		if(terminate[INDEXARRAY]) {\
			terminate[INDEXARRAY] = 0;\
			console_out("skip 1 period\r\n");\
			TerminateTask();\
		}\
		WHETSTONE_CODE(Task##INDEXTASK, KWPP[INDEXARRAY]); \
		terminate[INDEXARRAY] = 0; \
		TerminateTask();
#else
	#define TASK_CODE(INDEXTASK, INDEXARRAY) \
		WHETSTONE_CODE(Task##INDEXTASK, taskSet[INDEXARRAY].kwpp); \
		if (taskSet[INDEXARRAY].hasMissed && 0==stop)\
		{\
			taskSet[INDEXARRAY].hasMissed = 0;\
			taskSet[INDEXARRAY].miss++;\
		}\
		else\
			taskSet[INDEXARRAY].met++;\
		TerminateTask();
#endif	/* SKIP_1PERIOD */\

TASK( Task1) {
	TASK_CODE(1, 0);
}

TASK( Task2) {
	TASK_CODE(2, 1);
}

TASK( Task3) {
	TASK_CODE(3, 2);
}

TASK( Task4) {
	TASK_CODE(4, 3);
}

TASK( Task5) {
	TASK_CODE(5, 4);
}

TASK( TaskA1) {
	TASK_CODE(A1, 5);
}

TASK( TaskA2) {
	TASK_CODE(A2, 6);
}

TASK( TaskA3) {
	TASK_CODE(A3, 7);
}

TASK( TaskA4) {
	TASK_CODE(A4, 8);
}

TASK( TaskA5) {
	TASK_CODE(A5, 9);
}

TASK( TaskA6) {
	TASK_CODE(A6, 10);
}

TASK( TaskA7) {
	TASK_CODE(A7, 11);
}

TASK( TaskA8) {
	TASK_CODE(A8, 12);
}

TASK( TaskA9) {
	TASK_CODE(A9, 13);
}

TASK( TaskA10) {
	TASK_CODE(A10, 14);
}

void PreTaskHook()
{
	TaskType myTask;
	GetTaskID(&myTask);
	if (testStarted && SuperTask == myTask)
	{
		//GetElapsedValue(myCounter, &testTickStart, &testDuration);
		//testStarted = 0;
		stop = 1;
		//USART_printf("Stop: %d\r\n", stop);
		resetAlarms(taskSet, N_TASK+MAX_NEWTASKS);
	}
}

void PostTaskHook()
{
	TaskType myTask;
	GetTaskID(&myTask);
	if (SuperTask == myTask)
	{
		/* Store the value of the timer */
		//GetCounterValue(myCounter, &testTickStart);

	}
}

void ErrorHook(StatusType Error) {
	AlarmType t;
	uint8_t i;

	if (0==stop && OSErrorGetServiceId() == OSServiceId_IncrementCounter) {
		t = OSError_IncrementCounter_AlarmID();
		terminateExperiment = TERMINATION_OK;
		for (i = 0; i < (N_TASK + MAX_NEWTASKS); i++) {
			if (t == taskSet[i].taskAlarm) {
				taskSet[i].skip++;
				taskSet[i].hasMissed = 1;
			}
		}
	}
}

TASK( SuperTask) {
	static uint16_t eCount = FIRST_EXP;
	static uint16_t tCount = 0;
	static uint8_t addedTasks = 0;

	uint8_t i;
	float delta_L;

	testStarted = 1;

	if (tCount > 0) {

		if (terminateExperiment == 0)
		{
			terminateExperiment = checkFixAllZero(taskSet, N_TASK+addedTasks,
				                              (TEST_DURATION - PRETEST_LAG));

			terminateExperiment = checkSmallPeriod(taskSet, N_TASK+addedTasks);
		}

		if (terminateExperiment > 0) {
			//USART_printf("EXPERIMENT %d FINISHED!!\r\n\r\n", (eCount+1));
			printTestResults(N_TASK+addedTasks, tCount, (eCount+1),terminateExperiment);
			eCount++;
			tCount = 0;
			resetBase(taskSet, N_TASK+addedTasks, kwips_rawspeed, initFreq, initKWPP);
			terminateExperiment = 0;
		}
		else
			printTestResults(N_TASK+addedTasks, tCount, (eCount+1), 0);
	}

	if (0 == eCount) { /* increase frequency of task 5 by the freq. of task 4*/
		resetStats(taskSet, N_TASK);
		delta_L = taskSet[3].freq * taskSet[4].kwpp;
		setStepSize(delta_L, kwips_rawspeed, &step_size);
		taskSet[4].freq = (tCount > 0) ? (taskSet[4].freq + taskSet[3].freq) : taskSet[4].freq;
		updateInfo(taskSet, N_TASK, kwips_rawspeed);
		addedTasks = 0;
	} else if (1 == eCount) { /* increase frequency by a factor of 1.1,1.2,...*/
		resetBase(taskSet, N_TASK, kwips_rawspeed, initFreq, initKWPP);
		delta_L = getTotKWPS(taskSet, N_TASK) * 0.1;
		setStepSize(delta_L, kwips_rawspeed, &step_size);
		scaleFreq(taskSet, (1.0 + 0.1 * tCount), N_TASK);
		updateInfo(taskSet, N_TASK, kwips_rawspeed);
		addedTasks = 0;
	} else if (2 == eCount) { /* increase workload by E3_ADDWKPP KWPPs */
		resetStats(taskSet, N_TASK);
		delta_L = E3_ADDKWPP * getTotFreq(taskSet, N_TASK);
		setStepSize(delta_L, kwips_rawspeed, &step_size);

		if (tCount > 0)
			scaleKWPP(taskSet, E3_ADDKWPP, N_TASK);

		updateInfo(taskSet, N_TASK, kwips_rawspeed);
		addedTasks = 0;
	}  else if (3 == eCount) { /* add 1 task per iteration */
		resetStats(taskSet, N_TASK+addedTasks);
		delta_L = taskSet[5].kwips;
		setStepSize(delta_L, kwips_rawspeed, &step_size);
		if (tCount>0)
		{
			addedTasks++;
			if (MAX_NEWTASKS == addedTasks)
				terminateExperiment = ERROR_CREAT;
		}
	} else if (4 == eCount) { /* increase workload by 1KWPP */
		resetStats(taskSet, N_TASK+addedTasks);
		//USART_printf("ENDING!!!!......\r\nEnd of the benchmark!!\r\n");
		TerminateTask();
	}


	tCount++;
	for (i = 0; i < (N_TASK + addedTasks); i++)
		SetRelAlarm(taskSet[i].taskAlarm, PRETEST_LAG*1000, taskSet[i].period);
	SetRelAlarm(AlarmSuper, TEST_DURATION * 1000, TEST_DURATION * 1000);
	stop = 0;
	TerminateTask();
}
