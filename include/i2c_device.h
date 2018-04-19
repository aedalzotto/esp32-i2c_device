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

namespace I2CExcept {
	class CommandFailed : public std::exception {
    	const char *what() const throw() {
        	return "Failed to execute I2C communication command";
    	}
	};
}

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
	 *     - i2c_command_failed Case fail to execute I2C transmission.
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
	 *     - i2c_command_failed Case fail to execute I2C transmission.
	 */
	void read_buffer(uint8_t reg_addr, uint8_t *buffer, uint8_t size);

	/**
	 * @brief Writes a byte to an I2C device's register.
	 * 
	 * @param reg_addr Device's register to write.
	 * @param data Byte that will be written.
	 * 
	 * @throw
	 *     - i2c_command_failed Case fail to execute I2C transmission.
	 */
	void write_register(uint8_t reg_addr, uint8_t data);

	/**
	 * @brief Writes a buffer to an I2C device register.
	 * 
	 * @param reg_addr Device register to read.
	 * @param *data Buffer that will be written.
	 * @param size Buffer size to write.
	 * 
	 * @throw
	 *     - i2c_command_failed Case fail to execute I2C transmission.
	 */
	void write_buffer(uint8_t reg_addr, uint8_t *data, uint8_t size);

protected:
	i2c_port_t i2c_num;

private:
	uint8_t dev_addr;

};

#endif