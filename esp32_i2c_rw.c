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

#include <esp_log.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp32_i2c_rw.h"

#define I2C_NUM I2C_NUM_0

/*
 * @brief Select the register in the device where data will be read from.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the first register to read from.
 */
void select_register (uint8_t device_address, uint8_t register_address)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
	ESP_ERROR_CHECK (i2c_master_start (cmd));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, (device_address << 1)
	| I2C_MASTER_WRITE, 1));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, register_address, 1));
	ESP_ERROR_CHECK (i2c_master_stop (cmd));
	ESP_ERROR_CHECK (i2c_master_cmd_begin (I2C_NUM, cmd, 1000
	/ portTICK_PERIOD_MS));
	i2c_cmd_link_delete (cmd);
}

/*
 * @brief Read multiple bytes from 8-bit registers.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the first register to read from.
 * @param size: Number of registers to read.
 * @param data: Buffer to store the read data in.
 * @return Status of read operation.
 */
int8_t esp32_i2c_read_bytes (uint8_t device_address, uint8_t register_address,
uint8_t size, uint8_t* data)
{
	select_register (device_address, register_address);
	i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
	ESP_ERROR_CHECK (i2c_master_start (cmd));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, (device_address << 1)
	| I2C_MASTER_READ, 1));

	if (size > 1) {ESP_ERROR_CHECK (i2c_master_read (cmd, data, size - 1, 0));}

	ESP_ERROR_CHECK (i2c_master_read_byte (cmd, data + size - 1, 1));

	ESP_ERROR_CHECK (i2c_master_stop (cmd));
	ESP_ERROR_CHECK (i2c_master_cmd_begin (I2C_NUM, cmd, 1000
	/ portTICK_PERIOD_MS));
	i2c_cmd_link_delete (cmd);

	return (size);
}

/*
 * @brief Read single byte from an 8-bit register.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the register to read from.
 * @param data: Container to store the byte read from register.
 * @return Status of read operation.
 */
int8_t esp32_i2c_read_byte (uint8_t device_address, uint8_t register_address,
uint8_t* data)
{
	return (esp32_i2c_read_bytes (device_address, register_address, 1, data));
}

/*
 * @brief Read multiple bits from an 8-bit register.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the register to read from.
 * @param bit_start: First bit position to read (0-7).
 * @param size: Number of bits to read (Not more than 8).
 * @param data: Container to store the right-aligned value.
 * @return Status of read operation.
 */
int8_t esp32_i2c_read_bits (uint8_t device_address, uint8_t register_address,
uint8_t bit_start, uint8_t size, uint8_t* data)
{
	// Example:
	// 01101001 - Read byte.
	// 76543210 - Bit numbers.
	//    xxx   - Arguments: bit_start = 4, size = 3.
	//    010   - Mask.
	//    ->010 - Right shift.
	uint8_t bit;
	uint8_t count;

	if ((count = esp32_i2c_read_byte (device_address, register_address, &bit))
	!= 0)
	{
		uint8_t mask = ((1 << size) - 1) << (bit_start - size + 1);

		bit &= mask;
		bit >>= (bit_start - size + 1);
		*data = bit;
	}

	return (count);
}

/*
 * @brief Read single bit from an 8-bit register.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the register to read from.
 * @param bit_number: Bit position to read (0-7).
 * @param data: Container to store the bit.
 * @return Status of read operation.
 */
int8_t esp32_i2c_read_bit (uint8_t device_address, uint8_t register_address,
uint8_t bit_number, uint8_t* data)
{
	uint8_t bit;
	uint8_t count = esp32_i2c_read_byte (device_address, register_address,
	&bit);

	*data = bit & (1 << bit_number);

	return (count);
}

/*
 * @brief Write multiple bytes to 8-bit registers.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the first register to write to.
 * @param size: Number of bytes to write.
 * @param data: Array of bytes to write.
 * @return Status of write operation.
 */
bool esp32_i2c_write_bytes (uint8_t device_address, uint8_t register_address,
uint8_t size, uint8_t* data)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
	ESP_ERROR_CHECK (i2c_master_start (cmd));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, (device_address << 1)
	| I2C_MASTER_WRITE, 1));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, register_address, 1));
	ESP_ERROR_CHECK (i2c_master_write (cmd, data, size - 1, 0));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, data [size - 1], 1));
	ESP_ERROR_CHECK (i2c_master_stop (cmd));
	ESP_ERROR_CHECK (i2c_master_cmd_begin (I2C_NUM, cmd, 1000
	/ portTICK_PERIOD_MS));
	i2c_cmd_link_delete (cmd);

	return (true);
}

/*
 * @brief Write single byte to an 8-bit register.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the register to write to.
 * @param data: Array of bytes to write.
 * @return Status of write operation.
 */
bool esp32_i2c_write_byte (uint8_t device_address, uint8_t register_address,
uint8_t data)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
	ESP_ERROR_CHECK (i2c_master_start (cmd));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, (device_address << 1)
	| I2C_MASTER_WRITE, 1));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, register_address, 1));
	ESP_ERROR_CHECK (i2c_master_write_byte (cmd, data, 1));
	ESP_ERROR_CHECK (i2c_master_stop (cmd));
	ESP_ERROR_CHECK (i2c_master_cmd_begin (I2C_NUM, cmd, 1000
	/ portTICK_PERIOD_MS));
	i2c_cmd_link_delete (cmd);

	return (true);
}

/*
 * @brief Write multiple bits to an 8-bit register.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the register to write to.
 * @param bit_start: First bit position to write (0-7).
 * @param size: Number of bits to write (Not more than 8).
 * @param data: Right-aligned value to write.
 * @return Status of write operation.
 */
bool esp32_i2c_write_bits (uint8_t device_address, uint8_t register_address,
uint8_t bit_start, uint8_t size, uint8_t data)
{
	uint8_t bit = 0;

	if (esp32_i2c_read_byte (device_address, register_address, &bit) != 0)
	{
		uint8_t mask = ((1 << size) - 1) << (bit_start - size + 1);
		data <<= (bit_start - size + 1); // Shift data into correct position.
		data &= mask; // Zero all non-important bits in data.
		bit &= ~(mask); // Zero all important bits in existing byte.
		bit |= data; // Combine data with existing byte.

		return (esp32_i2c_write_byte (device_address, register_address, bit));
	}
	else {return (false);}
}

/*
 * @brief Write single bit to an 8-bit register.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the register to write to.
 * @param bit_number: Bit position to write (0-7).
 * @param data: Bit value to write.
 * @return Status of write operation.
 */
bool esp32_i2c_write_bit (uint8_t device_address, uint8_t register_address,
uint8_t bit_number, uint8_t data)
{
	uint8_t bit;

	esp32_i2c_read_byte (device_address, register_address, &bit);

	if (data != 0) {bit = (bit | (1 << bit_number));}
	else {bit = (bit & ~(1 << bit_number));}

	return (esp32_i2c_write_byte (device_address, register_address, bit));
}

/*
 * @brief Write word to an 8-bit register.
 *
 * @param device_address: I2C slave device address.
 * @param register_address: Address of the register to write to.
 * @param data: Word to write.
 * @return Status of write operation.
 */
int8_t esp32_i2c_write_word (uint8_t device_address, uint8_t register_address,
uint8_t data)
{
	uint8_t data_1 [] = {(uint8_t) (data >> 8), (uint8_t) (data & 0xFF)};
	// uint8_t data_2 [] = {(uint8_t) (data & 0xFF), (uint8_t) (data >> 8)};

	esp32_i2c_write_bytes (device_address, register_address, 2, data_1);

	return (1);
}
