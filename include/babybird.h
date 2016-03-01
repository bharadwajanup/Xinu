extern void parentbird();
extern void childbird(int,int);




struct qarray
{
	volatile int q[NPROC];
	volatile int head;
	volatile int tail;
};
typedef struct qarray qarray;

struct mutex_t
{
	volatile int value;
	volatile int test_and_set_lock;
	volatile qarray q;
};

struct cond_t
{
	volatile int value;
	volatile int test_and_set_lock;
	volatile qarray q;
};


typedef struct mutex_t mutex_t;
typedef struct cond_t cond_t;



