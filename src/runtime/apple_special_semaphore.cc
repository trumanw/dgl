#include "apple_special_semaphore.h"

int AppleSemaphore::fake_sem_init(fake_sem_t* psem, int flags, unsigned count) {
    fake_sem_t pnewsem;
    int result;
    result = pthread_mutex_init(&pnewsem.mutex, NULL);
    if (result)
        return result;
    result = pthread_cond_init(&pnewsem.cond_var, NULL);
    if (result) {
        pthread_mutex_destroy(&pnewsem.mutex);
        return result;
    }
    pnewsem.count = count;
    *psem = pnewsem;
    return 0;
}

int AppleSemaphore::fake_sem_destroy(fake_sem_t *psem) {
    if (!psem)
        return EINVAL;
    pthread_mutex_destroy(&psem->mutex);
    pthread_cond_destroy(&psem->cond_var);
    return 0;
}

int AppleSemaphore::fake_sem_post(fake_sem_t *pxsem) {
    if (!pxsem)
        return EINVAL;
    int result, xresult;
    result = pthread_mutex_lock(&pxsem->mutex);
    if (result)
        return result;
    pxsem->count = pxsem->count + 1;
    xresult = pthread_cond_signal(&pxsem->cond_var);
    result = pthread_mutex_unlock(&pxsem->mutex);
    if (result)
        return result;
    if (xresult) {
        errno = xresult;
        return -1;
    }
    return 0;
}

int AppleSemaphore::fake_sem_wait(fake_sem_t *pxsem) {
    int result, xresult;
    if (!pxsem)
        return EINVAL;
    result = pthread_mutex_lock(&pxsem->mutex);
    if (result)
        return result;
    xresult = 0;
    if (pxsem->count == 0) {
        xresult = pthread_cond_wait(&pxsem->cond_var, &pxsem->mutex);
    }
    if (!xresult) {
        if (pxsem->count > 0) {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->mutex);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int AppleSemaphore::fake_sem_trywait(fake_sem_t *pxsem) {
    int result, xresult;
    if (!pxsem)
        return EINVAL;
    result = pthread_mutex_lock(&pxsem->mutex);
    if (result)
        return result;
    xresult = 0;
    if (pxsem->count > 0)
        pxsem->count--;
    else
        xresult = EAGAIN;
    result = pthread_mutex_unlock(&pxsem->mutex);
    if (result)
        return result;
    if (xresult) {
        errno = xresult;
        return -1;
    }
    return 0;
}

int AppleSemaphore::fake_sem_timedwait(fake_sem_t *pxsem, const struct timespec *abstim) {
    int result, xresult;
    if (!pxsem)
        return EINVAL;
    result = pthread_mutex_lock(&pxsem->mutex);
    if (result)
        return result;
    xresult = 0;

    if (pxsem->count == 0) {
        xresult = pthread_cond_timedwait(&pxsem->cond_var, &pxsem->mutex, abstim);
    }
    if (!xresult && pxsem->count > 0) {
        pxsem->count--;
    }
    result = pthread_mutex_unlock(&pxsem->mutex);
    if (result) {
        return result;
    }
    if (xresult) {
        errno = xresult;
        return -1;
    }
    return 0;
}