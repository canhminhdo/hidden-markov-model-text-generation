#include <stdio.h>
#include "markovModel2.h"

int main(int argc, char *argv[]) {
    int i, nwords = MAXGEN;
    char *prefix[NPREF];
    lookupWord(NONWORD, 1);
    for (i = 0; i < NPREF; i ++)
        prefix[i] = NONWORD;
    build(prefix, stdin);
    add(prefix, NONWORD);
    generate(nwords);
    return 0;
}