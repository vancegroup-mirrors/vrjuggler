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

#ifndef _VPR_BASEIOSTATS_H
#define _VPR_BASEIOSTATS_H

#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/Util/Interval.h>

namespace vpr
{

class BlockIO;    // forward declare

/**
 * Base interface for IO Stat collection.
 *
 * This is the interface that the BlockIO class expects to deal with.
 * We add on other stats features using mixins that are added by a template
 * composition adapter.
 */
class VPR_CLASS_API BaseIOStatsStrategy
{
public:
   BaseIOStatsStrategy()
   {
       mRealObject = NULL;
   }

   virtual ~BaseIOStatsStrategy()
   {;}

   void setRealObject(vpr::BlockIO* real);

   enum { IS_NULL = 0 };

public:
   virtual void read_s(vpr::ReturnStatus& status,
                       void* buffer, const vpr::Uint32 length,
                       vpr::Uint32& bytes_read,
                       const vpr::Interval timeout = vpr::Interval::NoTimeout);

   virtual void readn_s(vpr::ReturnStatus& status,
                        void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytes_read,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout);

   virtual void write_s(vpr::ReturnStatus& status,
                        const void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytes_written,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout);

protected:
    vpr::BlockIO*   mRealObject;
};

/**
 * NULL strategy.
 *
 * @note Must derive from BaseIOStatsStrategy because the compiler still
 *       expects that interface.
 */
class NullIOStatsStrategy : public BaseIOStatsStrategy
{
public:
   enum { IS_NULL = 1 };
};

} // namespace vpr

#endif //_VPR_BASEIOSTATS_H