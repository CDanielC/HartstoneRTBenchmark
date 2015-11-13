#include "hartstone.h"

void hartstone_raw_speed(){
	portTickType start,stop = 0,el;
	uint32_t single_load = RAW_SINGLE_LOAD;
	raw_speed = 0;

	while( stop < (single_load*1000) ){
		start = xTaskGetTickCount();
		WHETSTONE_CODE(0,single_load);
		el = xTaskGetTickCount();
		el -= start;
		raw_speed++;
		stop += el;
	}
}

uint8_t total_deadline_miss(){
	uint8_t i = 0, sum = 0;

	for(i=0; i<(N_TASK+MAX_ADDITIONAL_TASKS); i++)
		sum += deadline_miss[i];

	return sum;
}

void baseline_test_init(){
	uint8_t k=0;

	// Baseline Test
	frequency[0] = BASELINE_FREQUENCY_0;
	period[0] = BASELINE_PERIOD_0;
	priority[0] = BASELINE_PRIORITY_0;
	load[0] = load_exp3[0] = BASELINE_LOAD_0;
	task_index[0] = 0;

	for(k=1;k<N_TASK;k++){
		frequency[k] = frequency[k-1]*2;
		period[k] = period[k-1]/2;
		priority[k] = priority[k-1]+1;
		load[k] = load_exp3[k] = load[k-1]/2;
		task_index[k] = k;
	}

	for(k=N_TASK;k<(N_TASK + MAX_ADDITIONAL_TASKS);k++){
		frequency[k] = frequency[2];
		period[k] = period[2];
		priority[k] = priority[2];
		load[k] = load[2];
		task_index[k] = k;
	}
}

void scale_frequencies(float scale){
	uint8_t i = 0;

	for(i=0; i<N_TASK; i++){
		frequency[i] = scale*frequency[i];
		period[i] = (1.0/frequency[i])*1000;
	}
}

void increment_workload(){
	uint8_t i;

	for(i=0; i<N_TASK; i++)
		load_exp3[i] += WORKLOAD_STEP;
}

void hartstone_create_taskset(uint8_t additional, pdTASK_CODE pvTaskCode){
	uint8_t k=0,error=0;

	if(additional >= MAX_ADDITIONAL_TASKS){
		hartstone_error(2);

	}

	for(k=0;k < (N_TASK + additional);k++){
		taskHandle[k] = NULL;
		xTaskCreate(pvTaskCode, ( const char * ) "Task", TASK_GEN_STACK_SIZE,(void *)&task_index[k], priority[k], &taskHandle[k] );
		if(taskHandle[k] == NULL)
			error++;
	}

	if(error>0){
		hartstone_error(2);
		//while(1);
	}
}

void hartstone_delete_taskset(uint8_t additional){
	uint8_t k=0,error=0;

	for(k = 0; k < (N_TASK + additional);k++){
		if(taskHandle[k] != NULL)
			vTaskDelete(taskHandle[k]);
		else
			error++;
	}

	if(error>0){
		hartstone_error(3);
		//while(1);
	}
}

