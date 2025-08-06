/*
 * measurement.h
 *
 *  Created on: Aug 5, 2025
 *      Author: hiroko
 */

#ifndef INC_MEASUREMENT_H_
#define INC_MEASUREMENT_H_

#include <stdbool.h>
#include "main.h"
#include "stdio.h"
#include "module.h"

#define selectpinsPort GPIOA
#define SAMPLES 10

extern ADC_HandleTypeDef hadc2;

extern int selectPins[4];



void callibration();
void mux_select(int pin);
void measurement(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5,float *vtec);
void values_eval(float *module, float *vtec);

#endif /* INC_MEASUREMENT_H_ */
