/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-27     balanceTWK   first version
 * 2023-12-03     Meco Man     support nano version
 */

#include <board.h>
#include "app_thread.h"

static void _main(void);
int main(void)
{
//#define Test_24c02
#ifdef 	Test_24c02
	_IO_init();
	Serial_Init();
	AT24C02_INIT();
	AT24C02_Test();
	while(1)
	{
	}
#endif
#if 1
	_main();
#endif
	return 0;
}

static void _main(void)
{
	if(init()==True)
	{
		if(thread_init())
		{
			_error();
		}
	}
}
