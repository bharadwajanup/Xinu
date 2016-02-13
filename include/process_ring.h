#define NUM_PROCESSES 4

volatile int processes[NUM_PROCESSES];
//int processes[NUM_PROCESSES];
volatile int master;

int pid[NUM_PROCESSES];

extern void process_ring(int, int);

extern void process_ring_infinite(int, int);

extern void process_ring_out(int, int);

extern void process_ring_hang(int, int);
