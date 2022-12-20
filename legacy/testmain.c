#include "./major-codeblocks/cb_search.h"
#include "./utils.c"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int main () {

    char *str = "hello my name is sheila";
    char chars[] = {'h','m','i','o','\0', 's', 'k'};

    for(int i = 0; i < 7; i++){
        printf("found %c in \"%s\" at: %lu\n", chars[i], str, cb_search(str, chars[i], true));
    }

    // clock_t t;

    // for(unsigned long l = 1000000; l < 5000000000; l *= 2){
    //     char *a = (char*) malloc (l* sizeof(char));
    //     memset(a, 'a', (l)*sizeof(char));
    //     printf("l: %lli\n", l);
    //     unsigned long res;
    //     double t;

    //     //len testing
    //     t = clock();
    //     res = strlen(a);
    //     t = clock() - t;
    //     printf("len-glibc:%lu(%f)\n",res, (double)t * 1000/CLOCKS_PER_SEC);

    //     t = clock();
    //     res = cb_search(a, '\0');
    //     t = clock() - t;
    //     printf("len-cb_search:%lu(%f)\n",res, ((double)t) * 1000/CLOCKS_PER_SEC);

    //     //random char search
    //     for(int i = 0; i < 10; i ++){
    //         unsigned long r = rand() % l;
    //         a[r] = 'b';
    //         char* res;

    //         t = clock();
    //         res = strchr(a, 'b');
    //         t = clock() - t;
    //         printf("char search(%lu)-glibc: %lu(%f)\n", r, a - res, (double)t * 1000/CLOCKS_PER_SEC);

    //         t = clock();
    //         res = a + cb_search(a, 'b');
    //         t = clock() - t;
    //         printf("char search(%lu)-cb_search: %lu(%f)\n", r, a - res, (double)t * 1000/CLOCKS_PER_SEC);

    //         a[r] = 'a';
    //     }

    //     free(a);
    // }

    return 0;
}