#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "misc.h"
#include "hubert.h"

void _fatal(char* errorMessage) {
  if (errno == EINTR) {
    // There was a signal so we need to exit, but it's not en error
    exit(EXIT_SUCCESS);
  }
  if (errno) {
    perror(errorMessage);
    if (errno == ENOENT) {
      fprintf(stderr, "Is %s running?\n", HUBERT_NAME);
    }
    exit(EXIT_FAILURE);
  } else {
    fprintf(stderr, "%s\n", errorMessage);
  }
}

void _warning(char* errorMessage) {
  if (errno) {
    perror(errorMessage);
  } else {
    fprintf(stderr, "%s\n", errorMessage);
  }
}
