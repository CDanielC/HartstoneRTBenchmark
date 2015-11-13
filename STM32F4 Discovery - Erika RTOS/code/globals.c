#include "globals.h"

volatile uint8_t terminateExperiment;
TickType testTickStart, testDuration;
volatile uint8_t testStarted;
float kwips_rawspeed;
float step_size;
taskStat taskSet[N_TASK+MAX_NEWTASKS];
volatile uint8_t stop;

const uint16_t initKWPP[N_TASK + MAX_NEWTASKS] = { T1_KWPP, T2_KWPP, T3_KWPP, T4_KWPP, T5_KWPP,
												   T3_KWPP, T3_KWPP, T3_KWPP, T3_KWPP, T3_KWPP,
												   T3_KWPP, T3_KWPP, T3_KWPP, T3_KWPP, T3_KWPP};

const float initFreq[N_TASK + MAX_NEWTASKS] = { T1_FREQ, T2_FREQ, T3_FREQ, T4_FREQ, T5_FREQ,
												   T3_FREQ, T3_FREQ, T3_FREQ, T3_FREQ, T3_FREQ,
												   T3_FREQ, T3_FREQ, T3_FREQ, T3_FREQ, T3_FREQ};

const AlarmType initAlarms[N_TASK + MAX_NEWTASKS] = { AlarmTask1, AlarmTask2, AlarmTask3,
		AlarmTask4, AlarmTask5, AlarmTaskA1, AlarmTaskA2, AlarmTaskA3,
		AlarmTaskA4, AlarmTaskA5, AlarmTaskA6, AlarmTaskA7, AlarmTaskA8,
		AlarmTaskA9, AlarmTaskA10};

void	initState(float max_util, const AlarmType *alarms,
				  const float *initFreq, const uint16_t *initKWPP)
{
	uint8_t i;
	testStarted = 0;
	terminateExperiment = 0;
	for (i = 0; i < (N_TASK + MAX_NEWTASKS); i++) {
		//terminate[i] = 0;
		taskSet[i].freq = initFreq[i];
		taskSet[i].period = 1000. / taskSet[i].freq;
		taskSet[i].kwpp = initKWPP[i];
		taskSet[i].kwips = taskSet[i].freq * taskSet[i].kwpp;
		taskSet[i].util = taskSet[i].kwips / max_util * 100.0;
		taskSet[i].taskAlarm = alarms[i];
		taskSet[i].smallperiod = 0;
	}
}
