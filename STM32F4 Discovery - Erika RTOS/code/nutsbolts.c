#include "nutsbolts.h"


void float2int2(float realval, uint16_t *dst)
{
	dst[0] = realval;
	float f2 = realval - dst[0];
	dst[1] = f2 * 10000;
}

void setStepSize(float delta_L, float speed, float *stepsize)
{
	*stepsize = (delta_L * 100) / speed;
}

float getTotKWPS(taskStat *ts, uint8_t n)
{
	uint8_t i;
	float res = 0;

	for(i=0; i<n; i++)
		res += ts[i].kwips;
	return res;
}

float getTotFreq(taskStat *ts, uint8_t n)
{
	uint8_t i;
	float res = 0;

	for(i=0; i<n; i++)
		res += ts[i].freq;
	return res;
}

float getTotUtil(taskStat *ts, uint8_t n)
{
	uint8_t i;
	float res = 0;

	for(i=0; i<n; i++)
		res += ts[i].util;
	return res;
}

void resetAlarms(taskStat *ts, uint8_t len) {
	TickRefType temp;
	uint8_t i;

	CancelAlarm(AlarmSuper);
	for(i=0; i<len; i++)
	{
		CancelAlarm(ts[i].taskAlarm);
	}
}

void resetStats(taskStat *ts, uint8_t dim)
{
	uint8_t i;
	for(i=0; i<dim; i++)
	{
		ts[i].met = 0;
		ts[i].miss = 0;
		ts[i].skip = 0;
		ts[i].hasMissed = 0;
	}

}

void updateInfo(taskStat *ts, uint8_t dim, float maxspeed) {
	uint8_t i;
	for (i = 0; i < dim; i++)
	{
		ts[i].period = 1000. / ts[i].freq;
		ts[i].kwips = ts[i].freq * ts[i].kwpp;
		ts[i].util = (ts[i].kwips / maxspeed) * 100;
	}
}

void scaleFreq(taskStat *ts, float factor, uint8_t dim) {
	uint8_t i;
	for (i = 0; i < dim; i++)
		ts[i].freq *= factor;
}

void scaleKWPP(taskStat *ts, int factor, uint8_t dim) {
	uint8_t i;
	for (i = 0; i < dim; i++)
		ts[i].kwpp += factor;
}

void resetFreq(taskStat *ts, uint8_t dim, const float *initFreq) {
	uint8_t i;
	for (i = 0; i < dim; i++)
	{
		ts[i].freq = initFreq[i];
		ts[i].period = 1000. / initFreq[i];
	}
}

void resetKWPP(taskStat *ts, uint8_t dim, const uint16_t *initKWPP) {
	uint8_t i;
	for (i = 0; i < dim; i++)
		ts[i].kwpp = initKWPP[i];
}

void resetBase(taskStat *ts, uint8_t dim, float max_util,
		       const float *initFreq, const uint16_t *initKWPP)
{
	uint8_t i;
	for (i = 0; i < dim; i++) {
		//terminate[i] = 0;
		taskSet[i].freq = initFreq[i];
		taskSet[i].period = 1000. / taskSet[i].freq;
		taskSet[i].kwpp = initKWPP[i];
		taskSet[i].kwips = taskSet[i].freq * taskSet[i].kwpp;
		taskSet[i].util = taskSet[i].kwips / max_util * 100;
		taskSet[i].smallperiod = 0;
	}
	resetStats(ts, dim);
}

void calcRawSpeed()
{
	TickType start, stop, el;
	stop = 0;
	int count = 0;
	while (stop < 10000)
	{
		GetCounterValue(myCounter, &start);
		WHETSTONE_CODE(0,10);
		GetElapsedValue(myCounter, &start, &el);
		//printf("aaa %d: total %d\r\n ms", el, stop);
		count+=10;
		stop += el;
	}
	kwips_rawspeed = 1000. * ((float)count/(float)stop);
}

uint8_t checkFixAllZero(taskStat *ts, uint8_t dim, float testlen)
{
	uint8_t i;
	for (i=0; i<dim; i++)
	{
		if (ts[i].met == 0 && ts[i].skip == 0 && ts[i].miss == 0)
		{
			ts[i].miss = 1;
			ts[i].skip = (testlen / ts[i].period) - 1.0;
			return TERMINATION_OK;
		}
	}
	return 0;
}

