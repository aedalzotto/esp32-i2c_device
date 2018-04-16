/**
 * @file i2c_utils.h
 * 
 * @author
 * Gabriel Boni Vicari (133192@upf.br)
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (c) 2012 Jeff Rowberg.
 * 
 * @brief I2C Read/Write functions for ESP32 ESP-IDF.
 *
 * @details Based on I2Cdevlib's ESP32_ESP-IDF by Dariusz Kempa.
 * I2Cdevlib's ESP32_ESP-IDF is based on I2Cdevlib's EFM32 by Nicolas Baldeck.
 * I2Cdevlib's EFM32 is based on I2Cdevlib's Arduino by Jeff Rowberg.
*/

#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include <driver/i2c.h>

/**
 * @brief Reads a device register of 1 byte via i2c.
 * 
 * @param i2c_num is the i2c port the device is connected to.
 * @param dev_addr is the device i2c address.
 * @param reg_addr is the device register address to read.
 * @param data is the address of the variable the register will be saved.
 * 
 * @returns
 *     ESP_FAIL(-1) Parameter error
 *     Others(>=0) The number of data bytes that pushed to the I2C slave buffer
*/
esp_err_t i2c_read_byte(i2c_port_t i2c_num, uint8_t dev_addr, 
                                    uint8_t reg_addr, uint8_t *data);

/**
 * @brief Read multiple bytes via i2c.
 * 
 * @param i2c_num is the i2c port the device is connected to.
 * @param dev_addr is the device i2c address.
 * @param reg_addr is the device register address to read.
 * @param *data is the address of the variable the register will be saved.
 * @param size is the number of bytes to be read.
 * 
 * @returns
 *     ESP_FAIL(-1) Parameter error
 *     Others(>=0) The number of data bytes that pushed to the I2C slave buffer
*/
esp_err_t i2c_read_bytes(i2c_port_t i2c_num, uint8_t dev_addr, 
                                uint8_t reg_addr, uint8_t *data, size_t size);

/**
 * @brief Write one byte via i2c.
 * 
 * @param i2c_num is the i2c port the device is connected to.
 * @param dev_addr is the device i2c address.
 * @param reg_addr is the device register address to write.
 * @param data is the value to be written.
 * 
 * @returns
 *     ESP_FAIL(-1) Parameter error
 *     Others(>=0) The number of data bytes that pushed to the I2C slave buffer
*/
esp_err_t i2c_write_byte(i2c_port_t i2c_num, uint8_t dev_addr,
									uint8_t reg_addr, uint8_t data);

/**
 * @brief Write multiple bytes via i2c.
 * 
 * @param i2c_num is the i2c port the device is connected to.
 * @param dev_addr is the device i2c address.
 * @param reg_addr is the device register address to write.
 * @param *data is the address of the variable to be written.
 * @param size is the number of bytes to write
 * 
 * @returns
 *     ESP_FAIL(-1) Parameter error
 *     Others(>=0) The number of data bytes that pushed to the I2C slave buffer
*/
esp_err_t i2c_write_bytes(i2c_port_t i2c_num, uint8_t dev_addr, 
                                uint8_t reg_addr, uint8_t *data, size_t size);

/**
 * @brief Read a bit from a register.
 * 
 * @details The byte read is masked with the offset desired and then
 * shifted to the least significant bit.
 * 
 * @param i2c_num is the i2c port the device is connected to.
 * @param dev_addr is the device i2c address.
 * @param reg_addr is the device register address to read.
 * @param *data is the address of the variable to be read.
 * @param offset is the bit offset to read.
 * 
 * @returns
 *     ESP_FAIL(-1) Parameter error
 *     Others(>=0) The number of data bytes that pushed to the I2C slave buffer
*/
esp_err_t i2c_read_bit(i2c_port_t i2c_num, uint8_t dev_addr, uint8_t reg_addr,
                                                uint8_t *data, uint8_t offset);

/**
 * @brief Read a number of bits from a register.
 * 
 * @details The byte read is masked with the size and offset desired and then
 * shifted to the least significant bits.
 * 
 * @param i2c_num is the i2c port the device is connected to.
 * @param dev_addr is the device i2c address.
 * @param reg_addr is the device register address to read.
 * @param *data is the address of the variable to be read.
 * @param offset is the most significant bit to read.
 * @param length is the number of bits to read.
 * 
 * @returns
 *     ESP_FAIL(-1) Parameter error
 *     Others(>=0) The number of data bytes that pushed to the I2C slave buffer
*/
esp_err_t i2c_read_bits(i2c_port_t i2c_num, uint8_t dev_addr, uint8_t reg_addr,
                                uint8_t *data, uint8_t offset, uint8_t length);

/**
 * @brief Masks a byte register via i2c.
 * 
 * @details The function reads the register and applies the mask to enable or
 * disable the bits. The "mask" param must be 0b00001111 to set the last 4 bits
 * in case "enable" param is true, or to disable the last 4 bits in case
 * "enable" param is false.
 * 
 * @param i2c_num is the i2c port the device is connected to.
 * @param dev_addr is the device i2c address.
 * @param reg_addr is the device register address to modify.
 * @param mask is the byte mask to enable or disable.
 * @param enable is if the mask '1's will be enabled or disabled in the
 * register.
 * 
 * @returns
 *     ESP_FAIL(-1) Parameter error
 *     Others(>=0) The number of data bytes that pushed to the I2C slave buffer
*/
esp_err_t i2c_write_bits(i2c_port_t i2c_num, uint8_t dev_addr,
								uint8_t reg_addr, uint8_t mask, bool enable);

#endif

/**
 * -----------------------------------------------------------------------------
 * I2Cdev device library code is placed under the MIT license.
 * 
 * Copyright (c) 2012 Jeff Rowberg
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * -----------------------------------------------------------------------------
*/