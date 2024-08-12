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

#include "app_u.h"


int main(void)
{
	if(u_init())
	{
		rt_create_thread_init();
	}
}
