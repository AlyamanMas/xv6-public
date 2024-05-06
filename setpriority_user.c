#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int priority, pid;
  if(argc < 3){
    exit();
  }
  pid = atoi(argv[1]);
  priority = atoi(argv[2]);
  setpriority(pid, priority);
  exit();
}
