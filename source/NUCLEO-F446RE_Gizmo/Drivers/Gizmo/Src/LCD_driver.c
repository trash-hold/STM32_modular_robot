#include "LCD_driver.h"
#include <stdio.h>

#define SERVO_INFO_BLOCK_OFF_Y 40
#define ACC_INFO_BLOCK_OFF_Y 85
#define STATUS_INFO_OFF_Y 150
#define SERVO_INFO_DATA_OFF 30
#define ACC_INFO_DATA_OFF 25
#define INFO_OFFSET 10
#define INFO_BIG_OFFSET 15

static SCREEN current_screen;
const uint16_t white_px_LUT[] = { 58, 62, 59, 62, 60, 62, 61, 62, 62, 62, 56, 63, 57, 63, 58, 63, 59, 63, 60, 63, 61, 63, 62, 63, 63, 63, 64, 63, 65, 63, 56, 64, 57, 64, 58, 64, 59, 64, 60, 64, 66, 64, 55, 65, 56, 65, 57, 65, 58, 65, 66, 65, 67, 65, 54, 66, 56, 66, 57, 66, 67, 66, 75, 66, 55, 67, 66, 67, 67, 67, 75, 67, 55, 68, 56, 68, 63, 68, 64, 68, 66, 68, 55, 69, 56, 69, 65, 69, 55, 70, 57, 70, 58, 70, 63, 70, 64, 70, 56, 71, 57, 71, 58, 71, 59, 71, 60, 71, 61, 71, 62, 71, 56, 72, 57, 72, 58, 72, 59, 72, 92, 74, 86, 79, 73, 80, 74, 80, 75, 80, 76, 80, 86, 80, 87, 80, 72, 81, 73, 81, 74, 81, 75, 81, 76, 81, 77, 81, 87, 81, 88, 81, 89, 81, 90, 81, 73, 82, 74, 82, 75, 82, 76, 82, 77, 82, 73, 83, 74, 83, 75, 83, 76, 83, 77, 83, 78, 83, 73, 84, 74, 84, 75, 84, 76, 84, 77, 84, 78, 84, 73, 85, 74, 85, 75, 85, 76, 85, 77, 85, 78, 85, 79, 85, 58, 86, 59, 86, 73, 86, 74, 86, 75, 86, 76, 86, 77, 86, 78, 86, 79, 86, 55, 87, 56, 87, 57, 87, 58, 87, 59, 87, 60, 87, 61, 87, 73, 87, 74, 87, 75, 87, 76, 87, 77, 87, 78, 87, 79, 87, 55, 88, 56, 88, 57, 88, 58, 88, 59, 88, 60, 88, 61, 88, 62, 88, 73, 88, 74, 88, 75, 88, 77, 88, 78, 88, 79, 88, 55, 89, 56, 89, 57, 89, 58, 89, 59, 89, 60, 89, 61, 89, 62, 89, 73, 89, 74, 89, 75, 89, 76, 89, 78, 89, 79, 89, 55, 90, 56, 90, 57, 90, 58, 90, 59, 90, 60, 90, 61, 90, 62, 90, 63, 90, 64, 90, 73, 90, 74, 90, 75, 90, 76, 90, 77, 90, 78, 90, 79, 90, 55, 91, 56, 91, 57, 91, 58, 91, 59, 91, 60, 91, 61, 91, 62, 91, 63, 91, 64, 91, 73, 91, 55, 92, 57, 92, 58, 92, 59, 92, 60, 92, 72, 92, 73, 92, 57, 93, 59, 93, 75, 93, 57, 94, 58, 94, 59, 94, 72, 94, 57, 95, 72, 95, 75, 95, 65, 109, 66, 109 };

