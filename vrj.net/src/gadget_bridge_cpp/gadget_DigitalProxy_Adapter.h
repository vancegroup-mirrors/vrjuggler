/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Generated from $Revision$ of $RCSfile$


#ifndef __GADGET_DIGITALPROXY_ADAPTER__
#define __GADGET_DIGITALPROXY_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/DigitalProxy.h>

class SHARPPY_API gadget_DigitalProxy_Adapter : public gadget::DigitalProxy
{
public:
   gadget_DigitalProxy_Adapter(const gadget::DigitalProxy& p0)
      : gadget::DigitalProxy(p0)
   {;}

   gadget_DigitalProxy_Adapter()
      : gadget::DigitalProxy()
   {;}

   virtual ~gadget_DigitalProxy_Adapter()
   {;}

   typedef bool (*refresh_callback_t)();
   refresh_callback_t refresh_callback;

   /// Override for virtual function gadget::TypedProxy<gadget::Digital>::refresh.
   virtual bool refresh()
   {
      return refresh_callback();
   }

   typedef std::basic_string<char,std::char_traits<char>,std::allocator<char> >* (*getDeviceName_callback_t)();
   getDeviceName_callback_t getDeviceName_callback;

   /// Override for virtual function gadget::TypedProxy<gadget::Digital>::getDeviceName.
   virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName()
   {
      return *(getDeviceName_callback());
   }

   typedef void (*updateData_callback_t)();
   updateData_callback_t updateData_callback;

   /// Override for virtual function gadget::DigitalProxy::updateData.
   virtual void updateData()
   {
      updateData_callback();
   }

   typedef bool (*config_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   config_callback_boost_shared_ptr_jccl__ConfigElement_t config_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function gadget::DigitalProxy::config.
   virtual bool config(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return config_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef gadget::Input* (*getProxiedInputDevice_callback_t)();
   getProxiedInputDevice_callback_t getProxiedInputDevice_callback;

   /// Override for virtual function gadget::DigitalProxy::getProxiedInputDevice.
   virtual gadget::Input* getProxiedInputDevice()
   {
      return getProxiedInputDevice_callback();
   }

}; // class gadget_DigitalProxy_Adapter




#endif
