/*
 * BMP280.h
 *
 *  Created on: Oct 3, 2020
 *      Author: Clément
 */

#ifndef SRC_BMP280_SIMPLE_H_
#define SRC_BMP280_SIMPLE_H_

typedef uint32_t BMP280_U32_t;
typedef int32_t BMP280_S32_t;
typedef int64_t BMP280_S64_t;

//uint8_t temp_calib[3];
//uint8_t pres_calib[3];

static const uint8_t BMP280_ADDR = 0x77 << 1;

static const uint8_t BMP280_ID_REG = 0xD0;
static const uint8_t BMP280_ID_LEN = 1;
static const uint8_t BMP280_ID_VAL = 0x58;

static const uint8_t BMP280_CTRL_MEAS_REG = 0xF4;

static const uint8_t BMP280_PRES_REG_MSB = 0xF7;
static const uint8_t BMP280_PRES_LEN = 3;

static const uint8_t BMP280_TEMP_REG_MSB = 0xFA;
static const uint8_t BMP280_TEMP_LEN = 3;

static const uint8_t BMP280_TRIM_REG_MSB = 0x88;
static const uint8_t BMP280_TRIM_LEN = 12 * 2;

//uint8_t temp_calib[3];
//uint8_t pres_calib[3];

typedef enum bmp280_address_enum {
    BMP280_ADDR_DIG_T1 = 0x88,
    BMP280_ADDR_DIG_T2 = 0x8A,
    BMP280_ADDR_DIG_T3 = 0x8C,
    BMP280_ADDR_DIG_P1 = 0x8E,
    BMP280_ADDR_DIG_P2 = 0x90,
    BMP280_ADDR_DIG_P3 = 0x92,
    BMP280_ADDR_DIG_P4 = 0x94,
    BMP280_ADDR_DIG_P5 = 0x96,
    BMP280_ADDR_DIG_P6 = 0x98,
    BMP280_ADDR_DIG_P7 = 0x9A,
    BMP280_ADDR_DIG_P8 = 0x9C,
    BMP280_ADDR_DIG_P9 = 0x9E,
    BMP280_ADDR_CTRL_MEAS  = 0xF4,
    BMP280_ADDR_CONFIG     = 0xF5,
    BMP280_ADDR_PRESS_MSB  = 0xF7,
    BMP280_ADDR_TEMP_MSB   = 0xFA,
} bmp280_address_t;

int BMP280_check();
int BMP280_init();
BMP280_S32_t BMP280_get_temperature();
int BMP280_get_pressure();
uint8_t* BMP280_Read_Reg(uint8_t reg, uint8_t value);
int BMP280_Write_Reg(uint8_t reg, uint8_t value);

BMP280_S32_t BMP280_compensate_temp(BMP280_S32_t adc_T);
BMP280_S32_t BMP280_compensate_press(BMP280_S32_t adc_P);

#endif /* SRC_BMP280_H_ */
