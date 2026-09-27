#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <dlfcn.h>
#include <errno.h>

/*
 * Under strict snap confinement, snapd's seccomp filter restricts sched_setaffinity
 * to pid 0 (the calling thread/process). When multi-threaded runtimes like AMD ROCm /
 * HIP spawn worker threads and attempt to pin them with pthread_setaffinity_np() or
 * sched_setaffinity(), the kernel/seccomp returns EPERM unless the privileged
 * process-control plug is manually connected.
 *
 * This wrapper intercepts pthread_setaffinity_np and sched_setaffinity. If the real
 * system call returns EPERM, it swallows the error and returns 0 (success) so the
 * runtime can proceed normally using inherited process affinity instead of aborting.
 */

typedef int (*real_pthread_setaffinity_np_fn)(pthread_t, size_t, const cpu_set_t *);
typedef int (*real_sched_setaffinity_fn)(pid_t, size_t, const cpu_set_t *);

int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t *cpuset) {
    static real_pthread_setaffinity_np_fn real_fn = NULL;
    if (!real_fn) {
        real_fn = (real_pthread_setaffinity_np_fn)dlsym(RTLD_NEXT, "pthread_setaffinity_np");
    }
    if (real_fn) {
        int ret = real_fn(thread, cpusetsize, cpuset);
        if (ret == EPERM) {
            return 0;
        }
        return ret;
    }
    return 0;
}

int sched_setaffinity(pid_t pid, size_t cpusetsize, const cpu_set_t *mask) {
    static real_sched_setaffinity_fn real_fn = NULL;
    if (!real_fn) {
        real_fn = (real_sched_setaffinity_fn)dlsym(RTLD_NEXT, "sched_setaffinity");
    }
    if (real_fn) {
        int ret = real_fn(pid, cpusetsize, mask);
        if (ret == -1 && errno == EPERM) {
            errno = 0;
            return 0;
        }
        return ret;
    }
    return 0;
}