void hartstone_print_report(uint8_t experiment_num, uint8_t test_num,uint8_t additional){

#ifdef GUI_OUTPUT
	USART_printf("%d,%d,%d,%d,%d",( total_deadline_miss() > 0 || additional > MAX_ADDITIONAL_TASKS )?1:0,experiment_num,test_num,raw_speed,(N_TASK + additional));
	uint8_t k,skipped;
	float load_p,workload;
	for(k=0;k<(N_TASK + additional);k++){
		load_p = ((experiment_num == 3)?load_exp3[k]:load[k]);
		workload = 100.0 * load_p*frequency[k]/raw_speed;
		USART_printf(",T%d,%.2f,%.2f,%.2f,%.2f",k+1,frequency[k],load_p,load_p*frequency[k],workload);
		if(deadline_met[k] == 0 && deadline_miss[k] == 0){
			deadline_miss[k] = 1;
		}
		skipped = ((deadline_miss[k]>0)?(TEST_LEN / period[k] - deadline_met[k] - deadline_miss[k]):0);
		USART_printf(",%d,%d,%d,%d",period[k],deadline_met[k],deadline_miss[k],skipped);
	}
	USART_printf(",%.2f;",hartstone_step_size(experiment_num));
#else
	uint8_t k,skipped;
	float load_p,workload,sum=0;

	USART_printf("Experiment:\tEXPERIMENT_%d\r\n\r\n",experiment_num);
	USART_printf("Raw speed in KWIPS: %d\r\n\r\n",raw_speed);
	USART_printf("Test %d characteristics:\r\n\r\n",test_num);
	USART_printf("Task \t Frequency \t Kilo-Whets \t Kilo-Whets \t Requested Workload\r\n");
	USART_printf("No. \t (Hertz) \t per period \t per second \t Utilization\r\n");
	for(k=0;k<(N_TASK + additional);k++){
		load_p = ((experiment_num == 3)?load_exp3[k]:load[k]);
		workload = 100.0 * load_p*frequency[k]/raw_speed;
		USART_printf("%d \t %.2f \t\t %.2f \t\t %.2f \t\t %.2f %% \r\n",k+1,frequency[k],load_p,load_p*frequency[k],workload);
		sum += (load_p*frequency[k]);
	}
	USART_printf("\t\t\t\t\t -------\t -------\r\n");
	USART_printf("\t\t\t\t\t %.2f \t\t %.2f %% \r\n\r\n",sum,sum*100/raw_speed);
	USART_printf("Experiment step size: %.2f %%\r\n",hartstone_step_size(experiment_num));
	USART_printf("\r\n-------------------------------------------------------\r\n");
	USART_printf("Test %d results:\r\n\r\n",test_num);
	USART_printf("Test duration (second): %d\r\n\r\n",TEST_LEN/1000);
	USART_printf("Task \t Period \t Met \t\t Missed \t Skipped\r\n");
	USART_printf("No. \t in msecs \t Deadlines \t Deadlines \t Deadlines\r\n");
	for(k=0;k<(N_TASK + additional);k++){
		if(deadline_met[k] == 0 && deadline_miss[k] == 0){
			deadline_miss[k] = 1;
		}
		skipped = ((deadline_miss[k]>0)?(TEST_LEN / period[k] - deadline_met[k] - deadline_miss[k]):0);
		USART_printf("%d \t %d \t\t %d \t\t %d \t\t %d \r\n",k+1,period[k],deadline_met[k],deadline_miss[k],skipped);
	}
	USART_printf("=======================================================\r\n\r\n");
#endif
}

float hartstone_step_size(uint8_t experiment_num){
	float step_size = 0;
	uint8_t i;

	switch(experiment_num){
		case 1:
			step_size = frequency[4-1]*load[5-1];
			break;
		case 2:
			for(i=0;i<N_TASK;i++)
				step_size += BASELINE_FREQUENCY_0*BASELINE_LOAD_0;
			step_size *= 0.1;
			break;
		case 3:
			for(i=0;i<N_TASK;i++)
				step_size += frequency[i] * WORKLOAD_STEP;
			break;
		case 4:
			step_size = frequency[3-1] * load[3-1];
			break;
		default:
			step_size = 0;
			break;
	}
	step_size = (step_size/raw_speed)*100;

	return step_size;
}

void hartstone_test(uint8_t exp,uint8_t test,uint8_t additional,pdTASK_CODE pvTaskCode){

	hartstone_create_taskset(additional,pvTaskCode);
	start = xTaskGetTickCount();
	vTaskDelay( TEST_LEN );
	hartstone_delete_taskset(additional);
	hartstone_print_report(exp,test+1,additional);
}


