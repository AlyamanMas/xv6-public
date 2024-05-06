#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
  int pid = fork();
  if (pid < 0) {
    printf(1, "Failed to fork :(\n");
  } else if (pid > 0) {
    wait();
  } else {
    setpriority(getpid(), 1);
    printptable();
    sleep(200);
    printptable();
    sleep(200);
    printptable();
  }

  exit();
}
