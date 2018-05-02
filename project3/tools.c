#include <stdlib.h>
/*
 * A collection of general tools for use throughout the program.
 */

int randomInteger(int minimum, int maximum)
{
    return minimum + rand() % (maximum - minimum);
}

