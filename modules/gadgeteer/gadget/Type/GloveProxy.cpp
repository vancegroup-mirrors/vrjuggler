/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/GloveProxy.h>


namespace gadget
{

GloveProxy::GloveProxy(const std::string& deviceName, const int unitNum)
   : base_type(deviceName, unitNum)
   , mVisible(true)
{
   /* Do nothing. */ ;
}

GloveProxyPtr GloveProxy::create(const std::string& deviceName,
                                 const int unitNum)
{
   return GloveProxyPtr(new GloveProxy(deviceName, unitNum));
}

GloveProxy::~GloveProxy()
{
   /* Do nothing. */ ;
}

const vpr::Interval& GloveProxy::getTimeStamp() const
{
   // XXX: Broken for now, this is a case similar to the KeyboardMouse type in
   //      that it does not point to one data element like digital, analog,
   //      and position.
   return vpr::Interval::NullInterval;
}

std::string GloveProxy::getElementType()
{
   return "glove_proxy";
}

} // End of gadget namespace
