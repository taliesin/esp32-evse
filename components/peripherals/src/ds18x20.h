/*
 * Copyright (c) 2016 Grzegorz Hetman <ghetman@gmail.com>
 * Copyright (c) 2016 Alex Stewart <foogod@gmail.com>
 * Copyright (c) 2018 Ruslan V. Uss <unclerus@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DS18X20_H
#define _DS18X20_H

#include <esp_err.h>

#include "onewire.h"

typedef onewire_addr_t ds18x20_addr_t;

/** An address value which can be used to indicate "any device on the bus" */
#define DS18X20_ANY ONEWIRE_NONE

/** Family ID (lower address byte) of DS18B20 sensors */
#define DS18B20_FAMILY_ID 0x28

/** Family ID (lower address byte) of DS18S20 sensors */
#define DS18S20_FAMILY_ID 0x10

/**
 * @brief Find the addresses of all ds18x20 devices on the bus.
 *
 * Scans the bus for all devices and places their addresses in the supplied
 * array. If there are more than `addr_count` devices on the bus, only the
 * first `addr_count` are recorded.
 *
 * @param pin         The GPIO pin connected to the ds18x20 bus
 * @param addr_list   A pointer to an array of ::ds18x20_addr_t values.
 *                    This will be populated with the addresses of the found
 *                    devices.
 * @param addr_count  Number of slots in the `addr_list` array. At most this
 *                    many addresses will be returned.
 * @param found       The number of devices found. Note that this may be less
 *                    than, equal to, or more than `addr_count`, depending on
 *                    how many ds18x20 devices are attached to the bus.
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18x20_scan_devices(gpio_num_t pin, ds18x20_addr_t *addr_list, size_t addr_count, size_t *found);

/**
 * @brief Tell one or more sensors to perform a temperature measurement and
 * conversion (CONVERT_T) operation.
 *
 * This operation can take up to 750ms to complete.
 *
 * If `wait=true`, this routine will automatically drive the pin high for the
 * necessary 750ms after issuing the command to ensure parasitically-powered
 * devices have enough power to perform the conversion operation (for
 * non-parasitically-powered devices, this is not necessary but does not
 * hurt). If `wait=false`, this routine will drive the pin high, but will
 * then return immediately. It is up to the caller to wait the requisite time
 * and then depower the bus using onewire_depower() or by issuing another
 * command once conversion is done.
 *
 * @param pin   The GPIO pin connected to the ds18x20 device
 * @param addr  The 64-bit address of the device on the bus. This can be set
 *              to ::DS18X20_ANY to send the command to all devices on the bus
 *              at the same time.
 * @param wait  Whether to wait for the necessary 750ms for the ds18x20 to
 *              finish performing the conversion before returning to the
 *              caller (You will normally want to do this).
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18x20_measure(gpio_num_t pin, ds18x20_addr_t addr, bool wait);

/**
 * @brief Read the value from the last CONVERT_T operation.
 *
 * This should be called after ds18x20_measure() to fetch the result of the
 * temperature measurement.
 *
 * @param pin         The GPIO pin connected to the ds18x20 device
 * @param addr        The 64-bit address of the device to read. This can be set
 *                    to ::DS18X20_ANY to read any device on the bus (but note
 *                    that this will only work if there is exactly one device
 *                    connected, or they will corrupt each others' transmissions)
 * @param temperature The temperature in degrees Celsius
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18x20_read_temperature(gpio_num_t pin, ds18x20_addr_t addr, int16_t *temperature);

/**
 * @brief Read the value from the last CONVERT_T operation (ds18b20 version).
 *
 * This should be called after ds18x20_measure() to fetch the result of the
 * temperature measurement.
 *
 * @param pin         The GPIO pin connected to the ds18x20 device
 * @param addr        The 64-bit address of the device to read. This can be set
 *                    to ::DS18X20_ANY to read any device on the bus (but note
 *                    that this will only work if there is exactly one device
 *                    connected, or they will corrupt each others' transmissions)
 * @param temperature The temperature in degrees Celsius
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18b20_read_temperature(gpio_num_t pin, ds18x20_addr_t addr, int16_t *temperature);

/**
 * @brief Read the value from the last CONVERT_T operation (ds18s20 version).
 *
 * This should be called after ds18x20_measure() to fetch the result of the
 * temperature measurement.
 *
 * @param pin         The GPIO pin connected to the ds18x20 device
 * @param addr        The 64-bit address of the device to read. This can be set
 *                    to ::DS18X20_ANY to read any device on the bus (but note
 *                    that this will only work if there is exactly one device
 *                    connected, or they will corrupt each others' transmissions)
 * @param temperature The temperature in degrees Celsius
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18s20_read_temperature(gpio_num_t pin, ds18x20_addr_t addr, int16_t *temperature);

/**
 * @brief Read the value from the last CONVERT_T operation for multiple devices.
 *
 * This should be called after ds18x20_measure() to fetch the result of the
 * temperature measurement.
 *
 * @param pin         The GPIO pin connected to the ds18x20 bus
 * @param addr_list   A list of addresses for devices to read.
 * @param addr_count  The number of entries in `addr_list`.
 * @param result_list An array of int16_ts to hold the returned temperature
 *                     values. It should have at least `addr_count` entries.
 *
 * @returns `ESP_OK` if all temperatures were fetched successfully
 */
