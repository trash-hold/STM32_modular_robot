/*
 * error_codes.h
 *
 *  Created on: Dec 17, 2024
 *      Author: Barabash
 */

#ifndef INC_ERROR_CODES_H_
#define INC_ERROR_CODES_H_


typedef enum ReturnCode{
	G_ERROR = 0x00,
	G_SUCCESS = 0x01,
	C_MOUNT_ERROR = 0x02,
	C_RTC_ERROR = 0x05,
	G_FILE_WRITE = 0x03,
	G_FILE_READ = 0x04,
	G_COM_OVERFLOW = 0x05,
	G_COM_TRANSMIT = 0x06,
	G_COM_RECEIVE = 0x07
}ReturnCode;


#endif /* INC_ERROR_CODES_H_ */
