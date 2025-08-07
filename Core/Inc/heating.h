/*
 * heating.h
 *
 *  Created on: Aug 5, 2025
 *      Author: hiroko
 */

#ifndef INC_HEATING_H_
#define INC_HEATING_H_

#include "module.h"
#include "main.h"
#include <stdbool.h>
#include <stdlib.h>

#define HEATING_TIME 10000

void module_active(Module *module, int id);
void modules_init(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5);
void is_module_heated(Module *module, float *prev_values);

bool all_heated(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5,uint16_t *heating_counter);

void modules_heating(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5, uint16_t *heating_counter,float *vtec);

#endif /* INC_HEATING_H_ */
