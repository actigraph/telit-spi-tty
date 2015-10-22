/*
 * SPI TTY driver for Telit HE910/UE910 cellular modems.
 *
 * Copyright (C) 2015 ActiGraph
 *	Judge Maygarden <judge.maygarden@actigraphcorp.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/tty.h>

MODULE_AUTHOR("Judge Maygarden");
MODULE_DESCRIPTION("TTY emulation over SPI with Telit xE910 cellular modems.");
MODULE_LICENSE("GPL");

#define DRVNAME     "telit_spi"
#define TTYNAME     "ttyTELIT"

struct tty_driver *telit_tty_drv;

#define TELIT_SPI_TTY_MINORS 1
static struct telit_spi_tty_driver telit_spi;

/*********** begin tty operations ***********/

static int
telit_spi_open(struct tty_struct * tty, struct file * filp)
{
    printk(KERN_INFO "%s(0x%p, 0x%p)\n", __func__, tty, filp);

    return 0;
}

static void
telit_spi_close(struct tty_struct * tty, struct file * filp)
{
    printk(KERN_INFO "%s(0x%p, 0x%p)\n", __func__, tty, filp);
}

static int
telit_spi_write_room(struct tty_struct *tty)
{
    printk(KERN_INFO "%s(0x%p)\n", __func__, tty);

    return 0;
}

static int
telit_spi_write(struct tty_struct * tty,
                    const unsigned char *buf,
                    int count)
{
    printk(KERN_INFO "%s(0x%p, 0x%p, %d)\n", __func__, tty, buf, count);

    return 0;
}

static void
telit_spi_wait_until_sent(struct tty_struct *tty, int timeout)
{
    printk(KERN_INFO "%s(0x%p, %d)\n", __func__, tty, timeout);
}

static struct tty_operations telit_spi_ops = {
    .open            = telit_spi_open,
    .close           = telit_spi_close,
    .write           = telit_spi_write,
    .hangup          = telit_spi_hangup,
    .write_room      = telit_spi_write_room,
    .wait_until_sent = telit_spi_wait_until_sent
};

/*********** end tty operations ***********/

/*********** being spi operations ***********/

struct spi_driver telit_spi_drv = {
    .driver = {
        .name = DRVNAME,
        .pm = &telit_spi_pm,
        .owner = THIS_MODULE
    },
    .probe = telit_spi_probe,
    .shutdown = telit_spi_shutdown,
    .remove = telit_spi_remove,
    .id_table = telit_spi_id_table
};

/*********** end spi operations ***********/

static int __init
telit_spi_init(void)
{
    int rv;

    printk(KERN_INFO "Loading telit_spi_tty...\n");

    telit_tty_driver = tty_alloc_driver(TELIT_SPI_TTY_MINORS, 0);
    if (!telit_tty_driver) {
        return -ENOMEM;
    }

    telit_tty_driver->owner = THIS_MODULE;
    telit_tty_driver->driver_name = DRVNAME;
    telit_tty_driver->name = TTYNAME;
    telit_tty_driver->major = 0;
    telit_tty_driver->minor_start = 0;
    telit_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
    telit_tty_driver->subtype = SERIAL_TYPE_NORMAL;
    telit_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
    telit_tty_driver->init_termios = tty_std_termios;

    tty_set_operations(telit_tty_driver, &telit_spi_ops);

    rv = tty_register_driver(telit_tty_driver);
    if (rv) {
        printk(KERN_ERR "%s: tty_register_driver failed(%d)\n",
                DRVNAME, rv);
        goto err_free_tty;
    }

    rv = spi_register_driver(&telit_spi_driver);
    if (rv) {
        printk(KERN_ERR "%s: spi_register_driver failed(%d)\n",
                DRVNAME, rv);
        goto err_unreg_spi;
    }

    return 0;

err_unreg_spi:
    spi_unregister_driver(&telit_spi_driver);
err_free_tty:
    put_tty_driver(telit_tty_driver);

    return rv;
}
module_init(telit_spi_init);

static void __exit
telit_spi_exit(void)
{
    printk(KERN_INFO "Unloading telit_spi_tty...\n");

    tty_unregister_driver(telit_tty_driver);
    put_tty_driver(telit_tty_driver);
    spi_unregister_driver(&telit_spi_driver);
}
module_exit(telit_spi_exit);

