/*
 * measurement.c
 *
 *  Created on: Aug 5, 2025
 *      Author: hiroko
 */

#include "measurement.h"

ADC_HandleTypeDef hadc2;


int selectPins[4] = {S0_Pin, S1_Pin, S2_Pin, S3_Pin};


void callibration(){
	  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);

}

void mux_select(int pin){
	   for (int i = 0; i < 4; i++) {
	        HAL_GPIO_WritePin(selectpinsPort, selectPins[i], GPIO_PIN_RESET);
	    }

	    // sets the pins to the correct value of the channel -> 0 to 4 bit shift
	    for (int i = 3; i >= 0; i--) {
	        HAL_GPIO_WritePin(selectpinsPort, selectPins[i], (GPIO_PinState)((pin >> i) & 0x01));
	    }
}

void measurement(Module *module1, Module *module2, Module *module3, Module *module4, Module *module5,float *vtec)
{
	  uint16_t temp_mux1[16] = {0};
	  uint16_t temp_mux2[16] = {0};
	  uint16_t temp_mux3[16] = {0};

	  float mux1_values[16] ={0};
	  float mux2_values[16] ={0};
	  float mux3_values[16] ={0};

	for (int s = 0; s < SAMPLES; s++) {
	  for (int j = 0; j < 16; j++) {
		  mux_select(j);

		  HAL_ADC_Start(&hadc2);

		  HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
		  temp_mux1[j] += HAL_ADC_GetValue(&hadc2);
		  HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
		  temp_mux2[j] += HAL_ADC_GetValue(&hadc2);
		  HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
		  temp_mux3[j] += HAL_ADC_GetValue(&hadc2);

		 // printf("%3d | %8lu | %8lu | %8lu \n", j, temp_mux1[j], temp_mux2[j],temp_mux3[j]);


	  }
	}

			for (int i = 0; i < 16; i++) {
		        mux1_values[i] = temp_mux1[i] / SAMPLES;
		        mux2_values[i] = temp_mux2[i] / SAMPLES;
		        mux3_values[i] = temp_mux3[i] / SAMPLES;
		    }

		    for (int z = 0; z < 7; z++) {
		        module1->values[z] = mux1_values[z];
		        module3->values[z] = mux2_values[z];
		        module5->values[z] = mux3_values[z];
		    }

		    for (int y = 0, x = 7; y < 7 && x < 14; y++, x++) {
		        module2->values[y] = mux1_values[x];
		        module4->values[y] = mux2_values[x];
		    }

		    *vtec = mux3_values[7] * 2 * 3.3f / 4095;


		    HAL_Delay(1000);
}

void values_eval(float *module, float *vtec){

	//OFFSET VALUES [V]
	for(int i = 0; i<4 ; i++){
		module[i] = module[i]*3.3f/4095;
	}

	//each current (+ and -) calculation and substraction  (TEC_CURRENT) [mA]
	float tec_current = ((module[4]*3.3f/4095)/(19.3*0.1)*1000) - ((module[5]*3.3f/4095)/(150*0.1)*1000);

	//changing the place of VTH in the table [V]
	module[4] = module[6]*3.3f/4095;;

	module[6] = tec_current;

	module[5] = *vtec;

}
