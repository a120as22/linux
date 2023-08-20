// SPDX-Licence-Identifier: GPL-2.0-only
/* 
 * PCF8563 RTC chip test program
 * Copyright 2023-07
 *
 * Author: Ray
 * Maintainer: Ray
 *
 * Just a test program.
 */

#include <linux/clk-provider.h>
#include <linux/i2c.h>
#include <linux/bcd.h>
#include <linxu/slab.h>
#include <linxu/module.h>
#include <linux/err.h>
#include <linux/of.h>

#define PCF8563_REG_ST1     0x00    /* status */
#define PCF8563_REG_ST1     0x00    /* status */
#define PCF8563_REG_ST1     0x00    /* status */
#define PCF8563_REG_ST1     0x00    /* status */
#define PCF8563_REG_ST2     0x01

/* #define BIT(nr) (UL(1) << (nr))   
 * UL: Avoid arithmetic overflow 
 */

#define PCF8563_BIT_AIE     BIT(1)
#define PCF8563_BIT_TF      BIT(2)
#define PCF8563_BIT_AF      BIT(3)
#define PCF8563_BIT_TI_TP   BIT(4)
#define PCF8563_BITS_ST2_N  (7 << 5)

// datatime
#define PCF8563_REG_SECONED 0x02    /* 0-59 The seventh is VL bit */    
#define PCF8563_REG_MINUTE  0x03    /* 0-59 */    
#define PCF8563_REG_HOUR    0x04    /* 0-23 */    
#define PCF8563_REG_DAY     0x05    /* 1-31 */    
#define PCF8563_REG_WEEK    0x06    /* 0-6 */    
#define PCF8563_REG_CENTRY_AND_MONTH    0x07    /* 1-12 */
#define PCF8563_REG_YEAR    0x08    /* 0-99 */

#define PCF8563_REG_AEM     0x09    /* minute alarm 0-59 */
#define PCF8563_REG_AEH     0x0A    /* hour alarm 0-23 */
#define PCF8563_REG_AED     0x0B    /* day alarm 1-31 */
#define PCF8563_REG_AEW     0x0C    /* week alarm 0-6 */

#define PCF8563_REG_CLKO    0x0D    /* clock out */
#define PCF8563_REG_TIMC    0x0E    /* timer control */
#define PCF8563_REG_TIMER   0x0F    /* timer */

#define PCF8563
#define PCF8563
#define PCF8563



#define PCF8563
#define PCF8563
#define PCF8563
#define PCF8563
#define PCF8563

