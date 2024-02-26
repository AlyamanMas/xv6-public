#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFFER_SIZE 512
// If debug is defined, the program will print debug information
//#define DEBUG

typedef struct
{
  uint number_of_lines;
  char* file_name;
} Config;

typedef struct buffer_node_t
{
  struct buffer_node_t* next;
  char buffer[BUFFER_SIZE];
} BufferNode;

// Should own `data` to avoid its data being freed by the actual owner of the
// data
typedef struct line_node_t
{
  struct line_node_t* next;
  char* data;
} LineNode;

uint
count_lines(char* buffer, uint size)
{
  uint count = 0;
  for (uint i = 0; i < size; i++) {
    if (buffer[i] == '\n') {
      count++;
    }
  }
  return count;
}

void
dbg_buffer_list(BufferNode* head)
{
#ifdef DEBUG
  BufferNode* current = head;
  while (current != 0) {
    printf(1,
           "BufferNode { lines_in_buffer: %d, strlen: %d, next: %d }\n",
           count_lines(current->buffer, BUFFER_SIZE),
           strlen(current->buffer),
           (unsigned long long)current->next);
    current = current->next;
  }
#endif
}

void
dbg_line_list(LineNode* head)
{
#ifdef DEBUG
  LineNode* current = head;
  while (current != 0) {
    printf(1,
           "LineNode {\n  data: %s,\n  has_new_line: %s,\n  strlen: %d,\n  "
           "next: %d\n}\n",
           current->data,
           current->data[strlen(current->data) - 1] == '\n' ? "true" : "false",
           strlen(current->data),
           (unsigned long long)current->next);
    current = current->next;
  }
#endif
}

Config
parse_args(int argc, char* argv[])
{
  Config config;
  config.number_of_lines = 5;
  config.file_name = "";
  if (argc == 1) {
    printf(1, "tail: missing file operand\n");
    exit();
  } else if (argc == 2) {
    config.file_name = argv[1];
  } else if (argc == 3) {
    config.number_of_lines = atoi(argv[1]);
    config.file_name = argv[2];
  } else {
    printf(1, "tail: too many arguments\n");
    printf(1, "Usage: tail [number_of_lines] file_name\n");
    exit();
  }
  return config;
}

