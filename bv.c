/* Code for clrBit, setBit, and valBit graciously provided
 * by Professor Darrell Don Earl Long
 */

# include <stdlib.h>
# include "bv.h"
# include <stdint.h>
# include <stdio.h>

//Allocates memory for new structure and pointer
bitV *newVec(uint32_t newLength)
{

        bitV *v = calloc(1, sizeof(uint32_t));
        (v -> v) = calloc(newLength, sizeof(uint8_t));
        (v->l) = newLength;
        return v;
}

void delVec(bitV * vector)
{
        free(vector->v);
        free(vector);
}

void one(bitV * vector)
{
        for (uint32_t i = 0; i < (vector -> l); i++)
        {
                (vector -> v)[i] = (vector -> v)[i] | ~(vector -> v)[i];
        }
}

void setBit(bitV * vector, uint32_t num)
{
        (vector -> v)[num >> 3] |= (01 << (num & 07));
}

void clrBit(bitV * vector, uint32_t num)
{
        (vector -> v)[num >> 3] &= ~(01 << (num & 07));
}

uint8_t valBit(bitV * vector, uint32_t num)
{
        return ((vector -> v)[num >> 3] & (01 << (num & 07))) >> (num & 07);
}

uint32_t lenVec(bitV * v)
{
        return (v -> l);
}
 
