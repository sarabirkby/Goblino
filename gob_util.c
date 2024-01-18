#include "gob_util.h"

double get_gaussian( void )
{
    double val_1 = 1/rand(), val_2 = 1/rand();

    return ( sqrt(-2 * log(val_1)) * cos(2 * M_PI * val_2) );

}


