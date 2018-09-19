#ifndef _LINUX_TIME64_H
#define _LINUX_TIME64_H

/* Vigor: Define just enough to compile. */

#include <uapi/linux/time.h>

#define MSEC_PER_SEC	1000L
#define USEC_PER_MSEC	1000L
#define NSEC_PER_USEC	1000L
#define NSEC_PER_MSEC	1000000L
#define USEC_PER_SEC	1000000L
#define NSEC_PER_SEC	1000000000L
#define FSEC_PER_SEC	1000000000000000LL

#endif /* _LINUX_TIME64_H */
