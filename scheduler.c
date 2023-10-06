#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>



// TODO: Add more fields to this struct
struct job {
    int id;
    int arrival;
    int length;
    struct job *next;
};

/*** Globals ***/ 
int seed = 100;

//This is the start of our linked list of jobs, i.e., the job list
struct job *head = NULL;

/*** Globals End ***/

/*Function to append a new job to the list*/
void append(int id, int arrival, int length){
  // create a new struct and initialize it with the input data
  struct job *tmp = (struct job*) malloc(sizeof(struct job));

  //tmp->id = numofjobs++;
  tmp->id = id;
  tmp->length = length;
  tmp->arrival = arrival;

  // the new job is the last job
  tmp->next = NULL;

  // Case: job is first to be added, linked list is empty 
  if (head == NULL){
    head = tmp;
    return;
  }

  struct job *prev = head;

  //Find end of list 
  while (prev->next != NULL){
    prev = prev->next;
  }

  //Add job to end of list 
  prev->next = tmp;
  return;
}


/*Function to read in the workload file and create job list*/
void read_workload_file(char* filename) {
  int id = 0;
  FILE *fp;
  size_t len = 0;
  ssize_t read;
  char *line = NULL,
       *arrival = NULL, 
       *length = NULL;

  struct job **head_ptr = malloc(sizeof(struct job*));

  if( (fp = fopen(filename, "r")) == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) > 1) {
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


void policy_FIFO(struct job *head, char *output_filename) {
  int x = 0;

  FILE *output_file = fopen(output_filename, "w");
  if (output_file == NULL) {
    fprintf(stderr, "Error: Could not open output file.\n");
    return;
  }
  fprintf(output_file, "Execution trace with FIFO:\n");
  while (head != NULL) {
    fprintf(output_file, "t= %d: [Job %d] arrived at [%d], ran for: [%d]\n", x, head->id, head->arrival, head->length);
    x = x + head->length;
    head = head->next;
  }
  fprintf(output_file, "End of execution with FIFO.\n");

  fclose(output_file);
}



void analyze_FIFO(struct job *head, char *output_filename) {

  int y = 0;
  double resTime = 0;
  double turnTime = 0;
  double waitTime = 0;
  double i = 0;

  FILE *output_file = fopen(output_filename, "a");
  if (output_file == NULL) {
    fprintf(stderr, "Error: Could not open output file.\n");
    return;
  }

  fprintf(output_file, "Begin analying FIFO.\n");
  while (head != NULL) {
    resTime += y;
    turnTime += y+head->length;
    waitTime += y;
    i++;
    fprintf(output_file, "Job %d -- Response time: %d Turnaround: %d Wait: %d\n", head->id, y, y+head->length, y);
    y = y + head->length;
    head = head->next;
  }
  fprintf(output_file, "Average -- Response: %.2f Turnaround %.2f Wait %.2f\n", (resTime/ i), (turnTime/i), (waitTime/i));
  fprintf(output_file, "End analying FIFO.\n");

  fclose(output_file);
}

void policy_SJF(struct job *head) {
  // TODO: Fill this in

  return;
}

void analyze_SJF(struct job *head) {
  // TODO: Fill this in

  return;
}

int main(int argc, char **argv) {

 if (argc < 5) {
    fprintf(stderr, "missing variables\n");
    fprintf(stderr, "usage: %s analysis-flag policy workload-file\n", argv[0]);
		exit(EXIT_FAILURE);
  }

  int analysis = atoi(argv[1]);
  char *policy = argv[2],
       *workload = argv[3];
  char *output_filename = argv[4];
  // Note: we use a global variable to point to 
  // the start of a linked-list of jobs, i.e., the job list  
  read_workload_file(workload);

  if (strcmp(policy, "FIFO") == 0 ) {
    policy_FIFO(head, output_filename);
    if (analysis) {
      printf("Begin analyzing FIFO:\n");
      analyze_FIFO(head, output_filename);
      printf("End analyzing FIFO.\n");
    }

    exit(EXIT_SUCCESS);
  }
  else if (strcmp(policy, "SJF") == 0){
    policy_SJF(head);
    if (analysis) {
      printf("Begin analyzing SJF:\n");
      analyze_SJF(head);
      printf("End analyzing SJF.\n");
    }
    exit(EXIT_SUCCESS);
  }

	exit(EXIT_SUCCESS);
}
