#define NSHMENT 10
#define SHMEMPTY -1
#define SHM_CREAT 0x01
#define SHM_FETCH 0x02
#define SHM_EXIST (SHM_CREAT | SHM_FETCH)
#define SHM_RND 0x01
#define SHM_RDONLY 0x02

struct shment{
    int key, refcount, size;
    char *shmemory;
};

extern struct shment shmtab[];
extern int16 curr_size;

char *shmat(int16 ,char *, int16);
char *shmdt(int16 ,int16);
uint16 shmget(int16, uint32, int16);

int writer(sid32, sid32, char*);
int reader(sid32, sid32);