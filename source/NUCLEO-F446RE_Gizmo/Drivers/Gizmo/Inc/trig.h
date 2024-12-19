#ifndef INC_TRIG_H_
#define INC_TRIG_H_

#include <stdint.h>

float LUT_ASine(float val);
float LUT_Sine(float angle);
void GetTiltAngles(float *abc_buffer, int16_t *xyz_acc);

#endif /* INC_TRIG_H_ */
