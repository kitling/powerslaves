#pragma once

#ifndef POWERSLAVES_API
#define POWERSLAVES_API

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <wchar.h>

/*! \brief Powersaves command type.
 * Magic numbers that need to be sent to the Powersaves MCU.
 */
enum powerslaves_cmdtype {
    TEST = 0x02,
    // 0x08 and 0x09 appear to disconnect USB and infloop
    SWITCH_MODE = 0x10,
    ROM_MODE = 0x11,
    SPI_MODE = 0x12,
    NTR = 0x13,
    CTR = 0x14,
    SPI = 0x15,
    /* UNK1 = 0x16, // "weird reset for SPI and locks up powerslaves" */
    /* UNK2 = 0x20, // does something. *shrugs* */
    /* UNK3 = 0x30, // does something. *shrugs* */
};

// if first 4 bytes are [0x99, 0x44, 0x46, 0x55], it seems to perform a firmware update

/*! \brief Optional function that initializes a particular powersaves.
 * This function is called automatically by any function that needs a powersaves,
 * so it's only necessary to use this if you have multiple powersaves.
 *
 *  \param serial Serial number to specify an exact powersaves device. If NULL, automatic selection is preformed.
 *
 *  \return 0 on success, -1 on failure.
 */
int powerslaves_select(const wchar_t *serial);

/*! \brief Sends a cartridge command.
 *
 *  \param type Type of the command to be sent.
 *  \param cmdbuf Pointer to cartridge command.
 *  \param response_len Length of the response expected to be received.
 *
 *  \return Number of bytes sent on success, -1 on communication error, -2 on invalid paramater.
 */
int powerslaves_send(enum powerslaves_cmdtype type, const uint8_t *cmdbuf, uint16_t response_len);

/*! \brief Sends a cartridge command of a specific length.
 *
 *  You should only need this for SPI commands. In all other cases, use the wrapper,
 *  powerslaves_send().
 *
 *  \param type Type of the command to be sent.
 *  \param cmdlen Length of cartridge command.
 *  \param cmdbuf Pointer to cartridge command.
 *  \param response_len Length of the response expected to be received.
 */
int powerslaves_sendlen(enum powerslaves_cmdtype type, uint16_t cmdlen, const uint8_t *cmdbuf, uint16_t response_len);

/*! \brief Receives a response to a cartridge command.
 *
 *  \param buf Pointer to buffer that will receive the response.
 *  \param len Number of bytes to read into the buffer.
 *
 *  \return Number of bytes received on success, -1 on communication error.
 */
int powerslaves_receive(uint8_t *buf, uint16_t len);

/*! \brief Sends a cartridge command and receives a response.
 *
 *  A small wrapper around powerslaves_send() and powerslaves_receive()
 *  for the common case.
 *
 *  \param type Type of the command to be sent.
 *  \param cmdbuf Pointer to cartridge command.
 *  \param response_len Length of the response expected to be received.
 *  \param resp Pointer to buffer that will receive the response.
 *
 *  \return Number of bytes received on success, -1 on communication error, -2 on invalid parameter.
 */
int powerslaves_sendreceive(enum powerslaves_cmdtype type, const uint8_t *cmdbuf, uint16_t response_len, uint8_t *resp);

/*! \brief Resets the Powersaves device and switches mode.
 *
 *  \param mode Mode to switch to. This can be either ROM_MODE or SPI_MODE.
 *
 *  \return 0 on success, -1 on communication failure, -2 on invalid parameter.
 */
int powerslaves_mode(enum powerslaves_cmdtype mode);

/*! \brief Deinitializes Powerslaves. */
void powerslaves_exit();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: POWERSLAVES_API */