const uint16_t l_grey_px_LUT[] = { 87, 47, 88, 47, 89, 47, 90, 47, 91, 47, 92, 47, 93, 47, 94, 47, 95, 47, 87, 48, 88, 48, 89, 48, 90, 48, 91, 48, 92, 48, 93, 48, 94, 48, 95, 48, 85, 49, 86, 49, 87, 49, 84, 50, 85, 50, 83, 51, 82, 52, 81, 53, 80, 54, 81, 54, 78, 55, 79, 55, 80, 55, 92, 55, 93, 55, 77, 56, 79, 56, 88, 56, 89, 56, 91, 56, 86, 57, 87, 57, 88, 57, 89, 57, 90, 57, 91, 57, 92, 57, 93, 57, 84, 58, 86, 58, 87, 58, 88, 58, 89, 58, 90, 58, 91, 58, 37, 59, 38, 59, 39, 59, 40, 59, 41, 59, 42, 59, 43, 59, 72, 59, 73, 59, 80, 59, 81, 59, 85, 59, 86, 59, 87, 59, 88, 59, 89, 59, 90, 59, 91, 59, 35, 60, 36, 60, 37, 60, 45, 60, 46, 60, 47, 60, 70, 60, 81, 60, 82, 60, 85, 60, 86, 60, 87, 60, 88, 60, 89, 60, 90, 60, 91, 60, 92, 60, 93, 60, 34, 61, 35, 61, 47, 61, 48, 61, 53, 61, 58, 61, 59, 61, 60, 61, 61, 61, 62, 61, 68, 61, 81, 61, 82, 61, 85, 61, 86, 61, 87, 61, 88, 61, 89, 61, 90, 61, 91, 61, 32, 62, 55, 62, 56, 62, 57, 62, 63, 62, 64, 62, 82, 62, 83, 62, 85, 62, 86, 62, 87, 62, 88, 62, 89, 62, 90, 62, 91, 62, 32, 63, 55, 63, 66, 63, 82, 63, 83, 63, 85, 63, 86, 63, 87, 63, 88, 63, 89, 63, 90, 63, 92, 63, 48, 64, 55, 64, 61, 64, 62, 64, 63, 64, 64, 64, 65, 64, 73, 64, 74, 64, 75, 64, 82, 64, 85, 64, 86, 64, 87, 64, 88, 64, 89, 64, 90, 64, 91, 64, 47, 65, 48, 65, 54, 65, 59, 65, 63, 65, 64, 65, 65, 65, 82, 65, 85, 65, 86, 65, 87, 65, 88, 65, 89, 65, 90, 65, 34, 66, 41, 66, 42, 66, 43, 66, 47, 66, 48, 66, 55, 66, 58, 66, 59, 66, 60, 66, 61, 66, 62, 66, 63, 66, 64, 66, 65, 66, 66, 66, 76, 66, 82, 66, 84, 66, 85, 66, 86, 66, 87, 66, 88, 66, 89, 66, 41, 67, 42, 67, 47, 67, 48, 67, 54, 67, 56, 67, 57, 67, 62, 67, 63, 67, 64, 67, 76, 67, 83, 67, 84, 67, 85, 67, 86, 67, 87, 67, 88, 67, 40, 68, 41, 68, 47, 68, 48, 68, 54, 68, 65, 68, 67, 68, 83, 68, 84, 68, 85, 68, 86, 68, 87, 68, 40, 69, 43, 69, 48, 69, 49, 69, 54, 69, 57, 69, 64, 69, 66, 69, 69, 69, 70, 69, 82, 69, 39, 70, 42, 70, 43, 70, 44, 70, 45, 70, 48, 70, 49, 70, 54, 70, 56, 70, 59, 70, 62, 70, 65, 70, 66, 70, 67, 70, 68, 70, 71, 70, 72, 70, 88, 70, 39, 71, 40, 71, 42, 71, 43, 71, 44, 71, 48, 71, 49, 71, 50, 71, 54, 71, 55, 71, 63, 71, 64, 71, 65, 71, 66, 71, 69, 71, 80, 71, 87, 71, 88, 71, 92, 71, 39, 72, 42, 72, 43, 72, 45, 72, 46, 72, 50, 72, 51, 72, 54, 72, 55, 72, 60, 72, 61, 72, 68, 72, 69, 72, 70, 72, 91, 72, 92, 72, 93, 72, 40, 73, 42, 73, 43, 73, 44, 73, 45, 73, 46, 73, 47, 73, 48, 73, 51, 73, 52, 73, 55, 73, 56, 73, 57, 73, 58, 73, 65, 73, 66, 73, 67, 73, 68, 73, 69, 73, 72, 73, 73, 73, 79, 73, 87, 73, 42, 74, 43, 74, 44, 74, 45, 74, 46, 74, 47, 74, 48, 74, 49, 74, 63, 74, 64, 74, 65, 74, 71, 74, 72, 74, 78, 74, 88, 74, 89, 74, 90, 74, 45, 75, 46, 75, 47, 75, 48, 75, 63, 75, 64, 75, 65, 75, 66, 75, 67, 75, 68, 75, 69, 75, 70, 75, 71, 75, 72, 75, 88, 75, 89, 75, 90, 75, 91, 75, 92, 75, 93, 75, 94, 75, 95, 75, 54, 76, 65, 76, 66, 76, 67, 76, 68, 76, 69, 76, 70, 76, 84, 76, 85, 76, 88, 76, 89, 76, 90, 76, 91, 76, 65, 77, 66, 77, 67, 77, 68, 77, 69, 77, 70, 77, 83, 77, 84, 77, 85, 77, 88, 77, 89, 77, 90, 77, 91, 77, 71, 78, 72, 78, 73, 78, 74, 78, 82, 78, 83, 78, 84, 78, 85, 78, 86, 78, 88, 78, 89, 78, 71, 79, 72, 79, 73, 79, 74, 79, 75, 79, 76, 79, 82, 79, 83, 79, 84, 79, 85, 79, 87, 79, 72, 80, 77, 80, 82, 80, 83, 80, 84, 80, 85, 80, 88, 80, 89, 80, 90, 80, 91, 80, 82, 81, 83, 81, 84, 81, 85, 81, 86, 81, 91, 81, 72, 82, 78, 82, 83, 82, 84, 82, 85, 82, 86, 82, 87, 82, 88, 82, 89, 82, 90, 82, 91, 82, 72, 83, 83, 83, 84, 83, 85, 83, 86, 83, 87, 83, 88, 83, 89, 83, 72, 84, 79, 84, 84, 84, 85, 84, 86, 84, 87, 84, 88, 84, 85, 85, 86, 85, 87, 85, 54, 86, 56, 86, 57, 86, 60, 86, 61, 86, 80, 86, 62, 87, 80, 87, 54, 88, 63, 88, 64, 88, 76, 88, 80, 88, 81, 88, 54, 89, 63, 89, 64, 89, 72, 89, 77, 89, 80, 89, 81, 89, 54, 90, 72, 90, 80, 90, 81, 90, 82, 90, 54, 91, 72, 91, 74, 91, 75, 91, 76, 91, 77, 91, 78, 91, 79, 91, 80, 91, 81, 91, 54, 92, 56, 92, 61, 92, 62, 92, 63, 92, 64, 92, 71, 92, 74, 92, 75, 92, 76, 92, 77, 92, 78, 92, 79, 92, 80, 92, 81, 92, 54, 93, 55, 93, 56, 93, 58, 93, 60, 93, 61, 93, 63, 93, 64, 93, 70, 93, 71, 93, 72, 93, 73, 93, 74, 93, 76, 93, 77, 93, 78, 93, 79, 93, 80, 93, 81, 93, 55, 94, 56, 94, 63, 94, 64, 94, 65, 94, 68, 94, 69, 94, 70, 94, 71, 94, 73, 94, 74, 94, 75, 94, 76, 94, 77, 94, 78, 94, 79, 94, 80, 94, 56, 95, 62, 95, 65, 95, 66, 95, 67, 95, 68, 95, 69, 95, 70, 95, 71, 95, 73, 95, 74, 95, 76, 95, 77, 95, 78, 95, 79, 95, 80, 95, 56, 96, 57, 96, 61, 96, 62, 96, 63, 96, 64, 96, 66, 96, 67, 96, 68, 96, 69, 96, 70, 96, 71, 96, 73, 96, 74, 96, 75, 96, 76, 96, 77, 96, 78, 96, 79, 96, 58, 97, 61, 97, 62, 97, 63, 97, 64, 97, 65, 97, 66, 97, 67, 97, 68, 97, 69, 97, 70, 97, 71, 97, 72, 97, 73, 97, 74, 97, 75, 97, 76, 97, 77, 97, 78, 97, 79, 97, 62, 98, 63, 98, 64, 98, 65, 98, 66, 98, 67, 98, 68, 98, 69, 98, 70, 98, 71, 98, 72, 98, 73, 98, 74, 98, 75, 98, 76, 98, 77, 98, 78, 98, 79, 98, 60, 99, 67, 99, 68, 99, 69, 99, 70, 99, 71, 99, 72, 99, 73, 99, 74, 99, 75, 99, 76, 99, 77, 99, 78, 99, 79, 99, 65, 100, 66, 100, 67, 100, 68, 100, 69, 100, 70, 100, 71, 100, 72, 100, 73, 100, 74, 100, 75, 100, 76, 100, 77, 100, 78, 100, 79, 100, 61, 101, 63, 101, 64, 101, 65, 101, 66, 101, 67, 101, 68, 101, 69, 101, 70, 101, 71, 101, 76, 101, 77, 101, 78, 101, 79, 101, 61, 102, 62, 102, 63, 102, 64, 102, 65, 102, 66, 102, 67, 102, 68, 102, 69, 102, 70, 102, 71, 102, 76, 102, 77, 102, 78, 102, 79, 102, 61, 103, 62, 103, 63, 103, 64, 103, 65, 103, 66, 103, 67, 103, 68, 103, 69, 103, 70, 103, 71, 103, 72, 103, 74, 103, 75, 103, 76, 103, 77, 103, 78, 103, 60, 104, 61, 104, 62, 104, 63, 104, 64, 104, 65, 104, 66, 104, 67, 104, 68, 104, 69, 104, 70, 104, 71, 104, 72, 104, 74, 104, 75, 104, 76, 104, 77, 104, 78, 104, 79, 104, 60, 105, 61, 105, 62, 105, 63, 105, 64, 105, 65, 105, 66, 105, 67, 105, 68, 105, 69, 105, 70, 105, 71, 105, 72, 105, 74, 105, 75, 105, 76, 105, 77, 105, 78, 105, 79, 105, 80, 105, 60, 106, 61, 106, 62, 106, 63, 106, 64, 106, 65, 106, 66, 106, 67, 106, 68, 106, 69, 106, 70, 106, 71, 106, 72, 106, 74, 106, 75, 106, 76, 106, 77, 106, 78, 106, 79, 106, 80, 106, 60, 107, 61, 107, 62, 107, 63, 107, 64, 107, 65, 107, 66, 107, 67, 107, 68, 107, 69, 107, 70, 107, 71, 107, 72, 107, 74, 107, 75, 107, 76, 107, 77, 107, 78, 107, 79, 107, 80, 107, 81, 107, 60, 108, 61, 108, 62, 108, 63, 108, 64, 108, 65, 108, 66, 108, 67, 108, 68, 108, 69, 108, 70, 108, 71, 108, 74, 108, 75, 108, 76, 108, 77, 108, 78, 108, 79, 108, 60, 109, 61, 109, 62, 109, 63, 109, 64, 109, 67, 109, 68, 109, 69, 109, 70, 109, 71, 109, 75, 109, 76, 109, 77, 109, 80, 109, 62, 110, 63, 110, 64, 110, 65, 110, 66, 110, 67, 110, 68, 110, 69, 110, 70, 110, 72, 110, 65, 111, 70, 111, 72, 111, 76, 111, 77, 111, 79, 111, 80, 111, 62, 112, 63, 112, 64, 112, 67, 112, 68, 112 };

