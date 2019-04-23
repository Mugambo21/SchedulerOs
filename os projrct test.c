#include <stdio.h>
#include<windows.h>
#define SIZE 100
typedef struct Processes {
	int p_no;
	int arr_time;
	int burst_time;
	int comp_time;
	int wait_time;
	int turn_time;
	int bt;
} Prs;
Prs * queue[SIZE];
int front = 0, rear = -1, processed = 0, curr_time = 0, tq = 6;
int last_front = 0, last_rear = -1;
void enqueue(Prs p[], int n) {
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
void swap(Prs * x, Prs * y) {
	Prs temp = * x;
	* x = * y;
	* y = temp;
}
void sort(Prs p[], int n) {
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

void execute() {
	
	
	int i;
	
	if (front-1 == rear) {
		printf("CPU idle for 1 second.\n");
		curr_time += 1;
	}
	else {
		last_front = front;
		last_rear = rear;		
		for (i = front; i <= rear; ++i, ++front)
		{
			if (queue[i]->burst_time > tq)
			{
				queue[i]->burst_time -= tq;
				curr_time += tq;
				printf("Process number %d excuted till %d seconds.\n", queue[i]->p_no, curr_time);
			}
			else if (queue[i]->burst_time > 0)
			{
				curr_time += queue[i]->burst_time;
				queue[i]->burst_time = 0;
				printf("Process number %d excuted till %d seconds.\n", queue[i]->p_no, curr_time);
				queue[i]->comp_time = curr_time;
				queue[i]->turn_time = queue[i]->comp_time - queue[i]->arr_time;
				queue[i]->wait_time = queue[i]->turn_time - queue[i]->bt;
				//printf(" Turnaround time of process number %d - %d\n",queue[i]->p_no,queue[i]->turn_time);
				//printf(" Waiting time of process number %d - %d\n\n",queue[i]->p_no,queue[i]->wait_time);
				++processed;
			}
		}
	}
}
int main() {
	
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole,11);
	
	printf("\t\t\t\t\t\tSCHEDULER");
	printf("\n\t\t==========================================================================================\n");
	
	
	
	int sum_wait = 0, sum_turn = 0;
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
	Prs p[n];
	for (i = 0; i < n; ++i) {
		printf("\n");
		printf("Enter arrival time of process %d: ", i+1);
		scanf("%d", &p[i].arr_time);
		printf("Enter burst time of process %d: ", i+1);
		scanf("%d", &p[i].burst_time);
		p[i].bt = p[i].burst_time;
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
	printf(" =========================================================================================================");
	printf("\n |   Process no.     |    Arrival Time    |    Burst Time    |    Turnaround Time   |    Waiting Time    |\n");
	
	for(i = 0; i < n; i++)
	{
		printf(" |         %d         |         %d          |        %d         |          %d           |         %d          |\n",queue[i]->p_no,queue[i]->arr_time,queue[i]->bt,queue[i]->turn_time,queue[i]->wait_time);
		sum_wait += queue[i]->wait_time;
		sum_turn += queue[i]->turn_time;
	}
    printf(" =========================================================================================================");
		
	printf("\nAverage Turnaround time: %f",(float)sum_turn/n);
	printf("\nAverage Waiting time: %f",(float)sum_wait/n);
	printf("\n =========================================================================================================");
	return 0;
}
