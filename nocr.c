
#include "nocr.h"

int nocr(char *line) {

  while (line[0]) {
    if (line[0] == '\r' || line[0] == '\n') {
      line[0] = 0;
    }
    line++;
  }

  return 0;

}
