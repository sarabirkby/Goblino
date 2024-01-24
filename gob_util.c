#include "gob_util.h"
#include <stdlib.h>
//#define DEBUG
#ifdef DEBUG
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


#ifdef DEBUG // debug

int main( void ) {

     srand(time(NULL));

    double maximum = 0.;
    int * rands = calloc(HIST_WIDTH, sizeof(int));
    for (int i = 0; i < 100000; i++) {
        for (int j = 0; j < HIST_WIDTH; j++) {
            double new_num = get_gaussian();
            double hist_step = 1 / (double) HIST_WIDTH;
            int hist_num = new_num / hist_step;
            rands[hist_num] += 1;
        }
    }
    int maximum_i = 0;
    for (int i = 0; i < HIST_WIDTH; i++) {
        if (rands[i] > maximum_i)
            maximum_i = rands[i];
    }

    for (int i = 0; i < HIST_WIDTH; i++) {
        int num_hist_tiles = HIST_LEN * rands[i] / maximum_i;

        if (i == 0)
            printf(" %d: ", i);
        else
            printf("%2.d: ", i);
        for (int j = 0; j < num_hist_tiles; j++) {
            printf("#");
        }
        printf("\n");


    }

        //if (i % 10 == 0)
            //printf("\n");
        //printf("%f", get_gaussian());
}

#endif

