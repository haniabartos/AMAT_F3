/*
 * module.h
 *
 *  Created on: Aug 5, 2025
 *      Author: hiroko
 */

#ifndef INC_MODULE_H_
#define INC_MODULE_H_


#include <stdbool.h>

typedef struct {
    bool active;
    int heated;
    float values[7];
} Module;

#endif /* INC_MODULE_H_ */
