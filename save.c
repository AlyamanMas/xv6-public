#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void
save(int fd, char* text)
{
  uint n = strlen(text);
  if (write(fd, text, n) != n) {
    printf(1, "save: write error\n");
    exit();
  }
}

int
main(int argc, char* argv[])
{
  int fd, i;
  char* string;

  if (argc <= 2) {
    printf(1, "Usage: save DEST STRING\n");
    exit();
  }

  // Delete file before writing to it.
  // This is because writing n bytes to a file with m bytes where m > n will
  // only overwrite the first n bytes and leave the last m - n bytes untouched
  unlink(argv[1]);

  // Create the file if it doesn't exist.
  // Open it in read write mode
  if ((fd = open(argv[1], O_CREATE | O_WRONLY)) < 0) {
    printf(1, "save: cannot open %s\n", argv[1]);
    exit();
  }

  save(fd, argv[2]);

  exit();
}
