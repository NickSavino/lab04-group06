#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

// TODO: Add more fields to this struct
struct job
{
  int id;
  int arrival;
  int length;
  struct job *next;
};

/*** Globals ***/
int seed = 100;

// This is the start of our linked list of jobs, i.e., the job list
struct job *head = NULL;

/*** Globals End ***/

/*Function to append a new job to the list*/
void append(int id, int arrival, int length)
{
  // create a new struct and initialize it with the input data
  struct job *tmp = (struct job *)malloc(sizeof(struct job));

  // tmp->id = numofjobs++;
  tmp->id = id;
  tmp->length = length;
  tmp->arrival = arrival;

  // the new job is the last job
  tmp->next = NULL;

  // Case: job is first to be added, linked list is empty
  if (head == NULL)
  {
    head = tmp;
    return;
  }

  struct job *prev = head;

  // Find end of list
  while (prev->next != NULL)
  {
    prev = prev->next;
  }

  // Add job to end of list
  prev->next = tmp;
  return;
}

/*Function to read in the workload file and create job list*/
void read_workload_file(char *filename)
{
  int id = 0;
  FILE *fp;
  size_t len = 0;
  ssize_t read;
  char *line = NULL,
       *arrival = NULL,
       *length = NULL;

  struct job **head_ptr = malloc(sizeof(struct job *));

  if ((fp = fopen(filename, "r")) == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) > 1)
  {
    arrival = strtok(line, ",\n");
    length = strtok(NULL, ",\n");

    // Make sure neither arrival nor length are null.
    assert(arrival != NULL && length != NULL);

    append(id++, atoi(arrival), atoi(length));
  }

  fclose(fp);

  // Make sure we read in at least one job
  assert(id > 0);

  return;
}

// for FIFO, each line is one input: x, y. x = arival time, y = duration
// example output: t=0: [Job 0] arrived at [0], ran for: [3]
//            curr time  job#     arival time     duration

void policy_FIFO(struct job *head)
{
  printf("Execution trace with FIFO:\n");
  struct job *temp = head;
  int currtime = 0;
  while (temp != NULL)
  {
    printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", currtime, temp->id, temp->arrival, temp->length);
    currtime += temp->length;
    temp = temp->next;
  }
  printf("End of execution with FIFO.\n");
  return;
}

// response time = start time - arival time
// turnaround time = completion time - arrival time
// output = Job 0 -- Response time: 0  Turnaround: 3  Wait: 0
void analyze_FIFO(struct job *head)
{
  struct job *temp = head;
  int response_t = 0;
  int turnaround_t = 0;
  int start_t = 0;
  double total_r = 0.0;
  double total_t = 0.0;

  int job_count = 0;
  while (temp != NULL)
  {
    response_t = start_t - temp->arrival;
    turnaround_t = start_t + temp->length - temp->arrival;
    printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n", temp->id, response_t, turnaround_t, response_t);
    start_t += temp->length;
    total_r += response_t;
    total_t += turnaround_t;
    job_count++;
    temp = temp->next;
  }
  double av_r = total_r / job_count;
  double av_t = total_t / job_count;
  printf("Average -- Response: %.2lf  Turnaround %.2lf  Wait %.2lf", av_r, av_t, av_r);
  return;
}

void policy_SJF(struct job *head)
{
  // TODO: Fill this in

  return;
}

void analyze_SJF(struct job *head)
{
  // TODO: Fill this in

  return;
}

//////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

  if (argc < 4)
  {
    fprintf(stderr, "missing variables\n");
    fprintf(stderr, "usage: %s analysis-flag policy workload-file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int analysis = atoi(argv[1]);
  char *policy = argv[2],
       *workload = argv[3];

  // Note: we use a global variable to point to
  // the start of a linked-list of jobs, i.e., the job list
  read_workload_file(workload);

  if (strcmp(policy, "FIFO") == 0)
  {
    policy_FIFO(head);
    if (analysis)
    {
      printf("Begin analyzing FIFO:\n");
      analyze_FIFO(head);
      printf("\nEnd analyzing FIFO.\n");
    }

    exit(EXIT_SUCCESS);
  }

  else if (strcmp(policy, "SJF") == 0)
  {
    int a = 0;
  }
  // TODO: Add other policies

  exit(EXIT_SUCCESS);
}
