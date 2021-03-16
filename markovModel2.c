#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "markovModel2.h"

char NONWORD[] = "\n"; // a sign word for termination

unsigned int hash(char *s[NPREF]) {
    unsigned int h = 0;
    unsigned char *p;
    for (int i = 0; i < NPREF; i++)
        for (p = (unsigned char *) s[i]; *p != '\0'; p++)
            h = MULTIPLIER * h + *p;
    return h % NHASH;
}

unsigned int hashWord(char *s) {
    unsigned int h = 0;
    unsigned char *p;
    for (p = (unsigned char *) s; *p != '\0'; p++)
        h = MULTIPLIER * h + *p;
    return h % NHASH;
}

char* lookupWord(char *s, int create) {
    Word *wp;
    int h;

    h = hashWord(s);
    wp = wordtab[h];

    for (wp = wordtab[h]; wp != NULL; wp = wp->next)
        if (strcmp(wp->word, s) == 0)
            return wp->word;

    if (create) {
        wp = (Word *) malloc(sizeof(Word));
        if (wp == NULL)
            printf("Cannot malloc memory for the word");
        wp->word = s;
        wp->next = wordtab[h];
        wordtab[h] = wp;
    }
    return wp != NULL ? wp->word : NULL;
}

State* lookup(char *prefix[NPREF], int create) {
    State *sp;
    int h, i;

    h = hash(prefix);
    sp = statetab[h];

    for (sp = statetab[h]; sp != NULL; sp = sp->next) {
        for (i = 0; i < NPREF; i++)
            if (sp->pref[i] != prefix[i]) // unique strings have unique addresses, so just enough to compare pointer values
                break;
        if (i == NPREF) { // we found it
            return sp;
        }
    }

    if (create) {
        sp = (State *) malloc(sizeof(State));
        if (sp == NULL)
            printf("Cannot alloc memmory for the state");
        for (i = 0; i < NPREF; i++)
            sp->pref[i] = prefix[i];
        sp->suf = NULL;
        sp->next = statetab[h];
        statetab[h] = sp;
    }
    return sp;
}

void build(char *prefix[NPREF], FILE *file) {
    char buff[100], fmt[100], *word;
    sprintf(fmt, "%%%lus", sizeof(buff) - 1);
    while(fscanf(file, fmt, buff) != EOF) {
        word = lookupWord(strdup(buff), 1);
        add(prefix, word);
    }
}

void add(char *prefix[NPREF], char *suffix) {
    State *sp;
    sp = lookup(prefix, 1);
    addSuffix(sp, suffix);
    // move prefix down 1 index in the array
    memmove(prefix, prefix + 1, (NPREF -1 ) * sizeof(prefix[0]));
    prefix[NPREF - 1] = suffix;
}

void addSuffix(State *sp, char *suffix) {

    Suffix *suf = (Suffix *) malloc(sizeof(Suffix));
    if (suf == NULL)
        printf("Cannot malloc to create a suffix");
    suf->word = suffix;
    suf->next = sp->suf;
    sp->suf = suf;
}

void generate(int nwords) {
    State *sp;
    Suffix *suf;
    char *prefix[NPREF], *w;
    int i, nmatch;

    for (i = 0; i < NPREF; i ++)
        prefix[i] = NONWORD;

    for (i = 0; i < nwords; i++) {
        sp = lookup(prefix, 0);
        nmatch = 0;
        for (suf = sp->suf; suf != NULL; suf = suf->next)
            if (rand() % ++nmatch == 0)
                w = suf->word;
        if (strcmp(w, NONWORD) == 0)
            break;
        printf("%s\n", w);
        memmove(prefix, prefix + 1, (NPREF - 1) * sizeof(prefix[0]));
        prefix[NPREF - 1] = w;
    }
}