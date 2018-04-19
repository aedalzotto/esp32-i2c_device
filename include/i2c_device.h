/**
 * @file i2c_device.h
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * Gabriel Boni Vicari (133192@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright 2018 Angelo Elias Dalzotto & Gabriel Boni Vicari
 * 
 * @brief This is an easy I2C C++ Library for ESP32 ESP-IDF.
 */

#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#if CONFIG_CXX_EXCEPTIONS != 1
#error "C++ exception handling must be enabled within make menuconfig. See Compiler Options > Enable C++ Exceptions."
#endif

#include <exception>
#include <driver/i2c.h>

#include "i2c_exceptions.h"

class I2CDevice {
public:
	/**
	 * @brief Class constructor.
	 * 
	 * @param dev_addr Device I2C address.
	 * @param i2c_num I2C port number. Default to port 0.
	 */
	I2CDevice(uint8_t dev_addr, i2c_port_t i2c_num = I2C_NUM_0);

	/**
	 * @brief Reads a byte register from I2C device.
	 * 
	 * @param reg_addr Device register to read.
	 * 
	 * @return Byte read from register.
	 * 
	 * @throw
	 *     - I2CExcept::CommandFailed Case fail to execute I2C transmission.
	 */
	uint8_t read_register(uint8_t reg_addr);

	/**
	 * @brief Reads a buffer from I2C device.
	 * 
	 * @param reg_addr Device register to read.
	 * @param *buffer Pointer to destination buffer.
	 * @param size Size of the buffer in bytes.
	 * 
	 * @throw
	 *     - I2CExcept::BufferSize Case "size" param is 0.
	 *     - I2CExcept::CommandFailed Case fail to execute I2C transmission.
	 */
	void read_buffer(uint8_t reg_addr, uint8_t *buffer, uint8_t size);

	/**
	 * @brief Modify a I2C Device register.
	 * 
	 * @details Reads a register and the masks it to write back to device.
	 * 
	 * @param reg_addr Device register to modify.
	 * @param and_mask AND mask to be applied to register (bits 0 to disable).
	 * @param or_mark OR mask to be applied AFTER AND. (bits 1 to enable)
	 * 
	 * @throw
	 *     - I2CExcept::CommandFailed Case fail to execute I2C transmission.
	 */
	void modify_register(uint8_t reg_addr, uint8_t and_mask, uint8_t or_mask);

protected:
	i2c_port_t i2c_num;

private:
	uint8_t dev_addr;

};

#endif