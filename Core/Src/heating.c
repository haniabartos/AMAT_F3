/*
 * heating.c
 *
 *  Created on: Aug 5, 2025
 *      Author: hiroko
 */


#include "heating.h"
#include "measurement.h"


float prev_values1[4];
float prev_values2[4]={0};
float prev_values3[4]={0};
float prev_values4[4]={0};
float prev_values5[4]={0};

void module_active(Module *module, int id){
	if((module->values[6]>2000)){
		module->active = true;
		printf("Detected module number: %d\r\n", id);
	}

}

void modules_init(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5){


	module_active(module1,1);
	module_active(module2,2);
	module_active(module3,3);
	module_active(module4,4);
	module_active(module5,5);

	// if not active we're not waiting to heat

	if (!module1->active) module1->heated = 1;
	if (!module2->active) module2->heated = 1;
	if (!module3->active) module3->heated = 1;
	if (!module4->active) module4->heated = 1;
	if (!module5->active) module5->heated = 1;


}
void is_module_heated(Module *module, float *prev_values){

	// let's assume it's heating and then try to deny

    module->heated = 1;


	for (int i = 0; i < 4; i++) {
	        if (abs(module->values[i] - prev_values[i]) > 2) {
	            module->heated = 0; /*if just one value differs from the previous one more than 2 units (around 1,6 mV)
	            we consider the module to be still heating */
	            break;
	        }
	    }

	    for (int j = 0; j < 4; j++) {
	           prev_values[j] = module->values[j];
	       }

}

bool all_heated(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5, uint16_t *heating_counter){

		/* checks if all the modules have heated or the heating time is over 4 hours */

	static bool printed = false;

    if ((module1->heated == 1 && module2->heated == 1 && module3->heated == 1 && module4->heated == 1 && module5->heated == 1) || (*heating_counter == 12)){

        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
        if (!printed) {
                   printf("ALL_HEATED\r\n");
                   printed = true;
               }
        return true;

    }
    return false;

}

void modules_heating(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5, uint16_t *heating_counter,float *vtec){

		/* controls the process of heating the modules*/



	while(!all_heated(module1,module2,module3,module4, module5, heating_counter)){

		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		measurement(module1,module2,module3,module4, module5,vtec);
		printf("Heating in progress...\r\n");

		 if (module1->active && module1->heated == 0) {
				//	HAL_GPIO_WritePin(P2_LED_GPIO_Port, P2_LED_Pin, GPIO_PIN_SET);
					is_module_heated(module1,prev_values1);
				 /*       if (module1->heated == 1) {
				                    HAL_GPIO_WritePin(P2_LED_GPIO_Port, P2_LED_Pin, GPIO_PIN_RESET);
				    	}*/
				 }
		 if (module2->active && module2->heated == 0) {
			 	  HAL_GPIO_WritePin(P3_LED_GPIO_Port, P3_LED_Pin, GPIO_PIN_SET);
				   is_module_heated(module2,prev_values2);
				       if (module2->heated == 1) {
				                    HAL_GPIO_WritePin(P3_LED_GPIO_Port, P3_LED_Pin, GPIO_PIN_RESET);
				    	}
				    }
		 if (module3->active && module3->heated == 0) {
				  HAL_GPIO_WritePin(P4_LED_GPIO_Port, P4_LED_Pin, GPIO_PIN_SET);
				   is_module_heated(module3,prev_values3);
				      if (module3->heated == 1) {
				                    HAL_GPIO_WritePin(P4_LED_GPIO_Port, P4_LED_Pin, GPIO_PIN_RESET);
				    	}
				    }
		 if (module4->active && module4->heated == 0) {
				   HAL_GPIO_WritePin(P5_LED_GPIO_Port, P5_LED_Pin, GPIO_PIN_SET);
			       is_module_heated(module4,prev_values4);
				        if (module4->heated == 1) {
				                    HAL_GPIO_WritePin(P5_LED_GPIO_Port, P5_LED_Pin, GPIO_PIN_RESET);
				    	}
				    }
		 if (module5->active && module5->heated == 0) {
				  	HAL_GPIO_WritePin(P6_LED_GPIO_Port, P6_LED_Pin, GPIO_PIN_SET);
				   is_module_heated(module5,prev_values5);
				          if (module5->heated == 1) {
				                    HAL_GPIO_WritePin(P6_LED_GPIO_Port, P6_LED_Pin, GPIO_PIN_RESET);
				        }
				    }

		 if (all_heated(module1, module2, module3, module4, module5, heating_counter)) {
		            break;
		        }

		 (*heating_counter)++;

		 HAL_Delay(HEATING_TIME);


	}
}
