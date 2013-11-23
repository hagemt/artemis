#ifndef __NUM_H__
#define __NUM_H__

#ifndef __
#define __(X) #X
#endif /* wrap */

#ifndef _
#define _(X) (X)
#endif /* gettext */

#ifndef N
#define N 16
#endif /* N */

typedef long num_t;

#include <stdio.h>

extern void
fprintb(FILE *, num_t);

#endif /* __NUM_H__ */
