/*
 *  arch/arm/mach-sun7i/include/mach/hardware.h
 *
 *  This file contains the hardware definitions of the RealView boards.
 *
 *  Copyright (C) 2012-2016 Allwinner Limited.
 *  Benn Huang (benn@allwinnertech.com)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __SW_HARDWARE_H
#define __SW_HARDWARE_H

// Linux
//#define IO_ADDRESS(x)		((x) + 0xf0000000)
//#define __io_address(n)		__io(IO_ADDRESS(n))
#define IO_ADDRESS(x)		(x)

#endif
