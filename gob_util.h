#include <stdlib.h>

# define M_PI 3.14159265358979323846

#define GAUSSIAN_PRECISION 3

#define HIST_WIDTH 50
#define HIST_LEN 110

/// Returns a uniformly distributed random number between 0 and 1.
double get_unif_rand( void );

/// Returns a normally distributed random number between 0 and 1.
double get_gaussian( void );
