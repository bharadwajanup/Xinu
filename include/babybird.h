extern void parentbird();
extern void childbird(int,int);



struct mutex_t
{
	volatile int value;
	int test_and_set_lock;
};

struct cond_t
{
	volatile int value;
	int test_and_set_lock;
};

typedef struct mutex_t mutex_t;
typedef struct cond_t cond_t;


