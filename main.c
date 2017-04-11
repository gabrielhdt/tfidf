#include "main.h"

// 2017 Gabriel Hondet <gabrielhondet@gmail.com>

int main(int argc, char **argv)
{
    int i;
    struct Docs* doctab;
    char** docnametab;
    //Preparing docs
    //Creating array containing names of documents
    docnametab = (char**) malloc(ART_NUM*sizeof(char*));
    for (i = 0 ; i < ART_NUM ; i++)
    {
        docnametab[i] = (char*) malloc(MAXWDLEN*sizeof(char));
        sprintf(docnametab[i], "%s/art%d.txt", ART_DIR, i);
    }
    doctab = (struct Docs*) malloc(ART_NUM*sizeof(struct Docs));
    for (i = 0 ; i < ART_NUM ; i++)
    {
        printf("Preparing doc %s...", docnametab[i]);
        doctab[i] = prepare_doc(docnametab[i]);
        printf("with %d words and %d distinct words\n", doctab[i].nwords,
               doctab[i].ndwords); 
    }
    //prepare_doc("articles/tfidf.txt");
    return 0;
}

struct Docs prepare_doc(const char* fname)
{
    int nwords, i = 0;
    struct Docs doc;
    char** termtab;
    struct Docs* docptr;
    docptr = &doc;
    strcpy(doc.fname, fname);

    // Getting number of words (based on space counts)
    nwords = wc(fname);
    doc.nwords = nwords;
    //Initialising wordlist
    termtab = (char**) malloc(nwords*sizeof(char*));
    for (i = 0 ; i < nwords ; i++)
    {
        termtab[i] = (char*) malloc(MAXWDLEN*sizeof(char));
    }
    i = 0;

    //Getting all terms
    find_terms(termtab, fname, nwords);

    //wordtab = (struct Words*) malloc(nwords*sizeof(struct Words));
    create_wordtab(termtab, docptr);

    //Setting term frequencies in doc
    for (i = 0 ; i < doc.ndwords ; i++)
    {
        doc.wordtab[i].tf = tf(doc.wordtab[i].term, termtab, doc.nwords);
    }
    return doc;
}

void find_terms(char** termtab, const char* fname, int nwords)
{
    FILE* file;
    char buf[MAXWDLEN];
    int c = ' ', i = 0, wn = 0;  // c in TOSKIP
    file = fopen(fname, "r");
    if (file == NULL)
    {
        printf("find_terms: error opening '%s'.\n", fname);
    }
    do
    {
        while (index(TOSKIP, c) != NULL)
        {
            c = fgetc(file);
        }
        while (index(WORDSEP, c) == NULL && c!= EOF)
        {
            buf[i] = c;
            i++;
            c = fgetc(file);
        }
        buf[i] = '\0';
        rm_punct(buf);
        strcpy(termtab[wn], buf);
        i = 0;
        wn++;
    } while (c != EOF && wn < nwords);
    fclose(file);
}

void create_wordtab(char** termtab, struct Docs* docptr)
    //Returns number of distinct words
{
    char** distinct_words;
    int nwords, i, wi = 0;
    struct Words* wordtab;
    nwords = docptr->nwords;
    distinct_words = (char**) malloc(nwords*sizeof(char*));
    for (i = 0 ; i < nwords ; i++)
    {
        distinct_words[i] = (char*) malloc(MAXWDLEN*sizeof(char));
    }
    for (i = 0 ; i < nwords ; i++)
    {
        if (!(str_in_strtab(termtab[i], distinct_words, nwords)))
        {
            strcpy(distinct_words[wi], termtab[i]);
            wi++;
        }
    }
    docptr->ndwords = wi;
    wordtab = (struct Words*) malloc(wi*sizeof(struct Words));
    for (i = 0 ; i < wi ; i++)
    {
        strcpy(wordtab[i].term, distinct_words[i]);
    }
    docptr->wordtab = wordtab;
}

int tf(const char* term, char** termtab, int nwords)
{
    int count = 0, i;
    for (i = 0 ; i < nwords ; i++)
    {
        if (strequal(term, termtab[i]))
        {
            count++;
        }
    }
    return count;
}

float idf(char* term, struct Docs* doctab)
{
    int j, i, count = 0;
    float idfc;
    for (i = 0 ; i < ART_NUM ; i++)
    {
        for (j = 0 ; j < doctab[i].ndwords ; j++)
        {
            if (strequal(term, doctab[i].wordtab[j].term))
            {
                count++;
            }
        }
    }
    idfc = log(((float) ART_NUM)/(1. + (float) count));
    return idfc;
}
