
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include <dirent.h>

#include "critbit.h"

#include "nocr.h"

int write_critlines(const char *str, void *extra) {

  FILE *fpout;

  fpout = (FILE*) extra;

  if (str != NULL) {

    fprintf(fpout, "%s\n", str);

    return 1;
    
  }
  
  return 0;
  
}

int main(int argc, char *argv[]) {

  char crit_fn[240];

  int retval;

  char *input_dir;
  char *output_dir;

  FILE *fp;

  critbit0_tree critlist;

  size_t len;
  char *line;
  ssize_t bytes_read;

  input_dir = argc>1 ? argv[1] : NULL;
  output_dir = argc>2 ? argv[2] : NULL;

  if (input_dir == NULL || output_dir == NULL) {
    printf("Please specify input and output directories.\n");
    return -1;
  }
  
  retval = sprintf(crit_fn, "%s/critlist.txt", output_dir);

  critlist = (critbit0_tree) { .root = NULL };

  line = NULL;
  len = 0;

  fp = fopen(crit_fn, "r");
  if (fp != NULL) {
  
    while (( bytes_read = getline(&line, &len, fp) >= 0)) {

      if (len <= 0) {
	continue;
      }

      retval = nocr(line);
      
      critbit0_insert(&critlist, line);

    }

    retval = fclose(fp);
    if (retval == -1) {
      perror("fclose");
      return -1;
    }
    
  }
  
  {

    DIR *dirp;

    struct dirent *rdp;

    char copy_cmd[1024];
    
    dirp = opendir(input_dir);
    if (dirp == NULL) {
      perror("opendir");
      return -1;
    }

    while ((rdp = readdir(dirp)) != NULL) {

      if (rdp->d_name[0] == '.') {
	continue;
      }

      if (!strncmp(rdp->d_name, "critlist.txt", 12)) {
	continue;
      }
      
      printf("Scanning %s\n", rdp->d_name);
      
      retval = critbit0_contains(&critlist, rdp->d_name);

      if (0 == retval) {

	memset(copy_cmd, 0, sizeof(copy_cmd));
	
	retval = sprintf(copy_cmd, "cp %s/%s %s/%s.eml", input_dir, rdp->d_name, output_dir, rdp->d_name);

	retval = system(copy_cmd);

	if (retval != 0) {
	  printf("Trouble with system cp command.\n");
	  return -1;
	}
	
	retval = critbit0_insert(&critlist, rdp->d_name);
	
      }
      

      
    }
    
    retval = closedir(dirp);
    if (retval == -1) {
      perror("closedir");
      return -1;
    }

  }

  {

    char out_fn[240];

    FILE *fpout;
    
    retval = sprintf(out_fn, "%s/%s", output_dir, "outfile_critlist-new.txt");

    fpout = fopen(out_fn, "w");
    if (fpout == NULL) {
      perror("fopen");
      return -1;
    }

    retval = critbit0_allprefixed(&critlist, "", write_critlines, fpout);

    retval = fclose(fpout);
    if (retval == -1) {
      perror("fclose");
      return -1;
    }

    retval = rename(out_fn, crit_fn);
    if (retval == -1) {
      perror("rename");
      return -1;
    }
    
  }
  
  critbit0_clear(&critlist);
  
  return 0;

}