const uint16_t d_grey_px_LUT[] = { 88, 49, 89, 49, 90, 49, 91, 49, 92, 49, 93, 49, 94, 49, 95, 49, 86, 50, 87, 50, 88, 50, 89, 50, 90, 50, 91, 50, 92, 50, 93, 50, 95, 50, 84, 51, 85, 51, 86, 51, 87, 51, 88, 51, 89, 51, 90, 51, 91, 51, 92, 51, 93, 51, 83, 52, 84, 52, 85, 52, 86, 52, 87, 52, 88, 52, 89, 52, 90, 52, 91, 52, 92, 52, 93, 52, 82, 53, 83, 53, 84, 53, 85, 53, 86, 53, 87, 53, 88, 53, 89, 53, 90, 53, 91, 53, 92, 53, 93, 53, 82, 54, 83, 54, 84, 54, 85, 54, 86, 54, 87, 54, 88, 54, 89, 54, 90, 54, 91, 54, 92, 54, 93, 54, 81, 55, 82, 55, 83, 55, 84, 55, 85, 55, 86, 55, 87, 55, 88, 55, 89, 55, 90, 55, 91, 55, 78, 56, 80, 56, 81, 56, 82, 56, 83, 56, 84, 56, 85, 56, 86, 56, 87, 56, 90, 56, 92, 56, 93, 56, 76, 57, 77, 57, 78, 57, 79, 57, 80, 57, 81, 57, 82, 57, 83, 57, 84, 57, 85, 57, 75, 58, 76, 58, 77, 58, 78, 58, 79, 58, 80, 58, 81, 58, 82, 58, 83, 58, 85, 58, 92, 58, 93, 58, 74, 59, 75, 59, 76, 59, 77, 59, 78, 59, 79, 59, 82, 59, 83, 59, 84, 59, 92, 59, 93, 59, 38, 60, 39, 60, 40, 60, 41, 60, 42, 60, 43, 60, 44, 60, 71, 60, 72, 60, 73, 60, 74, 60, 75, 60, 76, 60, 77, 60, 78, 60, 79, 60, 80, 60, 83, 60, 84, 60, 36, 61, 37, 61, 38, 61, 39, 61, 40, 61, 41, 61, 42, 61, 43, 61, 44, 61, 45, 61, 46, 61, 49, 61, 50, 61, 51, 61, 52, 61, 69, 61, 70, 61, 71, 61, 72, 61, 73, 61, 74, 61, 75, 61, 76, 61, 77, 61, 78, 61, 79, 61, 80, 61, 83, 61, 84, 61, 92, 61, 93, 61, 33, 62, 34, 62, 35, 62, 36, 62, 37, 62, 38, 62, 39, 62, 40, 62, 41, 62, 42, 62, 43, 62, 44, 62, 45, 62, 46, 62, 47, 62, 48, 62, 49, 62, 50, 62, 51, 62, 52, 62, 53, 62, 54, 62, 65, 62, 66, 62, 67, 62, 68, 62, 69, 62, 70, 62, 71, 62, 72, 62, 73, 62, 74, 62, 75, 62, 76, 62, 77, 62, 78, 62, 79, 62, 80, 62, 81, 62, 84, 62, 92, 62, 33, 63, 34, 63, 35, 63, 36, 63, 37, 63, 38, 63, 39, 63, 40, 63, 41, 63, 42, 63, 43, 63, 44, 63, 45, 63, 46, 63, 47, 63, 48, 63, 49, 63, 50, 63, 51, 63, 52, 63, 53, 63, 54, 63, 67, 63, 68, 63, 69, 63, 70, 63, 71, 63, 72, 63, 73, 63, 74, 63, 75, 63, 76, 63, 77, 63, 78, 63, 79, 63, 80, 63, 81, 63, 84, 63, 91, 63, 34, 64, 35, 64, 36, 64, 37, 64, 38, 64, 39, 64, 40, 64, 41, 64, 42, 64, 43, 64, 44, 64, 45, 64, 46, 64, 47, 64, 49, 64, 50, 64, 51, 64, 52, 64, 53, 64, 54, 64, 67, 64, 68, 64, 69, 64, 70, 64, 71, 64, 72, 64, 76, 64, 77, 64, 78, 64, 80, 64, 81, 64, 83, 64, 84, 64, 34, 65, 35, 65, 36, 65, 37, 65, 38, 65, 39, 65, 40, 65, 41, 65, 42, 65, 43, 65, 44, 65, 45, 65, 46, 65, 49, 65, 52, 65, 53, 65, 60, 65, 61, 65, 62, 65, 68, 65, 69, 65, 70, 65, 71, 65, 72, 65, 74, 65, 75, 65, 76, 65, 77, 65, 78, 65, 79, 65, 80, 65, 81, 65, 83, 65, 84, 65, 35, 66, 36, 66, 37, 66, 38, 66, 39, 66, 40, 66, 44, 66, 45, 66, 46, 66, 49, 66, 51, 66, 52, 66, 53, 66, 68, 66, 69, 66, 73, 66, 74, 66, 77, 66, 79, 66, 80, 66, 81, 66, 83, 66, 35, 67, 36, 67, 37, 67, 38, 67, 39, 67, 40, 67, 43, 67, 44, 67, 45, 67, 46, 67, 49, 67, 50, 67, 52, 67, 53, 67, 58, 67, 59, 67, 60, 67, 61, 67, 65, 67, 68, 67, 69, 67, 70, 67, 73, 67, 74, 67, 77, 67, 78, 67, 79, 67, 80, 67, 81, 67, 82, 67, 36, 68, 37, 68, 38, 68, 39, 68, 42, 68, 43, 68, 44, 68, 45, 68, 46, 68, 49, 68, 50, 68, 51, 68, 52, 68, 53, 68, 57, 68, 60, 68, 61, 68, 62, 68, 69, 68, 70, 68, 71, 68, 72, 68, 73, 68, 74, 68, 75, 68, 76, 68, 77, 68, 78, 68, 79, 68, 80, 68, 81, 68, 82, 68, 37, 69, 38, 69, 39, 69, 41, 69, 42, 69, 44, 69, 45, 69, 46, 69, 47, 69, 50, 69, 51, 69, 52, 69, 53, 69, 58, 69, 61, 69, 62, 69, 67, 69, 71, 69, 72, 69, 73, 69, 74, 69, 75, 69, 76, 69, 77, 69, 78, 69, 79, 69, 80, 69, 81, 69, 37, 70, 38, 70, 40, 70, 41, 70, 46, 70, 47, 70, 50, 70, 51, 70, 52, 70, 53, 70, 60, 70, 61, 70, 69, 70, 70, 70, 73, 70, 74, 70, 75, 70, 76, 70, 77, 70, 78, 70, 79, 70, 80, 70, 38, 71, 41, 71, 45, 71, 46, 71, 47, 71, 51, 71, 52, 71, 53, 71, 67, 71, 70, 71, 72, 71, 73, 71, 75, 71, 76, 71, 77, 71, 78, 71, 40, 72, 41, 72, 47, 72, 48, 72, 49, 72, 52, 72, 53, 72, 62, 72, 63, 72, 64, 72, 65, 72, 67, 72, 71, 72, 72, 72, 73, 72, 74, 72, 75, 72, 76, 72, 77, 72, 78, 72, 79, 72, 87, 72, 88, 72, 41, 73, 49, 73, 50, 73, 53, 73, 54, 73, 59, 73, 60, 73, 61, 73, 62, 73, 63, 73, 64, 73, 70, 73, 74, 73, 75, 73, 76, 73, 77, 73, 78, 73, 86, 73, 88, 73, 91, 73, 92, 73, 50, 74, 51, 74, 52, 74, 53, 74, 54, 74, 55, 74, 56, 74, 57, 74, 58, 74, 59, 74, 60, 74, 62, 74, 66, 74, 67, 74, 68, 74, 69, 74, 70, 74, 73, 74, 74, 74, 76, 74, 77, 74, 86, 74, 87, 74, 91, 74, 49, 75, 50, 75, 51, 75, 54, 75, 55, 75, 56, 75, 57, 75, 58, 75, 59, 75, 60, 75, 86, 75, 87, 75, 55, 76, 56, 76, 57, 76, 59, 76, 63, 76, 64, 76, 71, 76, 72, 76, 73, 76, 74, 76, 86, 76, 87, 76, 92, 76, 93, 76, 56, 77, 57, 77, 59, 77, 63, 77, 64, 77, 71, 77, 72, 77, 73, 77, 74, 77, 76, 77, 77, 77, 78, 77, 79, 77, 81, 77, 82, 77, 86, 77, 87, 77, 92, 77, 75, 78, 76, 78, 77, 78, 78, 78, 80, 78, 81, 78, 87, 78, 90, 78, 91, 78, 67, 79, 68, 79, 69, 79, 70, 79, 77, 79, 78, 79, 79, 79, 80, 79, 81, 79, 88, 79, 89, 79, 90, 79, 91, 79, 58, 80, 59, 80, 60, 80, 61, 80, 62, 80, 66, 80, 67, 80, 68, 80, 69, 80, 70, 80, 71, 80, 78, 80, 79, 80, 80, 80, 81, 80, 57, 81, 58, 81, 59, 81, 60, 81, 61, 81, 62, 81, 63, 81, 64, 81, 65, 81, 66, 81, 67, 81, 68, 81, 69, 81, 70, 81, 71, 81, 78, 81, 79, 81, 80, 81, 81, 81, 56, 82, 57, 82, 58, 82, 59, 82, 60, 82, 61, 82, 62, 82, 63, 82, 64, 82, 66, 82, 67, 82, 68, 82, 69, 82, 70, 82, 71, 82, 79, 82, 80, 82, 81, 82, 82, 82, 56, 83, 57, 83, 58, 83, 59, 83, 60, 83, 61, 83, 62, 83, 63, 83, 64, 83, 65, 83, 66, 83, 67, 83, 68, 83, 69, 83, 70, 83, 71, 83, 79, 83, 80, 83, 81, 83, 82, 83, 55, 84, 56, 84, 57, 84, 58, 84, 59, 84, 60, 84, 61, 84, 62, 84, 63, 84, 64, 84, 65, 84, 66, 84, 67, 84, 68, 84, 69, 84, 70, 84, 71, 84, 80, 84, 81, 84, 83, 84, 55, 85, 56, 85, 57, 85, 58, 85, 59, 85, 60, 85, 61, 85, 62, 85, 63, 85, 64, 85, 66, 85, 67, 85, 68, 85, 69, 85, 70, 85, 71, 85, 72, 85, 80, 85, 81, 85, 84, 85, 55, 86, 62, 86, 66, 86, 67, 86, 68, 86, 69, 86, 70, 86, 71, 86, 72, 86, 81, 86, 82, 86, 83, 86, 54, 87, 66, 87, 67, 87, 68, 87, 69, 87, 70, 87, 71, 87, 72, 87, 81, 87, 82, 87, 66, 88, 67, 88, 68, 88, 69, 88, 70, 88, 71, 88, 72, 88, 82, 88, 65, 89, 66, 89, 67, 89, 68, 89, 69, 89, 70, 89, 71, 89, 82, 89, 65, 90, 66, 90, 67, 90, 68, 90, 69, 90, 70, 90, 71, 90, 65, 91, 67, 91, 68, 91, 69, 91, 70, 91, 65, 92, 67, 92, 68, 92, 69, 92, 70, 92, 62, 93, 65, 93, 67, 93, 68, 93, 69, 93, 60, 94, 61, 94, 62, 94, 66, 94, 67, 94, 58, 95, 59, 95, 60, 95, 61, 95, 63, 95, 64, 95, 58, 96, 59, 96, 60, 96, 65, 96, 59, 97, 60, 97, 60, 98, 61, 98, 61, 99, 62, 99, 63, 99, 64, 99, 65, 99, 66, 99, 61, 100, 62, 100, 63, 100, 64, 100, 62, 101, 72, 101, 73, 101, 74, 101, 75, 101, 72, 102, 73, 102, 74, 102, 75, 102, 73, 103, 73, 104, 73, 105, 73, 106, 73, 107, 72, 108, 73, 108, 80, 108, 81, 108, 72, 109, 73, 109, 74, 109, 78, 109, 79, 109, 81, 109, 82, 109, 61, 110, 71, 110, 76, 110, 77, 110, 78, 110, 79, 110, 80, 110, 81, 110, 62, 111, 63, 111, 64, 111, 66, 111, 67, 111, 68, 111, 69, 111, 71, 111, 78, 111, 65, 112, 66, 112, 69, 112, 70, 112, 71, 112, 72, 112, 73, 112, 66, 113, 67, 113, 68, 113, 69, 113, 70, 113, 71, 113, 72, 113 };