uint8_t checkSmallPeriod(taskStat *ts, uint8_t dim)
{
	uint8_t i;
	for (i=0; i<dim; i++)
	{
		if (ts[i].period == 1 && ts[i].smallperiod == 0)
		{
			ts[i].smallperiod = 1;
		}
		else if (ts[i].period == 1 && ts[i].smallperiod == 1)
		{
			return ERROR_PER;
		}
	}
	return 0;
}


void printTestResults(uint8_t dim, uint16_t nTest, uint16_t nExp, uint8_t errorCode)
{
#ifndef __WITH_GUI
	uint16_t temp[2];
	uint16_t temp2[2];
	uint16_t temp3[2];
	uint8_t i;
	float t;

	USART_printf("============================================================================\r\n\r\n");
	if (errorCode)
	{
		USART_printf("Results of Experiment:\tEXPERIMENT_%d\r\n", nExp);
		USART_printf("error code:\t%d\r\n", errorCode);
	}
	else
		USART_printf("Experiment:\tEXPERIMENT_%d\r\n", nExp);

	USART_printf("Completion on:\tMiss at least 1 deadline\r\n\r\n");
	float2int2(kwips_rawspeed, temp);
	USART_printf("Raw speed in Kilo-Whetstone Instructions Per Second (KWIPS):\t%4d.%04d\r\n\r\n", temp[0], temp[1]);
	//USART_printf("Raw speed in Kilo-Whetstone Instructions Per Second (KWIPS):\t%4.04f\r\n\r\n", kwips_rawspeed);
	USART_printf("Test %d characteristics:\r\n\r\n", nTest);
	USART_printf("\tTask\tFrequency\tKWIPP\tKWIPS    \tUtilization\r\n");

	for(i=0; i<dim; i++)
	{
		float2int2(taskSet[i].freq, temp);
		float2int2(taskSet[i].kwips, temp2);
		float2int2(taskSet[i].util, temp3);
		USART_printf("\t%3d\t%4d.%04d\t%5d\t%4d.%04d\t%4d.%04d%%\r\n",
				(i+1), temp[0], temp[1], taskSet[i].kwpp, temp2[0], temp2[1],
				temp3[0], temp3[1]);
	}

	t = getTotKWPS(taskSet, dim);
	float2int2(t, temp);
	USART_printf("\r\nTotal KWIPS:\t\t%4d.%02d\r\n", temp[0], temp[1]);

	t = getTotUtil(taskSet, dim);
	float2int2(t, temp);
	USART_printf("Total Utilization:\t%4d.%02d%%\r\n", temp[0], temp[1]);

	float2int2(step_size, temp);
	USART_printf("\r\nExperiment Step Size:\t%4d.%04d%%\r\n", temp[0], temp[1]);
	USART_printf("\r\n----------------------------------------------------------------------------\r\n\r\n");
	USART_printf("Test %d results:\r\n\r\n", nTest);
	USART_printf("Test duration(seconds): \t %2d.%01d\r\n\r\n", (TEST_DURATION - PRETEST_LAG), 0);
	USART_printf("\tTask\tPeriod\tMet \tMiss\tSkip\r\n");

	for(i=0; i<dim; i++)
	{
		USART_printf("\t%4d\t%6d\t%4d\t%4d\t%4d\r\n",
				(i+1), taskSet[i].period, taskSet[i].met, taskSet[i].miss, taskSet[i].skip);
	}
	USART_printf("============================================================================\r\n\r\n");
#else
	uint16_t temp[2];
	uint16_t temp2[2];
	uint16_t temp3[2];
	uint8_t i;
	float t;

	if (errorCode > 1)
	{
		USART_printf("%d;", errorCode);
		return;
	}

	USART_printf("%d,", errorCode);

	USART_printf("%d,%d,",nExp, nTest);
	float2int2(kwips_rawspeed, temp);
	USART_printf("%d.%04d,", temp[0], temp[1]);

	USART_printf("%d,", dim);
	for(i=0; i<dim; i++)
	{
		float2int2(taskSet[i].freq, temp);
		float2int2(taskSet[i].kwips, temp2);
		float2int2(taskSet[i].util, temp3);
		USART_printf("T%d,%d.%04d,%d,%d.%04d,%d.%04d%,",
					(i+1), temp[0], temp[1], taskSet[i].kwpp, temp2[0], temp2[1],
					temp3[0], temp3[1]);
		USART_printf("%d,%d,%d,%d,",
					taskSet[i].period, taskSet[i].met, taskSet[i].miss, taskSet[i].skip);
	}

	float2int2(step_size, temp);
	USART_printf("%d.%04d", temp[0], temp[1]);

	USART_printf(";");
#endif
}
