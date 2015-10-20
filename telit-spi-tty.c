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

#define TELIT_SPI_TTY_MINORS 1
static struct tty_driver *telit_spi_tty_driver;

/*********** BEGIN tty_operations ***********/

static int
telit_spi_tty_open(struct tty_struct * tty, struct file * filp)
{
    return 0;
}

static void
telit_spi_tty_close(struct tty_struct * tty, struct file * filp)
{
}

static int
telit_spi_tty_write_room(struct tty_struct *tty)
{
    return 0;
}

static int
telit_spi_tty_write(struct tty_struct * tty,
                    const unsigned char *buf,
                    int count)
{
    return 0;
}

static void
telit_spi_tty_wait_until_sent(struct tty_struct *tty, int timeout)
{
}

static struct tty_operations telit_spi_ops = {
    .open            = telit_spi_tty_open,
    .close           = telit_spi_tty_close,
    .write           = telit_spi_tty_write,
    .write_room      = telit_spi_tty_write_room,
    .wait_until_sent = telit_spi_tty_wait_until_sent
};

/*********** END   tty_operations ***********/

static int __init
telit_spi_init(void)
{
    int rv;

    printk(KERN_INFO "Loading telit_spi_tty...\n");

    telit_spi_tty_driver = tty_alloc_driver(TELIT_SPI_TTY_MINORS, 0);
    if (!telit_spi_tty_driver) {
        return -ENOMEM;
    }

    telit_spi_tty_driver->owner = THIS_MODULE;
    telit_spi_tty_driver->driver_name = "telit_spi_tty";
    telit_spi_tty_driver->name = "ttySPI";
    telit_spi_tty_driver->major = 0;
    telit_spi_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
    telit_spi_tty_driver->subtype = SERIAL_TYPE_NORMAL;
    telit_spi_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
    tty_set_operations(telit_spi_tty_driver, &telit_spi_ops);

    rv = tty_register_driver(telit_spi_tty_driver);
    if (rv) {
        printk(KERN_ERR "%s - tty_register_driver failed.\n", __func__);
        put_tty_driver(telit_spi_tty_driver);
        return rv;
    }

    tty_register_device(telit_spi_tty_driver, 0, NULL);

    return 0;
}

static void __exit
telit_spi_exit(void)
{
    printk(KERN_INFO "Unloading telit_spi_tty...\n");

    tty_unregister_device(telit_spi_tty_driver, 0);
    tty_unregister_driver(telit_spi_tty_driver);
    put_tty_driver(telit_spi_tty_driver);
}

module_init(telit_spi_init);
module_exit(telit_spi_exit);

