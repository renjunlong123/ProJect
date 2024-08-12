#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

/**
 @brief Include
 */
#include "data.h"


/**
 @brief Function
 */
typedef void (*p_app)(void);
void read_ota_parm_info(void);
#endif
