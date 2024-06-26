/**
 * @file pow.h
 * @author SOPER teaching team.
 * @brief Computation of the POW.
 * @version 2.0
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _POW_H
#define _POW_H

# include "config.h"

/**
 * @brief Computes the following hash function:
 * f(x) = (X x + Y) % P.
 *
 * @param x Argument of the hash function, x.
 * @return Result of the hash function, f(x).
 */
long int pow_hash(long int x);

#endif
