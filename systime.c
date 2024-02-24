#include "types.h"
#include "date.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"

int
sys_gettime(void)
{
  struct rtcdate* r;
  if (argptr(0, (void*)&r, sizeof(struct rtcdate)) < 0)
    return -1;
  cmostime(r);
  return 0;
}
