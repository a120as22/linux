// SPDX-License-Identifier: GPL-2.0-only
/*
 * An I2C driver for the BH-1750
 * Copyright 2023-07 Tower Technologies
 *
 * Author: Ray
 * Maintainers: Ray
 *
 * based on the other drivers in this same firectory
 */

#include <linux/i2c.h>
#include <linux/bcd.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/err.h>

#define BH_1750_DEV_ADDRL	0x23
#define BH_1750_DEV_ADDRH	0x5C


#define BH_1750_PWR_OFF     0x00    /* power off */
#define BH_1750_PWR_ON      0x01    /* power on */
#define BH_1750_PWR_RESET   0x07    /* reset */

#define BH_1750_MODE_HIGH   0x10    /* 1lx 120ms */
#define BH_1750_MODE_HIGH2  0x11    /* 0.5lx 120ms */
#define BH_1750_MODE_LOW    0x13    /* 4xl 16ms */

#define BH_1750_ONE_HIGH    0x20    /* one */
#define BH_1750_ONE_HIGH2   0x21    /* one */
#define BH_1750_ONE_LOW     0x23    /* one */

// High bit measure time
#define BH_1750_MES_TIME_H  0x18    /* 01000_MT[7,6,5] */


// Low bit measure time 
#define BH_1750_MES_TIME_H  0x18    /* 011_MT[4,3,2,1,0] */

static struct i2c_driver bh_1750_driver;

static int bh_1750_read_block_data(struct i2c_client *client, unsigned char reg,
				unsigned char length, unsigned char *buf)
{
	struct i2c_msg msgs[] = {
		{/* setup read ptr, setup the direction*/
			.addr = client->addr,
			.len = 1,
			.buf = &reg,
		},
		{/* set the address and  data */
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = length,
			.buf = buf
		},
	};

    if ((i2c_transfer(client->adapter, msgs, 2)) != 2) {
		dev_err(&client->dev, "%s: read error\n", __func__);
		return -EIO;
	}

	return 0;
}

static int bh_1750_write_block_data(struct i2c_client *client, unsigned char reg,
				unsigned char length, unsigned char *buf)/* reg: register */
{
	int i, err;
	
	for (i = 0; i < length; i++) {
		unsigned char data[2] = { reg + i, buf[i] };

		err = i2c_master_send(client, data, sizeof(data));
		if ( err != sizeof(data)) {
			dev_err(&client->dev,
				"%s: error=%d addr=%02x, data=%02x\n",
				__func__, err, data[0], data[1]);
			return -EIO;
		}
	}
	return 0;
}

static int bh1750_drv_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
{
	int err;
	unsigned char buf[] = {
		BH_1750_PWR_ON,
		BH_1750_MODE_HIGH2
	};

	printk("probe\n");
	
	dev_dbg(&client->dev, "%s\n", __func__);

	err = bh_1750_write_block_data(client, BH_1750_DEV_ADDRL, 2, buf);
	if (err < 0) {
		dev_err(&client->dev, "%s: probe write error\n", __func__);
		return err;
	}

	return 0;
}

static int bh1750_drv_remove(struct i2c_client *client)
{
	int err=0;
	printk("remove\n");

	return err;
}

static const struct i2c_device_id bh1750_dev_id[] = {
	{ "test,bh1750", 0 },  /* 设备名字 */
	{}
};

static const struct of_device_id bh1750_of_match[] = {
	{ .compatible = "test,bh1750", },
	{}
};

static struct i2c_driver bh1750_drv = {
	.driver = {
		.name	= "bh1750_drv",
        .owner = THIS_MODULE,
		.of_match_table = bh1750_of_match,
	},
	.probe		= bh1750_drv_probe, /* 当匹配到i2c设备时调用 */
	.remove		= bh1750_drv_remove, /* 当卸载i2c设备或驱动时调用 */
	.id_table	= bh1750_dev_id, 	/* 这个结构体中的名字很重要 */
};


static struct i2c_board_info hb1750_i2c_device = {
 	I2C_BOARD_INFO("bh1750", 0x23),
};



static int __init bh1750_drv_init(void)
{

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	i2c_add_driver(&bh1750_drv);
	printk("bh1750_drv_init\n");

	return 0;
}
 
static void __exit bh1750_drv_exit(void)
{
 
	i2c_del_driver(&bh1750_drv);
	printk("bh1750_drv_exit\n");
	return;
}

module_init(bh1750_drv_init);
module_exit(bh1750_drv_exit);

// module_i2c_driver(bh1750_drv);

MODULE_DESCRIPTION("Philips BH1750 driver");
MODULE_LICENSE("GPL");
