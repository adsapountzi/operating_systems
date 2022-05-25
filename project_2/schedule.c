/* schedule.c
 * This file contains the primary logic for the 
 * scheduler.
 */
#include "schedule.h"
#include "macros.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "list.h"

#define NEWTASKSLICE (NS_TO_JIFFIES(100000000))

/* Local Globals
 * rq - This is a pointer to the runqueue that the scheduler uses.
 * current - A pointer to the current running task.
 */
struct runqueue *rq;
struct task_struct *current;

/* External Globals
 * jiffies - A discrete unit of time used for scheduling.
 *			 There are HZ jiffies in a second, (HZ is 
 *			 declared in macros.h), and is usually
 *			 1 or 10 milliseconds.
 */
extern long long jiffies;
extern struct task_struct *idle;

/*-----------------Initilization/Shutdown Code-------------------*/
/* This code is not used by the scheduler, but by the virtual machine
 * to setup and destroy the scheduler cleanly.
 */
 
 /* initscheduler
  * Sets up and allocates memory for the scheduler, as well
  * as sets initial values. This function should also
  * set the initial effective priority for the "seed" task 
  * and enqueu it in the scheduler.
  * INPUT:
  * newrq - A pointer to an allocated rq to assign to your
  *			local rq.
  * seedTask - A pointer to a task to seed the scheduler and start
  * the simulation.
  */
void initschedule(struct runqueue *newrq, struct task_struct *seedTask)
{
	seedTask->next = seedTask->prev = seedTask;
	newrq->head = seedTask;
	newrq->nr_running++;
}

/* killschedule
 * This function should free any memory that 
 * was allocated when setting up the runqueu.
 * It SHOULD NOT free the runqueue itself.
 */
void killschedule()
{
	return;
}


void print_rq () {
	struct task_struct *curr;
	
	printf("Rq: \n");
	curr = rq->head;
	if (curr)
		printf("%p", curr);
	while(curr->next != rq->head) {
		curr = curr->next;
		printf(", %p", curr);
	};
	printf("\n");
}

/*-------------Scheduler Code Goes Below------------*/
/* This is the beginning of the actual scheduling logic */

/* schedule
 * Gets the next task in the queue
 */
