#include "main.h"

int
strequal(const char* str1, const char* str2)
{
  if (strlen(str1) != strlen(str2)) {
    return 0;
  } else {
    int len, i = 0;
    len = (int)strlen(str1);
    while (i < len && str1[i] == str2[i]) {
      i++;
    }
    return i == len ? 1 : 0;
  }
}

int
str_in_strtab(char* str, char** strtab, int lentab)
{
  int i;
  for (i = 0; i < lentab; i++) {
    if (strequal(str, strtab[i])) {
      return 1;
    }
  }
  return 0;
}

struct Words*
strtab2words(char** strtab, int nwords)
{
  struct Words* wordstab;
  int i;
  wordstab = (struct Words*)malloc(nwords * sizeof(struct Words));
  for (i = 0; i < nwords; i++) {
    strcpy(wordstab[i].term, strtab[i]);
  }
  return wordstab;
}

void
rm_punct(char* buf)
{
  int k = 0, leave = 0;
  while (!leave && k < MAXWDLEN && buf[k] != '\0') {
    if (is_punct(buf[k])) {
      buf[k] = '\0';
      leave = 1;
    }
    k++;
  }
}

int
is_punct(int c)
{
  int k = 0, punct_bool = 0;
  while (PUNCT[k] != '\0' && punct_bool == 0) {
    if (c == PUNCT[k]) {
      punct_bool = 1;
    } else {
      k++;
    }
  }
  return punct_bool;
}

int
wc(const char* fname)
{
  FILE* file;
  int c = 0, wordc = 0;
  file = fopen(fname, "r");
  if (file == NULL) {
    printf("wc: error opening file %s\n", fname);
    return -1;
  }
  c = fgetc(file);
  while (c != EOF) {
    if (c == ' ' || c == '\n') {
      wordc++;
      // Skipping spaces
      while (index(TOSKIP, c) != NULL) {
        c = fgetc(file);
      }
    } else {
      c = fgetc(file);
    }
  }
  fclose(file);
  return wordc;
}
