#ifndef _markov_model_
#define _markov_model_

enum {
    NPREF = 2,  // number of prefix
    NHASH = 4093,   // size of the hash table
    MAXGEN = 1000,  // maximum length of output
    MULTIPLIER = 31 // used for hashing
};

extern char NONWORD[]; // a sign word for termination

typedef struct State State;
typedef struct Suffix Suffix;

struct State {
    char *pref[NPREF]; // prefix words
    Suffix *suf; // list of suffices
    State *next; // next in hash table
};

struct Suffix {
    char *word; // suffix
    Suffix *next; // next in list of suffixes
};

State *statetab[NHASH]; // hash table of states

unsigned int hash(char *s[NPREF]);
State* lookup(char *prefix[NPREF], int create);
void build(char *prefix[NPREF], FILE *file);
void add(char *prefix[NPREF], char *suffix);
void addSuffix(State *sp, char *suffix);
void generate(int nwords);

#endif