esp_err_t ds18x20_read_temp_multi(gpio_num_t pin, ds18x20_addr_t *addr_list, size_t addr_count, int16_t *result_list);

/** Perform a ds18x20_measure() followed by ds18s20_read_temperature()
 *
 *  @param pin         The GPIO pin connected to the ds18s20 device
 *  @param addr        The 64-bit address of the device to read. This can be set
 *                     to ::DS18X20_ANY to read any device on the bus (but note
 *                     that this will only work if there is exactly one device
 *                     connected, or they will corrupt each others' transmissions)
 *  @param temperature The temperature in degrees Celsius
 */
esp_err_t ds18s20_measure_and_read(gpio_num_t pin, ds18x20_addr_t addr, int16_t *temperature);

/** Perform a ds18x20_measure() followed by ds18b20_read_temperature()
 *
 *  @param pin         The GPIO pin connected to the ds18x20 device
 *  @param addr        The 64-bit address of the device to read. This can be set
 *                     to ::DS18X20_ANY to read any device on the bus (but note
 *                     that this will only work if there is exactly one device
 *                     connected, or they will corrupt each others' transmissions)
 *  @param temperature The temperature in degrees Celsius
 */
esp_err_t ds18b20_measure_and_read(gpio_num_t pin, ds18x20_addr_t addr, int16_t *temperature);

/** Perform a ds18x20_measure() followed by ds18x20_read_temperature()
 *
 *  @param pin         The GPIO pin connected to the ds18x20 device
 *  @param addr        The 64-bit address of the device to read. This can be set
 *                     to ::DS18X20_ANY to read any device on the bus (but note
 *                     that this will only work if there is exactly one device
 *                     connected, or they will corrupt each others' transmissions)
 *  @param temperature The temperature in degrees Celsius
 */
esp_err_t ds18x20_measure_and_read(gpio_num_t pin, ds18x20_addr_t addr, int16_t *temperature);

/**
 * @brief Perform a ds18x20_measure() followed by ds18x20_read_temp_multi()
 *
 * @param pin         The GPIO pin connected to the ds18x20 bus
 * @param addr_list   A list of addresses for devices to read.
 * @param addr_count  The number of entries in `addr_list`.
 * @param result_list An array of int16_ts to hold the returned temperature
 *                    values. It should have at least `addr_count` entries.
 *
 * @returns `ESP_OK` if all temperatures were fetched successfully
 */
esp_err_t ds18x20_measure_and_read_multi(gpio_num_t pin, ds18x20_addr_t *addr_list, size_t addr_count, int16_t *result_list);

/**
 * @brief Read the scratchpad data for a particular ds18x20 device.
 *
 * This is not generally necessary to do directly. It is done automatically
 * as part of ds18x20_read_temperature().
 *
 * @param pin     The GPIO pin connected to the ds18x20 device
 * @param addr    The 64-bit address of the device to read. This can be set
 *                to ::DS18X20_ANY to read any device on the bus (but note
 *                that this will only work if there is exactly one device
 *                connected, or they will corrupt each others' transmissions)
 * @param buffer  An 8-byte buffer to hold the read data.
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18x20_read_scratchpad(gpio_num_t pin, ds18x20_addr_t addr, uint8_t *buffer);

/**
 * @brief Write the scratchpad data for a particular ds18x20 device.
 *
 * @param pin     The GPIO pin connected to the ds18x20 device
 * @param addr    The 64-bit address of the device to write. This can be set
 *                to ::DS18X20_ANY to read any device on the bus (but note
 *                that this will only work if there is exactly one device
 *                connected, or they will corrupt each others' transmissions)
 * @param buffer  An 3-byte buffer to hold the data to write
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18x20_write_scratchpad(gpio_num_t pin, ds18x20_addr_t addr, uint8_t *buffer);

/**
 * @brief Issue the copy scratchpad command, copying current scratchpad to
 *        EEPROM.
 *
 * @param pin     The GPIO pin connected to the ds18x20 device
 * @param addr    The 64-bit address of the device to command. This can be set
 *                to ::DS18X20_ANY to read any device on the bus (but note
 *                that this will only work if there is exactly one device
 *                connected, or they will corrupt each others' transmissions)
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
esp_err_t ds18x20_copy_scratchpad(gpio_num_t pin, ds18x20_addr_t addr);

#endif /* _DS18X20_H */