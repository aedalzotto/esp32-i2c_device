/*
 * @files esp32_i2c_rw.c and esp32_i2c_rw.h
 * @author Gabriel Boni Vicari (133192@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * @brief I2C Read/Write functions for ESP32 ESP-IDF.
 *
 * Based on I2Cdevlib's ESP32_ESP-IDF by Dariusz Kempa.
 * I2Cdevlib's ESP32_ESP-IDF is based on I2Cdevlib's EFM32 by Nicolas Baldeck.
 * I2Cdevlib's EFM32 is based on I2Cdevlib's Arduino by Jeff Rowberg.
 */

/*
--------------------------------------------------------------------------------
I2Cdev device library code is placed under the MIT license.

Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------
*/

#ifndef ESP32_I2C_RW_H
#define ESP32_I2C_RW_H

#include <driver/i2c.h>

void select_register (uint8_t device_address, uint8_t register_address);

int8_t esp32_i2c_read_bytes (uint8_t device_address, uint8_t register_address,
uint8_t size, uint8_t* data);

int8_t esp32_i2c_read_byte (uint8_t device_address, uint8_t register_address,
uint8_t* data);

int8_t esp32_i2c_read_bits (uint8_t device_address, uint8_t register_address,
uint8_t bit_start, uint8_t size, uint8_t* data);

int8_t esp32_i2c_read_bit (uint8_t device_address, uint8_t register_address,
uint8_t bit_number, uint8_t* data);

bool esp32_i2c_write_bytes (uint8_t device_address, uint8_t register_address,
uint8_t size, uint8_t* data);

bool esp32_i2c_write_byte (uint8_t device_address, uint8_t register_address,
uint8_t data);

bool esp32_i2c_write_bits (uint8_t device_address, uint8_t register_address,
uint8_t bit_start, uint8_t size, uint8_t data);

bool esp32_i2c_write_bit (uint8_t device_address, uint8_t register_address,
uint8_t bit_number, uint8_t data);

int8_t esp32_i2c_write_word (uint8_t device_address, uint8_t register_address,
uint8_t data);
#endif