void Screen_Init()
{
	LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;//SCAN_DIR_DFT = D2U_L2R
	LCD_Init( Lcd_ScanDir );
	LCD_Clear(BLACK);
}


static void Screen_Draw_HeaderJustified(uint16_t y, const char* string, uint16_t length, sFONT* Font, uint16_t color)
{
	uint16_t x_offset = (128 - length*(Font->Width)) / 2;
	LCD_DisplayString(x_offset, y, string, Font, LCD_BACKGROUND, color);
}

void Screen_DrawInitScreen()
{
	// Clear screen
	LCD_Clear(BLACK);

	// ====================================
	// Draw header
	// ====================================
	Screen_Draw_HeaderJustified(10, "Gizmo Robot", 11, &Font16, WHITE);

	// ====================================
	// Draw Gizmo
	// ====================================
	int px_number = (sizeof(white_px_LUT) / sizeof(uint16_t)) / 2;
	for(int i = 0; i < px_number; i++)
	{
		LCD_DrawPoint(white_px_LUT[i*2], white_px_LUT[i*2 + 1], WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	}

	px_number = (sizeof(l_grey_px_LUT) / sizeof(uint16_t)) / 2;
	for(int i = 0; i < px_number; i++)
	{
		LCD_DrawPoint(l_grey_px_LUT[i*2], l_grey_px_LUT[i*2 + 1], GRAY, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	}

	px_number = (sizeof(d_grey_px_LUT) / sizeof(uint16_t)) / 2;
	for(int i = 0; i < px_number; i++)
	{
		LCD_DrawPoint(d_grey_px_LUT[i*2], d_grey_px_LUT[i*2 + 1], DARK_GREY, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	}

	// ====================================
	// Display init panel
	// ====================================
	Screen_Draw_HeaderJustified(135, "Initializing", 12, &Font12, WHITE);
	Screen_Draw_HeaderJustified(150, "Please wait", 11, &Font8, GRAY);

	current_screen = INIT_SCREEN;
}

ReturnCode Screen_DrawNextInit(PERIPHERAL per, uint8_t clear)
{
	// Check if displayed panel is correct
	if (current_screen != INIT_SCREEN)
		return G_ERROR;

	switch(per)
	{
		case CAN:
			// If clear is needed overwrite
			if (clear == 0x01)
			{
				Screen_Draw_HeaderJustified(135, "CAN init", 8, &Font12, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(150, "Servo0 init", 11, &Font8, LCD_BACKGROUND);
				return G_SUCCESS;
			}
			// If write is needed clear previous sequence
			Screen_DrawNextInit(0xFF, 0x01);
			Screen_Draw_HeaderJustified(135, "CAN init", 8, &Font12, WHITE);
			Screen_Draw_HeaderJustified(150, "Servo0 init", 11, &Font8, WHITE);
			break;
		case SERVO_0:
			// If clear is needed overwrite
			if (clear == 0x01)
			{
				Screen_Draw_HeaderJustified(125, "CAN init", 8, &Font8, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(135, "Servo0 init", 11, &Font12, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(150, "Servo1 init", 11, &Font8, LCD_BACKGROUND);
				return G_SUCCESS;
			}
			// If write is needed clear previous sequence
			Screen_DrawNextInit(CAN, 0x01);
			Screen_Draw_HeaderJustified(125, "CAN init", 8, &Font8, DARK_GREEN);
			Screen_Draw_HeaderJustified(135, "Servo0 init", 11, &Font12, WHITE);
			Screen_Draw_HeaderJustified(150, "Servo1 init", 11, &Font8, WHITE);
			break;

		case SERVO_1:
			// If clear is needed overwrite
			if (clear == 0x01)
			{
				Screen_Draw_HeaderJustified(125, "Servo0 init", 11, &Font8, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(135, "Servo1 init", 11, &Font12, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(150, "Acc0 init", 9, &Font8, LCD_BACKGROUND);
				return G_SUCCESS;
			}
			// If write is needed clear previous sequence
			Screen_DrawNextInit(SERVO_0, 0x01);
			Screen_Draw_HeaderJustified(125, "Servo0 init", 11, &Font8, DARK_GREEN);
			Screen_Draw_HeaderJustified(135, "Servo1 init", 11, &Font12, WHITE);
			Screen_Draw_HeaderJustified(150, "Acc0 init", 9, &Font8, WHITE);
			break;

		case ACC_0:
			// If clear is needed overwrite
			if (clear == 0x01)
			{
				Screen_Draw_HeaderJustified(125, "Servo1 init", 11, &Font8, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(135, "Acc0 init", 9, &Font12, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(150, "Acc1 init", 9, &Font8, LCD_BACKGROUND);
				return G_SUCCESS;
			}
			// If write is needed clear previous sequence
			Screen_DrawNextInit(SERVO_1, 0x01);
			Screen_Draw_HeaderJustified(125, "Servo1 init", 11, &Font8, DARK_GREEN);
			Screen_Draw_HeaderJustified(135, "Acc0 init", 9, &Font12, WHITE);
			Screen_Draw_HeaderJustified(150, "Acc1 init", 9, &Font8, WHITE);
			break;
		case ACC_1:
			// If clear is needed overwrite
			if (clear == 0x01)
			{
				Screen_Draw_HeaderJustified(125, "Acc0 init", 9, &Font8, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(135, "Acc1 init", 9, &Font12, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(150, "SD init", 7, &Font8, LCD_BACKGROUND);
				return G_SUCCESS;
			}
			// If write is needed clear previous sequence
			Screen_DrawNextInit(ACC_0, 0x01);
			Screen_Draw_HeaderJustified(125, "Acc0 init", 9, &Font8, DARK_GREEN);
			Screen_Draw_HeaderJustified(135, "Acc1 init", 9, &Font12, WHITE);
			Screen_Draw_HeaderJustified(150, "SD init", 7, &Font8, WHITE);
			break;
		case SD:
			// If clear is needed overwrite
			if (clear == 0x01)
			{
				Screen_Draw_HeaderJustified(125, "Acc1 init", 9, &Font8, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(135, "SD init", 7, &Font12, LCD_BACKGROUND);
				Screen_Draw_HeaderJustified(150, "Done", 4, &Font8, LCD_BACKGROUND);
				return G_SUCCESS;
			}
			// If write is needed clear previous sequence
			Screen_DrawNextInit(ACC_1, 0x01);
			Screen_Draw_HeaderJustified(125, "Acc1 init", 9, &Font8, DARK_GREEN);
			Screen_Draw_HeaderJustified(135, "SD init", 7, &Font12, WHITE);
			Screen_Draw_HeaderJustified(150, "Done", 4, &Font8, WHITE);
			break;

		default:
			if(clear == 0x00)
			{
				Screen_DrawNextInit(SD, 0x01);
				Screen_Draw_HeaderJustified(135, "Done", 4, &Font12, DARK_GREEN);
				return G_SUCCESS;
			}
			Screen_Draw_HeaderJustified(135, "Done", 4, &Font12, LCD_BACKGROUND);
			Screen_Draw_HeaderJustified(135, "Initializing", 12, &Font12, LCD_BACKGROUND);
			Screen_Draw_HeaderJustified(150, "Please wait", 11, &Font8, LCD_BACKGROUND);
			break;
	}

	return G_SUCCESS;
}


void Screen_DrawInfoScreen()
{
	//LCD clear
	LCD_Clear(BLACK);

	// ====================================
	// Header
	// ====================================
	Screen_Draw_HeaderJustified(5, "Information panel", 17, &Font12, WHITE);

	// Separator lines
	LCD_DrawLine(0, 25, 128, 25, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);
	LCD_DrawLine(0, 30, 128, 30, PURPLE, LINE_SOLID, DOT_PIXEL_2X2);

	// ====================================
	// Servo panel
	// ====================================
	Screen_Draw_HeaderJustified(SERVO_INFO_BLOCK_OFF_Y, "Servo angles [deg]", 18, &Font8, WHITE);

	for (int i = 0; i < 2; i++)
	{
		LCD_DisplayString(5 + i*69, SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET, "Ref: ", &Font8, BLACK, WHITE);
		LCD_DisplayString(5 + i*69, SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET + INFO_OFFSET, "Act: ", &Font8, BLACK, WHITE);
	}

	// Column separator
	LCD_DrawLine(64, SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET, 64, SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET + INFO_OFFSET + 8, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);

	// Panel separator
	LCD_DrawLine(0, SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET + INFO_OFFSET + 15, 128, SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET + INFO_OFFSET + 15, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);

	// ====================================
	// Module orientation panel
	// ====================================
	Screen_Draw_HeaderJustified(ACC_INFO_BLOCK_OFF_Y, "Module orientation [deg]", 24, &Font8, WHITE);

	for (int i = 0; i < 2; i++)
	{
		LCD_DisplayString(5 + i*69, ACC_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET, "A: ", &Font8, BLACK, WHITE);
		LCD_DisplayString(5 + i*69, ACC_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET + INFO_OFFSET, "B: ", &Font8, BLACK, WHITE);
		LCD_DisplayString(5 + i*69, ACC_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET + 2 * INFO_OFFSET, "C: ", &Font8, BLACK, WHITE);
	}
	// Column separator
	LCD_DrawLine(64, ACC_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET, 64, ACC_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET + 2*INFO_OFFSET + 8, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);

	// Status panel
	LCD_DrawLine(0, 140, 128, 140, PURPLE, LINE_SOLID, DOT_PIXEL_1X1);
		LCD_DrawLine(0, 143, 128, 143, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);

	// ====================================
	// Status panel
	// ====================================
	LCD_DisplayString(5, STATUS_INFO_OFF_Y, "S1", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(25, STATUS_INFO_OFF_Y, "S2", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(45, STATUS_INFO_OFF_Y, "A1", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(65, STATUS_INFO_OFF_Y, "A2", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(85, STATUS_INFO_OFF_Y, "SD", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(105, STATUS_INFO_OFF_Y, "CAN", &Font8, BLACK, DARK_GREEN);

	current_screen = INFO_SCREEN;
}

ReturnCode Screen_UpdateStatus(PERIPHERAL per, uint8_t is_ok)
{
	// Check if displayed panel is correct
	if (current_screen != INFO_SCREEN)
		return G_ERROR;

	uint16_t color = is_ok == 0x01 ? DARK_GREEN : DARK_RED;
	switch(per)
	{
		case SERVO_0:
			LCD_DisplayString(5, STATUS_INFO_OFF_Y, "S1", &Font8, BLACK, color);
			break;
		case SERVO_1:
			LCD_DisplayString(25, STATUS_INFO_OFF_Y, "S2", &Font8, BLACK, color);
			break;
		case ACC_0:
			LCD_DisplayString(45, STATUS_INFO_OFF_Y, "A1", &Font8, BLACK, color);
			break;
		case ACC_1:
			LCD_DisplayString(65, STATUS_INFO_OFF_Y, "A2", &Font8, BLACK, color);
			break;
		case SD:
			LCD_DisplayString(85, STATUS_INFO_OFF_Y, "SD", &Font8, BLACK, color);
			break;
		case CAN:
			LCD_DisplayString(105, STATUS_INFO_OFF_Y, "CAN", &Font8, BLACK, color);
			break;
		default:
			return G_ERROR;
	}

	return G_SUCCESS;
}

ReturnCode Screen_UpdateData(PERIPHERAL per, float* data, uint8_t length)
{
	// Check if displayed panel is correct
	if (current_screen != INFO_SCREEN)
		return G_ERROR;

	// Get position to which data should be written to
	uint8_t x, y;
	switch(per)
	{
		case SERVO_0:
			x = SERVO_INFO_DATA_OFF;
			y = SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET;
			break;
		case SERVO_1:
			x = SERVO_INFO_DATA_OFF + 69;
			y = SERVO_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET;
			break;
		case ACC_0:
			x = ACC_INFO_DATA_OFF;
			y = ACC_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET;
			break;
		case ACC_1:
			x = ACC_INFO_DATA_OFF + 69;
			y = ACC_INFO_BLOCK_OFF_Y + INFO_BIG_OFFSET;
			break;
		default:
			return G_ERROR;
	}

	// Display value
	char buff[7];
	for(int i = 0; i < length; i++)
	{
		snprintf(buff, sizeof(buff), "%2.2f", *(data + i));

		// Offset if there is '-' sign additionally
		if( *(data + i) < 0)
			LCD_DisplayString(x - 5, y + i * INFO_OFFSET, buff, &Font8, BLACK, WHITE);
		else
			LCD_DisplayString(x, y + i * INFO_OFFSET, buff, &Font8, BLACK, WHITE);
	}

	return G_SUCCESS;
}




