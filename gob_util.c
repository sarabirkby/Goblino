#include "gob_util.h"
#include <stdlib.h>

double get_unif_rand( void )
{
    return ( (double)(rand()) + 1.0 ) / ( (double)(RAND_MAX) + 1.0 );
}


double get_gaussian( void )
{
    double running_val;

    for (int i = 0; i < GAUSSIAN_PRECISION; i++) {
        running_val += get_unif_rand();
    }

    return running_val / GAUSSIAN_PRECISION;

}

