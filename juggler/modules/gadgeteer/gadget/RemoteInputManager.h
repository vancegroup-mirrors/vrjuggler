/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_REMOTE_INPUT_MANAGER_H
#define _GADGET_REMOTE_INPUT_MANAGER_H

#include <gadget/gadgetConfig.h>

#include <list>

#include <vpr/Sync/Mutex.h>
#include <vpr/Util/ReturnStatus.h>	
#include <vpr/Util/Assert.h>

#include <gadget/PacketHandler.h>

#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Config/ConfigElementPtr.h>

namespace cluster
{  
   class DeviceRequest;
   class Packet;
}

namespace gadget
{

class Node;
class DeviceServer;
class Input;
class VirtualDevice;

class GADGET_CLASS_API RemoteInputManager
   : public gadget::PacketHandler/*, public jccl::ConfigElementHandler*/
{
public:
   RemoteInputManager(const vpr::GUID& guid);

   virtual ~RemoteInputManager();

   /**
    * This function is called when the network losses
    * a connection to a Node. The RemoteInputManager
    * needs to handle this by removing all VirtualDevices on 
    * the lost node. And removing the node as a client from 
    * all DeviceServers.
    */
   void recoverFromLostNode(Node* lost_node);
   
   /**
    * Handle a incoming packet.
    */
   void handlePacket(cluster::Packet* packet, Node* node);

   /** 
    *  Returns the name of the this plugin
    */
   virtual std::string getHandlerName()
   {
      return(std::string("RemoteInputManager"));
   }
   
   vpr::GUID getHandlerGUID()
   {
      return mHandlerGUID;
   }
   
   /** Calls any action needed by this plugin before postFrame()
    *
    *  This function was inherited from the
    *  ClusterPlugin abstract class. 
    */
   void sendDataAndSync();


   // Methods used by InputManager.
   gadget::Input* getVirtualDevice(const vpr::GUID& device_id);
   gadget::Input* getVirtualDevice(const std::string& device_name);
   
   // Configuration methods.
   /*
   bool configAdd(jccl::ConfigElementPtr element);
   bool configRemove(jccl::ConfigElementPtr element);
   bool configCanHandle(jccl::ConfigElementPtr element);  
   */

   // Debug methods.
   void debugDumpDeviceServers(int debug_level);
   void debugDumpVirtualDevices(int debug_level);
private:
   // VirtualDevice methods.
   vpr::ReturnStatus addVirtualDevice(const vpr::GUID& device_id, const std::string& name, 
                                      const std::string& device_base_type, const std::string& hostname);
   void addVirtualDevice(VirtualDevice* device);
   void removeVirtualDevice(const std::string& device_name);
   void removeVirtualDevice(const vpr::GUID& device_id);
   vpr::ReturnStatus removeVirtualDevicesOnHost(const std::string& host_name);
   
   // DeviceServer methods.
   vpr::ReturnStatus addDeviceServer(const std::string& name, gadget::Input* device);
   void addDeviceServer(DeviceServer* device);
   void removeDeviceServer(const std::string& device_name);
   void removeDeviceServer(const vpr::Uint16& device_id);
   DeviceServer* getDeviceServer(const std::string& device_name);
   
   // Connection management.
   vpr::ReturnStatus removeDeviceClientsForHost(const std::string& host_name);
   
   // Configuration helpers.
   jccl::ConfigElementPtr getConfigElementPointer(std::string& name);
   vpr::ReturnStatus createPendingConfigRemove(std::string device_name);
   vpr::ReturnStatus createPendingConfigRemoveAndAdd(std::string device_name);

   // Device request management.
public:
   void addPendingDeviceRequest(cluster::DeviceRequest* new_device_req, Node* node);
   void sendDeviceRequests();
private:
   void removePendingDeviceRequest(std::string device_name);
   vpr::Uint16 getNumberPendingDeviceRequests();
   
protected:
   vpr::GUID                            mHandlerGUID;
   std::map<cluster::DeviceRequest*, 
            Node*>               mPendingDeviceRequests;     /**< UserData Request list. */
   vpr::Mutex                           mPendingDeviceRequestsLock; /**< Lock on UserData Request list.*/   
   
   std::map<vpr::GUID, VirtualDevice*>  mVirtualDevices;     /**< List of Virtual Devices on the local Node. */
   vpr::Mutex                           mVirtualDevicesLock; /**< Lock on Virtual Device list.*/

   std::vector<DeviceServer*>           mDeviceServers;      /**< List of Devices that should act as servers to remote Nodes.*/
   vpr::Mutex                           mDeviceServersLock;  /**< Lock on Device Server list.*/
};

} // end namespace gadget

#endif