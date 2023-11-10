/*
 * drv_bmp280.h
 *
 *      Author: [Clément Coutouillat]
 */

#ifndef DRV_BMP280_H_
#define DRV_BMP280_H_

#include <stdint.h>

typedef enum bmp280_command_enum {
    BMP280_CMD_READ       = 0x80,
    BMP280_CMD_WRITE      = 0x00,
    BMP280_CMD_MULTIBYTE  = 0x40,
    BMP280_CMD_SINGLEBYTE = 0x00
} bmp280_command_t;


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


typedef enum bmp280_mode_enum {
    BMP280_MODE_SLEEP = 0x00,
    BMP280_MODE_FORCED = 0x01,
    BMP280_MODE_NORMAL = 0x03
} bmp280_mode_t;


typedef enum bmp280_oversampling_enum {
    BMP280_OVERSAMPLING_SKIPPED = 0x00,
    BMP280_OVERSAMPLING_1X = 0x01,
    BMP280_OVERSAMPLING_2X = 0x02,
    BMP280_OVERSAMPLING_4X = 0x03,
    BMP280_OVERSAMPLING_8X = 0x04,
    BMP280_OVERSAMPLING_16X = 0x05
} bmp280_oversampling_t;


typedef enum bmp280_filter_enum {
    BMP280_FILTER_OFF = 0x00,
    BMP280_FILTER_2   = 0x01,
    BMP280_FILTER_4   = 0x02,
    BMP280_FILTER_8   = 0x03,
    BMP280_FILTER_16  = 0x04
} bmp280_filter_t;


typedef int (* bmp280_transmit_drv_t)(uint8_t address, uint8_t *p_data, uint16_t size);
typedef int (* bmp280_receive_drv_t)(uint8_t address, uint8_t *p_data, uint16_t size);


typedef struct bmp280_serial_drv_struct {
    bmp280_transmit_drv_t transmit;
    bmp280_receive_drv_t receive;
} bmp280_serial_drv_t;


typedef struct h_bmp280_struct {
    // Mode
    bmp280_mode_t mode;

    // Oversampling
    bmp280_oversampling_t oversampling_pressure;
    bmp280_oversampling_t oversampling_temperature;

    // Filter
    bmp280_filter_t filter;

    // Standby duration
    bmp280_standby_t standby;

    // Calibration data
    h_bmp280_calib_data_t calib_data;

    // driver I2C
    bmp280_serial_drv_t serial_drv;

    // Temperature et pression
    int32_t temperature;
    uint32_t pressure;

    // Mesure dispo
    uint8_t data_available;

} h_bmp280_t;
