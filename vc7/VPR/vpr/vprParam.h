/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _VPR_PARAM_H_
#define _VPR_PARAM_H_


/*
 * Define to the 9-digit VPR version integer.  This is the "internal version
 * number" that is intended for internal feature checks by VPR itself.
 */
#define __VPR_version 32002

/*
 * This is the "human-readable" VPR version _string_.  It is of the form
 * <major>.<minor>.<patch>-<build>.  The major version number is updated when
 * major architectural changes are made; the minor when there are minor API
 * changes; and the patch when there are bug fixes.  The build number is
 * representative of the nightly build process relative to a given version
 * x.y.z.
 */
namespace vpr
{
   const char* VPR_VERSION  = "v0.32.2-0 'Ring Wraith' (NSPR) HEAD Nov 15, 2003 12:06:59";
}


#endif	/* _VPR_PARAM_H_ */
