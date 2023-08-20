#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/slab.h>

static struct i2c_adapter *g_adapter;

static void bh1750_emulate_xfer(struct i2c_adapter *i2c_adap, struct i2c_msg *msg)
{
	printk("bh1750_emulate_xfer");
}

static int i2c_bus_virtual_xfer(struct i2c_adapter *i2c_adap,
		struct i2c_msg msgs[], int num)
{
	int i;
	//emulate bh1750 addr=0x23
	for(i=0;i<num;i++) {
		// do transfer msgs[i]
		if(msgs[i].addr == 0x23) {
			bh1750_emulate_xfer(i2c_adap, &msgs[i]);
		} else {
			i = -EIO;
			break;
		}
	}
	return i;
}

static u32 i2c_bus_virtual_func(struct i2c_adapter *adap)
{
	return (I2C_FUNC_I2C | I2C_FUNC_NOSTART | I2C_FUNC_SMBUS_EMUL |
	       I2C_FUNC_SMBUS_READ_BLOCK_DATA |
	       I2C_FUNC_SMBUS_BLOCK_PROC_CALL |
	       I2C_FUNC_PROTOCOL_MANGLING);
}

const struct i2c_algorithm i2c_bus_virtual_algo = {
	.master_xfer = i2c_bus_virtual_xfer,
	.functionality = i2c_bus_virtual_func,
};

static int i2c_bus_virtual_probe(struct platform_device *pdev)
{
	/* get info from device tree, to set i2c_adapter or hardware  */

	/* alloc, set, register i2c_adapter */
	printk("i2c_bus_virtual_probe\n");
	g_adapter = kzalloc(sizeof(*g_adapter), GFP_KERNEL);

	g_adapter->owner = THIS_MODULE;
	g_adapter->class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
	g_adapter->nr = -1;
	snprintf(g_adapter->name, sizeof(g_adapter->name), "bh1750");
	g_adapter->algo = &i2c_bus_virtual_algo;

	i2c_add_adapter(g_adapter);		//i2c_add_numerd_adapter(g_adapter);

	return 0;
}

static int i2c_bus_virtual_remove(struct platform_device *dev)
{
	i2c_del_adapter(g_adapter);
	return 0;
}

static const struct of_device_id i2c_bus_virtual_ids[] = {
	{ .compatible = "test,bh1750", },
	{ /* end of list */ }
};

struct platform_driver i2c_bus_virtual_driver = {
	.driver = {
		.name = "bh1750",
		.of_match_table = i2c_bus_virtual_ids,
	},
	.probe = i2c_bus_virtual_probe,
	.remove = i2c_bus_virtual_remove,
};

static int i2c_bus_virtual_init(void)
{
	int ret;
	printk("i2c_bus_virtual_init\n");
	ret = platform_driver_register(&i2c_bus_virtual_driver);
	if(ret)
		printk(KERN_ERR"i2c_driver register failed\n");

	return 0;
}

static void i2c_bus_virtual_exit(void)
{
	platform_driver_unregister(&i2c_bus_virtual_driver);
}

module_init(i2c_bus_virtual_init);
module_exit(i2c_bus_virtual_exit);

MODULE_AUTHOR("tuo");
MODULE_DESCRIPTION("Philips BH1750 driver");
MODULE_LICENSE("GPL");
