#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
  int pid;
  int y;

  y = 0;
  pid = 0;

  for (int i = 0; i < 3; i++) {
    pid = fork();

    if (pid < 0) {
      printf(1, "%d Failed to Create Child!\n", getpid());
    } else if (pid > 0) // if parent
    {
      printf(1, "Process %d Created Child %d\n", getpid(), pid);
    } else // if child
    {
      printf(1, "Child: %d is created\n", getpid());
      printptable();
      sleep(200);
      exit();
    }
  }
  for (int i = 0; i < 3; ++i) {
    wait();
  }
  exit();
}
