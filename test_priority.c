#include "types.h"
#include "user.h"

int
main(void)
{
  int i, pid;

  // Create 3 processes with different priorities
  for (i = 0; i < 3; i++) {
    pid = fork();
    if (pid != 0) {
      setpriority(pid, 5 - i); // the parent set the priority of the child
      printptable();
    }

    if (pid == 0) { // child process
      int j;
      for (j = 0; j < 1000000; j++) {
        // Do nothing
      }
      exit();
    }
  }

  // Wait for all child processes to complete
  for (i = 0; i < 3; i++) {
    wait();
  }

  printf(1, "Finished all processes.\n");
  exit();
}
