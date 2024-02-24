#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"

int
main(int argc, char* argv[])
{
  struct rtcdate* r;
  gettime(r);

  printf(1,
         "%d-%d-%dT%d:%d:%d\n",
         r->year,
         r->month,
         r->day,
         r->hour,
         r->minute,
         r->second);

  exit();
}
