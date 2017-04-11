#ifndef HEADER_FILE
#define HEADER_FILE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define ART_DIR "articles"
#define ART_NUM 20
#define MAXWDLEN 100
#define PUNCT ".,?;:!()\""
#define TOSKIP " \n()\"^*"
#define WORDSEP " \n"

//main.c
struct Docs prepare_doc(const char*);
int tf(const char* term, char** termtab, int nwords);
float idf(char* term, struct Docs* doctab);
void find_terms(char**, const char*, int);
void create_wordtab(char**, struct Docs*);
void complete_idfs(struct Docs* doctab);
int tfidf_cmp(const void *, const void *);
void sort_wordtab(struct Docs doc);
//utils.c
int str_in_strtab(char* str, char** strtab, int lentab);
int strequal(const char*, const char*);
int wc(const char*);
int is_punct(int);
void rm_punct(char*);

struct Words {
    char term[MAXWDLEN];
    int tf;
    float idf;
} word;

struct Docs {
    char fname[MAXWDLEN];
    struct Words* wordtab;
    int nwords;
    int ndwords;  // Distinct words. More useful than nwords
} doc;
#endif
