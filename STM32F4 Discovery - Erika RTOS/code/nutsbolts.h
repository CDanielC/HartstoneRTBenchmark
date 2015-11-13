/*! \file nutsbolts.h
    \brief Hartstone Benchmark utility functions
	
	@author Daniel Casini, Emiliano Palermiti, Matteo Pampana
*/


#ifndef NUTSBOLTS_H_
#define NUTSBOLTS_H_

#include "globals.h"

/** @addtogroup Functions	Useful Functions
  * @{
  */

/**
  * @brief  Convert a float in two integers: integer and decimal part.
  *
  *
  * @param  realval: the float that has to be converted
  * @param	dst: array of two integers in which the result will be stored
  *
  * @retval None
  */
void float2int2(float realval, uint16_t *dst);

/**
  * @brief  Compute the step size
  *
  *
  * @param  delta_L: increment factor of the test utilization
  * @param	speed: rawspeed
  * @param	stepsize: output variable
  *
  * @retval None
  */
void setStepSize(float delta_L, float speed, float *stepsize);

/**
  * @brief  Reset the alarms
  *
  * @param	ts: the taskset
  * @param	len: length of the taskset
  *
  * @retval None
  */
void resetAlarms(taskStat *ts, uint8_t len);

/**
  * @brief  Reset the taskset structure
  *
  * @param	ts: the taskset
  * @param	dim: length of the taskset
  *
  * @retval None
  */
void resetStats(taskStat *ts, uint8_t dim);

/**
  * @brief  Update periods, kwips and utilizations of the taskset
  *
  * @param	ts: the taskset
  * @param	dim: length of the taskset
  * @param	maxspeed: rawspeed
  *
  * @retval None
  */
void updateInfo(taskStat *ts, uint8_t dim, float maxspeed);

/**
  * @brief  Scale the frequencies of tasks activation by a factor.
  *
  * @param	ts: the taskset
  * @param	factor: increment factor of the frequencies
  * @param	dim: length of the taskset
  *
  * @retval None
  */
void scaleFreq(taskStat *ts, float factor, uint8_t dim);

/**
  * @brief  Scale the KWPP of tasks by a fixed amount.
  *
  * @param	ts: the taskset
  * @param	factor: increment amount of KWPP
  * @param	dim: length of the taskset
  *
  * @retval None
  */
void scaleKWPP(taskStat *ts, int factor, uint8_t dim);

/**
  * @brief  Reset the frequencies to the baseline ones
  *
  * @param	ts: the taskset
  * @param	dim: length of the taskset
  * @param	initFreq: baseline frequencies
  *
  * @retval None
  */
void resetFreq(taskStat *ts, uint8_t dim, const float *initFreq);

/**
  * @brief  Reset the KWPP to the baseline ones
  *
  * @param	ts: the taskset
  * @param	dim: length of the taskset
  * @param	initKWPP: baseline KWPP
  *
  * @retval None
  */
void resetKWPP(taskStat *ts, uint8_t dim, const uint16_t *initKWPP);

/**
  * @brief  Compute the rawspeed
  *
  * @retval None
  */
void calcRawSpeed();

/**
  * @brief  Get the total KWIPS
  *
  * @param	ts: the taskset
  * @param	n: length of the taskset
  *
  * @retval ret: the amount of KWIPSs of the entire taskset
  */
float getTotKWPS(taskStat *ts, uint8_t n);

/**
  * @brief  Get the total Frequency
  *
  * @param	ts: the taskset
  * @param	n: length of the taskset
  *
  * @retval ret: the sum of the frequencies relatives to the entire taskset
  */
float getTotFreq(taskStat *ts, uint8_t n);

/**
  * @brief  Get the total utilization
  *
  * @param	ts: the taskset
  * @param	n: length of the taskset
  *
  * @retval ret: the sum of the utilizations relatives to the entire taskset
  */
float getTotUtil(taskStat *ts, uint8_t n);

/**
  * @brief  Reset the structure with baseline parameters
  *
  * @param	ts: the taskset
  * @param	dim: length of the taskset
  * @param	max_util: rawspeed
  * @param	initFreq: baseline Frequencies
  * @param 	initKWPP: baseline KWPP
  *
  * @retval None
  */
void resetBase(taskStat *ts, uint8_t dim, float max_util,
		       const float *initFreq, const uint16_t *initKWPP);

/**
  * @brief  Check and fix the situation in which met, missed and skipped deadlines
  * are all set to zero. It sets missed deadlines equal to 1, and the skipped deadlines
  * equal to testlen / period - 1.
  *
  * @param	ts: the taskset
  * @param	dim: length of the taskset
  * @param	testlen: duration of the test
  *
  * @retval ret: TERMINATION_OK if the problem has been fixed, 0 otherwise
  */
uint8_t checkFixAllZero(taskStat *ts, uint8_t dim, float testlen);

/**
  * @brief  Check whether a task has a period smaller than the calendar clock
  * resolution.
  *
  * @param	ts: the taskset
  * @param	dim: length of the taskset
  *
  * @retval ret: ERROR_PER if the problem has been detected, 0 otherwise
  */
uint8_t checkSmallPeriod(taskStat *ts, uint8_t dim);

/**
  * @brief  Print the results in the format specified by the __WITH_GUI macro.
  *
  * @param	dim: length of the taskset
  * @param	nTest: test number
  * @param	nExp: experiment number
  * @param	errorCode: status of the experiment (running, terminated, error)
  *
  * @retval None
  */
void printTestResults(uint8_t dim, uint16_t nTest, uint16_t nExp, uint8_t errorCode);

/**
  * @}
  */

#endif /* NUTSBOLTS_H_ */
