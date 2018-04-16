/**
 * @file i2c_utils.c
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

#include "i2c_utils.h"

esp_err_t i2c_read_byte(i2c_port_t i2c_num, uint8_t dev_addr,
									uint8_t reg_addr, uint8_t *data)
{
    return i2c_read_bytes(i2c_num, dev_addr, reg_addr, data, 1);
}

esp_err_t i2c_read_bytes(i2c_port_t i2c_num, uint8_t dev_addr, 
                                uint8_t reg_addr, uint8_t *data, size_t size)
{
	if(!size)
        return ESP_OK;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_write_byte(cmd, reg_addr, true);

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);

    if(size > 1)
        i2c_master_read(cmd, data, size-1, 0); //0 is ACK

    i2c_master_read_byte(cmd, data+size-1, 1); //1 is NACK

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t i2c_write_byte(i2c_port_t i2c_num, uint8_t dev_addr,
									uint8_t reg_addr, uint8_t data)
{
	return i2c_write_bytes(i2c_num, dev_addr, reg_addr, &data, 1);
}

esp_err_t i2c_write_bytes(i2c_port_t i2c_num, uint8_t dev_addr, 
                                uint8_t reg_addr, uint8_t *data, size_t size)
{
	// start transmission to device
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_num << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true); // send register address

	if(size > 1)
        i2c_master_write(cmd, data, size-1, 0); //0 is ACK

    i2c_master_write_byte(cmd, *(data+size-1), 1);

    // end transmission
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t i2c_read_bit(i2c_port_t i2c_num, uint8_t dev_addr, uint8_t reg_addr,
                                				uint8_t *data, uint8_t offset)
{
	return i2c_read_bits(i2c_num, dev_addr, reg_addr, data, offset, 1);
}

esp_err_t i2c_read_bits(i2c_port_t i2c_num, uint8_t dev_addr, uint8_t reg_addr,
                                uint8_t *data, uint8_t offset, uint8_t length)
{
	*data = 0;

	esp_err_t ret;
	ret = i2c_read_byte(i2c_num, dev_addr, reg_addr, data);
	if(ret != 1)
		return ret;

	*data &= ((1 << length) - 1) << (offset - length + 1); // Mask
	*data >>= (offset - length + 1); // Set to first bits

	return 1;
}


esp_err_t i2c_write_bits(i2c_port_t i2c_num, uint8_t dev_addr,
								uint8_t reg_addr, uint8_t mask, bool enable)
{
	esp_err_t ret;
	uint8_t reg;
	ret = i2c_read_byte(i2c_num, dev_addr, reg_addr, &reg);
	if(ret != 1)
		return ret;

	if(enable)
		reg |= mask;
	else
		reg &= ~(mask);

	return i2c_write_byte(i2c_num, dev_addr, reg_addr, reg);
}

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