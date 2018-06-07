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
 * @brief Exceptions for I2C Library.
 */

#ifndef I2C_DEVICE_EXCEPTIONS_H_
#define I2C_DEVICE_EXCEPTIONS_H_

#if CONFIG_CXX_EXCEPTIONS != 1
#error "C++ exception handling must be enabled within make menuconfig. See Compiler Options > Enable C++ Exceptions."
#endif

#include <exception>

namespace I2CExcept {
	class CommandFailed : public std::exception {
    	const char *what() const throw() {
        	return "Failed to execute I2C communication command";
    	}
	};
    class BufferSize : public std::exception {
        const char *what() const throw() {
        	return "Invalid buffer size passed to function";
    	}
    };
}

#endif