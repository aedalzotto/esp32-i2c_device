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
 * @brief I2C Device C++ Library for ESP32 implementation.
 */

#include "i2c_device.h"

I2CDevice::I2CDevice(uint8_t dev_addr, i2c_port_t i2c_num = I2C_NUM_0)
{
    this->dev_addr = dev_addr;
    this->i2c_num = i2c_num;
}

uint8_t I2CDevice::read_register(uint8_t reg_addr)
{
    uint8_t reg;
    try {
        read_buffer(reg_addr, &reg, 1);
        return reg;
    } catch(...){
        throw;
    }
}

void I2CDevice::read_buffer(uint8_t reg_addr, uint8_t *buffer, uint8_t size)
{
    if(!size)
        throw I2CExcept::BufferSize();

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_write_byte(cmd, reg_addr, true);

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);

    if(size > 1)
        i2c_master_read(cmd, buffer, size-1, I2C_MASTER_ACK);

    i2c_master_read_byte(cmd, buffer+size-1, I2C_MASTER_NACK);

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

void I2CDevice::write_register(uint8_t reg_addr, uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);

    i2c_master_write_byte(cmd, data, I2C_MASTER_NACK);

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if(ret != ESP_OK)
        throw I2CExcept::CommandFailed();
}

void I2CDevice::write_buffer(uint8_t reg_addr, uint8_t *data, uint8_t size)
{
    if(!size)
        throw I2CExcept::BufferSize();

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);

	if(size > 1)
        i2c_master_write(cmd, data, size-1, I2C_MASTER_ACK);

    i2c_master_write_byte(cmd, *(data+size-1), I2C_MASTER_NACK);

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if(ret != ESP_OK)
        throw I2CExcept::CommandFailed();
}