void hartstone_start(void) {

	/* Create the first task that manages the tests */
	xTaskCreate(vManagementTask, ( const char * ) "Task", TASK_MAN_STACK_SIZE, NULL, 7, NULL );
	vTaskStartScheduler(); // This should never return.
	while(1);
}

void hartstone_error(uint8_t errorCode) {
	switch(errorCode){
		case 2:
			#ifndef GUI_OUTPUT
				USART_printf("Error in the task set creation");
			#else
				USART_printf("2;");
				while(1)
					USART_printf(" ");
			#endif
			break;

		case 3:
			#ifndef GUI_OUTPUT
				USART_printf("Error in delete taskset");
			#else
				USART_printf("3;");
				while(1)
					USART_printf(" ");
			#endif
			break;

		case 4:
			#ifndef GUI_OUTPUT
				USART_printf("Error ");
			#else
				USART_printf("4;");
			#endif
			break;
	}
}

void vManagementTask( void * pvParameters ){
	uint8_t i = 0;

#ifdef RAW_TEST
	hartstone_raw_speed();
#endif

#ifdef EXP_1
	/* Experiment 1 */
	i = 0;
	float p;
	int p1;
	uint8_t first = 0;
	do{
		/* From specifics:
		 * The amount by which the
		 *	frequency increases must preserve the harmonic nature of the task set frequencies: this means a
		 *	minimum increase by an amount equal to the frequency of task 4.
		 */
		baseline_test_init();
		frequency[4] += (i)*frequency[3];
		p = (1.0/frequency[4])*1000;
		p1 = (int)p;
		period[4] = p1;

		if(period[4] == 1){
			if(first == 0)
				first = 1;
			else{
				hartstone_error(4);
				break;
			}
		}
		hartstone_test(1, i, 0, vGenericTask);

		i++;
	}while( total_deadline_miss() == 0 );
#endif

#ifdef EXP_2
	/* Experiment 2 */

	i = 0;
	first = 0;
	do{
		/* From specifics:
		 * Starting with the baseline task set, all the frequencies are scaled by 1.1, then 1.2,
		 * then 1.3, and so on for each new test until a deadline is missed
		 */
		baseline_test_init();
		scale_frequencies((1.0 + 0.1*i));

		if(period[4] == 1){
			if(first == 0)
				first = 1;
			else{
				hartstone_error(4);
				break;
			}
		}
		hartstone_test(2, i, 0, vGenericTask);
		i++;
	}while( total_deadline_miss() == 0 );
#endif

#ifdef EXP_3
	/* Experiment 3 */

	i = 0;
	baseline_test_init();
	do{

		hartstone_test(3, i, 0, vGenericTaskExp3);

		/* From specifics:
		 * Starting with the baseline task set, all the load are increased by 1KWIPP
		 * for each new test until a deadline is missed
		 */
		increment_workload();
		i++;
	}while( total_deadline_miss() == 0 );
#endif

#ifdef EXP_4
	/* Experiment 4 */

	i = 0;
	baseline_test_init();
	do{

		/* From specifics:
		 * Starting with the baseline task set, new tasks with the same frequency
		 * and workload as the "middle" task, task 3, of the baseline set are
		 * added until a deadline is missed. */

		/* Here i represents the number of additional tasks in this test */

		hartstone_test(4, i, i, vGenericTask);
		i++;
	}while( total_deadline_miss() == 0 && i<=MAX_ADDITIONAL_TASKS );
#endif
	while(1);
}


void vGenericTask( void * pvParameters )
{
	INIT_PERIODIC()
	START_PERIODIC()

	WHETSTONE_CODE(index,load[index])
	WAIT_FOR_NEXT_PERIOD()
}

void vGenericTaskExp3( void * pvParameters )
{
	INIT_PERIODIC()
	START_PERIODIC()

	WHETSTONE_CODE(index,load_exp3[index])
	WAIT_FOR_NEXT_PERIOD()
}
