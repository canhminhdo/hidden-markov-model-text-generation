#include <stdio.h>
#include "markovModel.h"

int main(int argc, char *argv[]) {
    int i, nwords = MAXGEN;
    char *prefix[NPREF];
    for (i = 0; i < NPREF; i ++)
        prefix[i] = NONWORD;
    build(prefix, stdin);
    add(prefix, NONWORD);
    generate(nwords);
    return 0;
}