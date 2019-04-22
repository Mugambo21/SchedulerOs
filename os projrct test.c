#include <stdio.h>
#define SIZE 100
typedef struct Prs {
	int p_no;
	int arr_time;
	int burst_time;
} Prs;
Prs * queue[MAX_QUEUE_SIZE];
int front = 0, rear = -1, processed = 0, curr_time = 0, tq = 6;
int last_front = 0, last_rear = -1;
void swap(Process * a, Process * b) {
	Process temp = * a;
	* a = * b;
	* b = temp;
}
void sort(Process p[], int n) {
	int i, j;
	short swapped;	
	for (i = 0; i < n; ++i) {
		swapped = 0;
		for (j = 0; j < n-i-1; ++j)
		{
			if (p[j].arr_time > p[j+1].arr_time)
			{
				swap(&p[j], &p[j+1]);
				swapped = 1;
			}
		}
		if (swapped == 0)
			break;
	}
}
void enqueue(Pss p[], int n) {
	int i, j, can_insert;
	for (i = 0; i < n; ++i)
	{
		can_insert = 1;
		
		if (p[i].arr_time <= curr_time && p[i].burst_time > 0)
		{
			if (front == 0) {
				queue[++rear] = &p[i];
			}
			else
			{
				for (j = last_front; j <= last_rear; ++j) {
					if (queue[j]->p_no == p[i].p_no)
						can_insert = 0;
				}
				if (can_insert == 1)
					queue[++rear] = &p[i];
			}
		}
	}
	for (i = last_front; i <= last_rear; ++i)
	{
		if (queue[i]->burst_time > 0)
			queue[++rear] = queue[i];
	}
}
int main() {
	int n, i;
	short err_flag = 0;	
	do {
		if (err_flag == 1)
			fprintf(stderr, "\nNumber of processes should be greater than 1.\n");
		printf("Enter the number of processes: ");
		scanf("%d", &n);
		err_flag = 1;
	} while (n < 1);
	err_flag = 0;
	Process p[n];
	for (i = 0; i < n; ++i) {
		printf("\n");
		printf("Enter arrival time of process %d: ", i+1);
		scanf("%d", &p[i].arr_time);
		printf("Enter burst time of process %d: ", i+1);
		scanf("%d", &p[i].burst_time);
		p[i].p_no = i+1;
	}
	sort(&p[0], n); // Sort the processes according to the arrival time of each process.	
	while (1) {
		enqueue(p, n);
		printf("\nIn queue: ");
		for (i = 0; i <= rear; ++i) {
			printf("%d ", queue[i]->p_no);
		}
		printf("\nFront = %d, Rear = %d.\n\n", front, rear);
		execute();
		// If all the processes have been processed, break from the loop.
		if (processed == n)
			break;
	}
	return 0;
}
