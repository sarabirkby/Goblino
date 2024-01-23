#include "gob_util.h"
#include <stdlib.h>
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#include <time.h>
#endif // DEBUG
double get_unif_rand( void )
{
    return ( (double)(rand()) + 1.0 ) / ( (double)(RAND_MAX) + 1.0 );
}


double get_gaussian( void )
{
    double running_val = 0;

    for (int i = 0; i < GAUSSIAN_PRECISION; i++) {
        double val = get_unif_rand();
        if (val > 1)
            printf("the val: %f", val);
        running_val += val;

    }

    double final_val = running_val / GAUSSIAN_PRECISION;
    if (final_val > 1)
        printf("%f\n", final_val);
    return final_val;

}


#if DEBUG // debug

int main( void ) {

     srand(time(NULL));

    double maximum = 0.;
    int * rands = calloc(10, sizeof(int));
    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < 10; j++) {
            double new_num = get_gaussian();
            if (new_num < 0.1)
                rands[0] += 1;
            else if (new_num < 0.2)
                rands[1] += 1;
            else if (new_num < 0.3)
                rands[2] += 1;
            else if (new_num < 0.4)
                rands[3] += 1;
            else if (new_num < 0.5)
                rands[4] += 1;
            else if (new_num < 0.6)
                rands[5] += 1;
            else if (new_num < 0.7)
                rands[6] += 1;
            else if (new_num < 0.8)
                rands[7] += 1;
            else if (new_num < 0.9)
                rands[8] += 1;
            else
                rands[9] += 1;
        }
    }
    int maximum_i = 0;
    for (int i = 0; i < 10; i++) {
        if (rands[i] > maximum_i)
            maximum_i = rands[i];
    }

    for (int i = 0; i < 10; i++) {
        int num_histo_tiles = 100 * rands[i] / maximum_i;

        printf("%d: ", i);
        for (int j = 0; j < num_histo_tiles; j++) {
            printf("#");
        }
        printf("\n");


    }

        //if (i % 10 == 0)
            //printf("\n");
        //printf("%f", get_gaussian());
}

#endif

