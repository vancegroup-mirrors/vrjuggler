// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/Position.h>
#include <gadget_Position_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_Position_Adapter* gadget_Position_Position__0(gadget_Position_Adapter::config_callback_jccl_ConfigElementPtr_t cb0, gadget_Position_Adapter::getBaseType_callback_t cb1, gadget_Position_Adapter::writeObject_callback_vpr_ObjectWriter_t cb2, gadget_Position_Adapter::readObject_callback_vpr_ObjectReader_t cb3)
   {
      gadget_Position_Adapter* obj = new gadget_Position_Adapter();
      obj->config_callback_jccl_ConfigElementPtr = cb0;
      obj->getBaseType_callback = cb1;
      obj->writeObject_callback_vpr_ObjectWriter = cb2;
      obj->readObject_callback_vpr_ObjectReader = cb3;
      return obj;
   }

   SHARPPY_API void delete_gadget_Position(gadget_Position_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::Position::getPositionData()
   SHARPPY_API gadget::PositionData* gadget_Position_getPositionData__int0(gadget_Position_Adapter* self_)
   {
      gadget::PositionData* result;
      result = new gadget::PositionData(self_->getPositionData());
      return result;
   }

   // Wrapper for non-virtual method gadget::Position::getPositionData()
   SHARPPY_API gadget::PositionData* gadget_Position_getPositionData__int1(gadget_Position_Adapter* self_, int p0)
   {
      gadget::PositionData* result;
      result = new gadget::PositionData(self_->getPositionData(p0));
      return result;
   }


   // Wrapper for non-virtual method gadget::Position::swapPositionBuffers()
   SHARPPY_API void gadget_Position_swapPositionBuffers__0(gadget_Position_Adapter* self_)
   {
      self_->swapPositionBuffers();
   }


   // Wrapper for virtual method gadget::Position::config()
   SHARPPY_API bool gadget_Position_config__jccl_ConfigElementPtr1(gadget_Position_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->gadget::Position::config(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::Position::getBaseType()
   SHARPPY_API char* gadget_Position_getBaseType__0(gadget_Position_Adapter* self_)
   {
      std::string temp_result;
      char* result;
      temp_result = self_->gadget::Position::getBaseType();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for virtual method gadget::Position::writeObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Position_writeObject__vpr_ObjectWriter1(gadget_Position_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Position::writeObject(p0));
      return result;
   }


   // Wrapper for virtual method gadget::Position::readObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Position_readObject__vpr_ObjectReader1(gadget_Position_Adapter* self_, vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Position::readObject(p0));
      return result;
   }


} // extern "C" for gadget::Position

