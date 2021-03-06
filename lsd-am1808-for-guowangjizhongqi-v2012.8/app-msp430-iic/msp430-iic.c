/**
 * @file i2c_rx8025t_test.c
 * @brief test for rx8025t on /dev/i2c-0
 *
 * Copyright (C) 2006-2011, toby
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author toby.zhang <toby.chs@gmail.com>
 * @date 2011-7-24 21:30:07
 * @version 0.01
 *
 * This file is pary of driver for RX8025T.
 */

/**
 * @note
 *  \n
 *  
 * \b Change \b Logs: \n
 * 2011.06.01   创建初始 by toby \n
 * 
 */

/**
 * @addtogroup api_i2c_rx8025t_test
 * @{ 
 */

//#include "i2c_rx8025t_test.h"

// TODO: Add your code here

#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

/*
 * MAIN FUNCTION
 */
int main(int argc, char **argv)
{
	int fd;
	int ret;
	unsigned long u32_i;
	struct i2c_rdwr_ioctl_data rx8025_ram_data;
	unsigned int ad_value;

	fd = open("/dev/i2c-1", O_RDWR);

	/*
	 * /dev/i2c-0是在注册i2c-dev.c后产生的，代表一个可操作的适配器。如果不使用i2c-dev.c
	 * 的方式，就没有，也不需要这个节点。
	 */
	if(fd<0)
	{
		perror("open error");
	}

	/*
	 * 因为操作时序中，最多是用到2个开始信号（字节读操作中），所以此将
	 * rx8025_ram_data.nmsgs配置为2
	 */
	rx8025_ram_data.nmsgs = 2;
	rx8025_ram_data.msgs = (struct i2c_msg*)malloc(rx8025_ram_data.nmsgs*sizeof(struct i2c_msg));
	if(!rx8025_ram_data.msgs)
	{
		perror("rx8025_ram_data.msgs malloc error");
		exit(1);
	}

	ioctl(fd, I2C_TIMEOUT, 50);/*超时时间*/
	ioctl(fd, I2C_RETRIES, 2);/*重复次数*/


	/*
	 * 读操作
	 */

	(rx8025_ram_data.msgs[0]).buf = (unsigned char*)malloc(2);
	(rx8025_ram_data.msgs[1]).buf = (unsigned char*)malloc(2); // 存放返回值的地址。
	for(u32_i = 0;u32_i < 2;u32_i++)
	{
		rx8025_ram_data.nmsgs=2; // 发送二个消息,即二次操作
		(rx8025_ram_data.msgs[0]).len = 1; // RX8025T目标数据的地址长度
		(rx8025_ram_data.msgs[0]).addr = 0x68; // RX8025T设备地址
		(rx8025_ram_data.msgs[0]).flags = 0; // 写操作, 地址
		(rx8025_ram_data.msgs[0]).buf[0] = u32_i+10; // RX8025T数据地址
		(rx8025_ram_data.msgs[1]).len = 2; // 读出的数据长度, 一个字节
		(rx8025_ram_data.msgs[1]).addr = 0x68;// RX8025T设备地址 
		(rx8025_ram_data.msgs[1]).flags = I2C_M_RD; // 读操作
		(rx8025_ram_data.msgs[1]).buf[0] = 0;//初始化读缓冲
		(rx8025_ram_data.msgs[1]).buf[1] = 0;//初始化读缓冲
		ret = ioctl(fd, I2C_RDWR, (unsigned long)&rx8025_ram_data);
		if(ret<0)
		{
			perror("I2C: READ ioctl error");
		}

		printf("buf[0] = %x\n", (rx8025_ram_data.msgs[1]).buf[0]);
		printf("buf[1] = %x\n", (rx8025_ram_data.msgs[1]).buf[1]);
		ad_value = (rx8025_ram_data.msgs[1]).buf[0];
		ad_value = ad_value << 8;
		ad_value = ad_value + (rx8025_ram_data.msgs[1]).buf[1];
		printf("AD channel %d, value = %d\n",u32_i,ad_value);
	}	
	printf("AD channel 0 is batter, AD channel 1 is 4-20mA\n");

	// free buff
	free((rx8025_ram_data.msgs[0]).buf);
	free((rx8025_ram_data.msgs[1]).buf);

	close(fd);

	return 0;
}

/**
 * Close the Doxygen group.
 * @}
 */

