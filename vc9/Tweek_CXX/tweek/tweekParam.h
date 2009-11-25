/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _TWEEK_PARAM_H_
#define _TWEEK_PARAM_H_

/**
 * The 9-digit Tweek version integer. This form provides three digits for each
 * of the major, minor, and patch numbers with no leading zeros. The number
 * is a compile-time constant and is suitable (and intended) for use with
 * preprocessor integer comparisons.
 */
#define __TWEEK_version 1003005

/**
 * The Tweek version \em string. Instead of referencing this symbol, use
 * tweek::TWEEK_VERSION or tweek::getVersionString().
 */
#define __TWEEK_VERSION_STRING__ "v1.3.5-0 'Two-Face' (NSPR) trunk Nov 23, 2009 16:44:59"

/**
 * The individual TWEEK version numbers
 */
#define __TWEEK_MAJOR__ 1
#define __TWEEK_MINOR__ 3
#define __TWEEK_PATCH__ 5

#endif	/* _TWEEK_PARAM_H_ */
