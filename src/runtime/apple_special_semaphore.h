#include <errno.h>
#include <pthread.h>

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    unsigned count;
} fake_sem_t;

class AppleSemaphore {
    public:
        static int fake_sem_init(fake_sem_t* psem, int flags, unsigned count);
        static int fake_sem_destroy(fake_sem_t *psem);
        static int fake_sem_post(fake_sem_t *pxsem);
        static int fake_sem_wait(fake_sem_t *pxsem);
        static int fake_sem_trywait(fake_sem_t *pxsem);
        static int fake_sem_timedwait(fake_sem_t *pxsem, const struct timespec *abstim);
};