BufferNode*
reverse_list(BufferNode* head)
{
  BufferNode* prev = 0;
  BufferNode* current = head;
  BufferNode* next = 0;
  while (current != 0) {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  return prev;
}

LineNode*
reverse_lines_list(LineNode* head)
{
  LineNode* prev = 0;
  LineNode* current = head;
  LineNode* next = 0;
  while (current != 0) {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  return prev;
}

void
delete_after_n(LineNode* head, int n)
{
  if (head == 0 || n <= 0) {
    return;
  }

  LineNode* current = head;
  int count = 1;
  while (current != 0 && count < n) {
    current = current->next;
    count++;
  }
  if (current == 0) {
    return;
  }
  LineNode* temp = current->next;
  current->next = 0;

  // Free the rest of the memory
  while (temp != 0) {
    LineNode* next_node = temp->next;
    free(temp->data);
    free(temp);
    temp = next_node;
  }
}

char*
strcat(char* destination, const char* source)
{
  char* ptr = destination + strlen(destination);
  while (*source != '\0') {
    *ptr++ = *source++;
  }
  *ptr = '\0';
  return destination;
}

LineNode*
merge_lines(LineNode* head)
{
  LineNode* current = head;
  LineNode* prev = 0;
  LineNode* merged_head = 0;
  LineNode* merged_current = 0;

  while (current != 0) {
    if (prev != 0 && prev->data[strlen(prev->data) - 1] != '\n') {
      unsigned long long prev_len = strlen(prev->data);
      unsigned long long current_len = strlen(current->data);
      unsigned long long total_len = prev_len + current_len + 1;
      char* merged_data = (char*)malloc(total_len);
      if (merged_data == 0) {
        printf(1, "Memory allocation failed\n");
        exit();
      }
      strcpy(merged_data, prev->data);
      strcat(merged_data, current->data);

      free(prev->data);
      prev->data = merged_data;
      prev->next = current->next;
      free(current);
      current = prev;
    } else {
      // If the previous node and the current node do not form a complete line,
      // simply update the merged list pointers
      if (merged_head == 0) {
        merged_head = current;
        merged_current = current;
      } else {
        merged_current->next = current;
        merged_current = merged_current->next;
      }
    }

    prev = current;
    current = current->next;
  }

  // Terminate the merged list
  if (merged_current != 0) {
    merged_current->next = 0;
  }

  return merged_head;
}

LineNode*
from_buffer_to_line(BufferNode* list)
{
  LineNode* head = 0;
  LineNode** current = &head;
  while (list != 0) {
    uint current_line_start = 0;
    for (uint i = 0; i < strlen(list->buffer); ++i) {
      if (*current == 0) {
        *current = (LineNode*)malloc(sizeof(LineNode));
      }
      if (list->buffer[i] == '\n') {
        (*current)->data = malloc(i - current_line_start + 2);
        memmove((*current)->data,
                list->buffer + current_line_start,
                i - current_line_start + 1);
        (*current)->data[i - current_line_start + 1] = '\0';
        current_line_start = i + 1;
        current = &(*current)->next;
      }
    }
    if (current_line_start < strlen(list->buffer)) {
      if (*current == 0) {
        *current = (LineNode*)malloc(sizeof(LineNode));
      }
      (*current)->data = malloc(strlen(list->buffer) - current_line_start + 2);
      memmove((*current)->data,
              list->buffer + current_line_start,
              strlen(list->buffer) - current_line_start + 1);
      (*current)->data[strlen(list->buffer) - current_line_start] = 0;
    }
    current = &(*current)->next;
    list = list->next;
  }
  return merge_lines(head);
}

BufferNode*
construct_list(int fd, uint lines_required)
{
  BufferNode* head = 0;
  BufferNode** current = &head;
  uint lines_so_far = 0;

  lseek(fd, 0, SEEK_END);

  for (uint i = 1; lines_so_far < lines_required; ++i) {
    int offset = lseek(fd, -BUFFER_SIZE * i, SEEK_END);
    uint correction = 0;
    if (offset < 0) {
      correction = offset;
      lseek(fd, 0, SEEK_SET);
    }
#ifdef DEBUG
    printf(1, "offset: %d\n", offset);
#endif

    if (*current == 0) {
      *current = (BufferNode*)malloc(sizeof(BufferNode));
    }

    read(fd, (*current)->buffer, BUFFER_SIZE + correction);
    lines_so_far += count_lines((*current)->buffer, BUFFER_SIZE + correction);
    current = &(*current)->next;
    if (offset <= 0) {
      break;
    }
  }

  return head;
}

void
print_list(BufferNode* tail)
{
  BufferNode* current = tail;
  while (current != 0) {
    printf(1, "%s", current->buffer);
    current = current->next;
  }
}

void
print_lines(LineNode* head)
{
  LineNode* current = head;
  while (current != 0) {
    printf(1, "%s", current->data);
    current = current->next;
  }
}

int
main(int argc, char* argv[])
{
#ifdef DEBUG
  printf(1, "head\n");
#endif
  Config config = parse_args(argc, argv);
  int fd = open(config.file_name, O_RDONLY);
#ifdef DEBUG
  printf(1, "head: %s\n", config.file_name);
#endif

  if (fd < 0) {
    printf(1, "head: cannot open %s\n", config.file_name);
    exit();
  }

#ifdef DEBUG
  printf(1, "head: %d\n", config.number_of_lines);
#endif
  BufferNode* head = construct_list(fd, config.number_of_lines);
  dbg_buffer_list(head);
  BufferNode* tail = reverse_list(head);
  dbg_buffer_list(tail);
  LineNode* lines = from_buffer_to_line(tail);
  LineNode* reversed = reverse_lines_list(lines);
  delete_after_n(reversed, config.number_of_lines);
  LineNode* final = reverse_lines_list(reversed);
  dbg_line_list(final);
  print_lines(final);
#ifdef DEBUG
  printf(1, "tail: done\n");
#endif

  exit();
}