void schedule()
{
	struct task_struct *next, *temp = rq->head;
	unsigned long long time = sched_clock();
	double min_exp_burst;
	double max_waiting_rq = 0;
	double temp_last_burst, temp_exp_burst;
	double goodness, min_goodness, temp_goodness;
	int i;
//	printf("In schedule\n");
//	print_rq();
	
	current->need_reschedule = 0; /* Always make sure to reset that, in case *
								   * we entered the scheduler because current*
								   * had requested so by setting this flag   */
	//printf("mesa sto resc\n");
	if (rq->nr_running == 1) {
		
		context_switch(rq->head);
	}
	else {	
		//printf("mesa sto else\n");
		current->last_run = time;
		temp =  rq->head->next;
		//printf("head next: %p ",temp);
		//Temporary variables for the proc that is currently running
		temp_last_burst = time - current->got_cpu;
		temp_exp_burst = (temp_last_burst + 0.5*(current->exp_burst))/(1.5);
		//printf("temp_exp_burst: %lf time: %llu temp_last_burst: %lf current->exp_burst: %lf \n",temp_exp_burst, time, temp_last_burst, current->exp_burst );

		//min_exp_burst starts at that value so that it is included in the for loop
		min_exp_burst = ULONG_MAX;
		max_waiting_rq = 0;
		next = NULL;
		//printf("current_temp_burst:%p -> %lf\n", current, temp_exp_burst);

		//printf("--------------------IN FOR LOOP %ld-----------------\n", rq->nr_running);
		for (i = 0; i < rq->nr_running - 1 ; i++){
			if (temp == current){
				//printf("head: %p current:%p -> %lf\n",rq->head, temp, temp_exp_burst);
				if (temp_exp_burst < min_exp_burst){
					min_exp_burst = temp_exp_burst;
					next = temp;
				}
				if (max_waiting_rq < time - current->last_run){
					max_waiting_rq = time - current->last_run;
				}
				temp = temp->next;
				continue;
			}
			if (min_exp_burst > temp->exp_burst){
				min_exp_burst = temp->exp_burst;
				next = temp;
			}
			if (max_waiting_rq < time - temp->last_run){
				max_waiting_rq = time - temp->last_run;
			}
			
			//printf("head: %p temp:%p -> %lf\n",rq->head, temp, temp->exp_burst);
			temp = temp->next;
		}
		//printf("--------------------EXIT FOR LOOP-----------------\n");


		//----------- UNCOMMENT FOR GOODNESS ------------//
		
		temp = rq->head->next;
		temp_goodness = ((1 + temp_exp_burst)/(1 + min_exp_burst))*((1 + max_waiting_rq)/(1 + time - current->last_run));
		min_goodness = ULONG_MAX;
		//printf("temp_exp_burst: %lf time: %llu temp_last_burst: %lf current->exp_burst: %lf temp_goodness: %lf\n",temp_exp_burst, time, temp_last_burst, current->exp_burst, temp_goodness);
		temp = rq->head->next;

		//printf("--------------------IN FOR LOOP %ld-----------------\n", rq->nr_running);
		for (i = 0; i < rq->nr_running - 1; i++){
			if (temp == current){
				//printf("head: %p current:%p -> %lf\n",rq->head, temp, temp_goodness);
				if (min_goodness > temp_goodness){
					min_goodness = temp_goodness;
					next = temp;
				}
				temp = temp->next;
				continue;
			}
			goodness = ((1 + temp->exp_burst)/(1 + min_exp_burst))*((1 + max_waiting_rq)/(1 + time - temp->last_run));
			if (min_goodness > goodness){
				min_goodness = goodness;
				next = temp;
			}
			//printf("head: %p temp:%p -> %lf\n",rq->head, temp, goodness);
			temp = temp->next;
		}
		//-----------------------------------------------//
		
		// printf("edw %lu\n", rq->nr_running);
		//print_rq();
		//printf("next:%p current:%p\n",next, current);
		if(current != next){
			//printf("Got CPU: %p -> %llu\n", next, time);
			next->got_cpu = time;

			//calculating next expected burst for the proccess that will stop running
			current->last_burst = time - current->got_cpu;
			current->last_run = time;
			current->exp_burst = (current->last_burst + 0.5*(current->exp_burst))/(1.5);
			
			context_switch(next);
		}
	}
}


/* sched_fork
 * Sets up schedule info for a newly forked task
 */
void sched_fork(struct task_struct *p)
{
	p->time_slice = 100;
	//-------------------//
	p->last_burst = 0;
	p->exp_burst = 0;
	p->last_run = sched_clock();
	p->got_cpu = 0;



}

/* scheduler_tick
 * Updates information and priority
 * for the task that is currently running.
 */
void scheduler_tick(struct task_struct *p)
{
	schedule();
}

/* wake_up_new_task
 * Prepares information for a task
 * that is waking up for the first time
 * (being created).
 */
void wake_up_new_task(struct task_struct *p)
{	
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	
	p->last_burst = 0;
	p->exp_burst = 0;
	p->last_run = sched_clock();
	p->got_cpu = 0;

	rq->nr_running++;
}

/* activate_task
 * Activates a task that is being woken-up
 * from sleeping.
 */
void activate_task(struct task_struct *p)
{
	unsigned long long time = sched_clock();
	
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	p->last_run = time;
	p->got_cpu = 0;

	rq->nr_running++;
}

/* deactivate_task
 * Removes a running task from the scheduler to
 * put it to sleep.
 */
void deactivate_task(struct task_struct *p)
{
	//print_rq();
	//printf("p: %p, p->prev: %p, p->next: %p\n",p, p->prev, p->next);
	unsigned long long time = sched_clock();
	p->prev->next = p->next;
	//printf("edw\n");
	p->next->prev = p->prev;
	//printf("ekei\n");
	p->next = p->prev = NULL; /* Make sure to set them to NULL *
							   * next is checked in cpu.c      */
	
	p->last_burst = time - p->got_cpu;
	p->last_run = time;
	p->exp_burst = (p->last_burst + 0.5*(p->exp_burst))/(1.5);


	rq->nr_running--;
}
