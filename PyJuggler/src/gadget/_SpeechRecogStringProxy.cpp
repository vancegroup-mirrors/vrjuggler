// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/SpeechRecogStringProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_SpeechRecogStringProxy_Wrapper: gadget::SpeechRecogStringProxy
{
    gadget_SpeechRecogStringProxy_Wrapper(PyObject* self_, const gadget::SpeechRecogStringProxy& p0):
        gadget::SpeechRecogStringProxy(p0), self(self_) {}

    gadget_SpeechRecogStringProxy_Wrapper(PyObject* self_):
        gadget::SpeechRecogStringProxy(), self(self_) {}

    void updateData() {
        try
        {
            call_method< void >(self, "updateData");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_updateData() {
        gadget::SpeechRecogStringProxy::updateData();
    }

    vpr::Interval getTimeStamp() const {
        try
        {
           return call_method< vpr::Interval >(self, "getTimeStamp");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::Interval();
    }

    vpr::Interval default_getTimeStamp() const {
        return gadget::SpeechRecogStringProxy::getTimeStamp();
    }

    bool config(jccl::ConfigElementPtr p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::SpeechRecogStringProxy::config(p0);
    }

    void set(std::string p0, gadget::SpeechRecogString* p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::string p0, gadget::SpeechRecogString* p1) {
        gadget::SpeechRecogStringProxy::set(p0, p1);
    }

    bool refresh() {
        try
        {
            return call_method< bool >(self, "refresh");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_refresh() {
        return gadget::SpeechRecogStringProxy::refresh();
    }

    std::string getDeviceName() const {
        try
        {
            return call_method< std::string >(self, "getDeviceName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::string default_getDeviceName() const {
        return gadget::SpeechRecogStringProxy::getDeviceName();
    }

    bool isStupified() const {
        try
        {
            return call_method< bool >(self, "isStupified");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_isStupified() const {
        return gadget::SpeechRecogStringProxy::isStupified();
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_SpeechRecogStringProxy()
{
    class_< gadget::SpeechRecogStringProxy, pyj::gadget_SpeechRecogStringProxy_Wrapper >("SpeechRecogStringProxy",
         "A proxy class to string speech recognition devices used by the\n"
         "Input Manager.\n\n"
         "A string speech recognition proxy always points to a string\n"
         "speech recognition device and a unit number within that device.\n"
         "The Input Manager can therefore keep an array of these around and\n"
         "treat them as string speech recognition devices that only return\n"
         "a single sub-device's amount of data (one int)."
         ,
         init<  >()
        )
        .def(init< const gadget::SpeechRecogStringProxy& >())
        .def("updateData",
             (void (gadget::SpeechRecogStringProxy::*)() )&gadget::SpeechRecogStringProxy::updateData,
             (void (pyj::gadget_SpeechRecogStringProxy_Wrapper::*)())&pyj::gadget_SpeechRecogStringProxy_Wrapper::default_updateData,
             "updateData()\n"
             "Updates the cached data copy from the device."
         )
        .def("getTimeStamp",
             (vpr::Interval (gadget::SpeechRecogStringProxy::*)() const)&gadget::SpeechRecogStringProxy::getTimeStamp,
             (vpr::Interval (pyj::gadget_SpeechRecogStringProxy_Wrapper::*)() const)&pyj::gadget_SpeechRecogStringProxy_Wrapper::default_getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("config",
             (bool (gadget::SpeechRecogStringProxy::*)(jccl::ConfigElementPtr) )&gadget::SpeechRecogStringProxy::config,
             (bool (pyj::gadget_SpeechRecogStringProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_SpeechRecogStringProxy_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this proxy using the given jccl.ConfigElement\n"
             "instance."
         )
        .def("set",
             (void (gadget::TypedProxy<gadget::SpeechRecogString>::*)(std::string,gadget::SpeechRecogString*) )&gadget::TypedProxy<gadget::SpeechRecogString>::set,
             (void (pyj::gadget_SpeechRecogStringProxy_Wrapper::*)(std::string, gadget::SpeechRecogString*))&pyj::gadget_SpeechRecogStringProxy_Wrapper::default_set,
             "set(devName, devPtr)\n"
             "Sets the proxy to point to the given type-specific device.\n"
             "Pre-condition:\n"
             "devPtr must be a valid device of type gadget.SpeechRecogString\n"
             "Post-condition:\n"
             "The proxy now references the given device.  The device name we\n"
             "are proxying is set to devPtr.getInstanceName().\n"
             "Arguments:\n"
             "devName -- The name of the device at which we are pointing.\n"
             "devPtr  -- Pointer to the device.  For gadget.SpeechRecogString,\n"
             "           this points to an instance of gadget.SpeechRecogString."
         )
        .def("refresh",
             (bool (gadget::TypedProxy<gadget::SpeechRecogString>::*)() )&gadget::TypedProxy<gadget::SpeechRecogString>::refresh,
             (bool (pyj::gadget_SpeechRecogStringProxy_Wrapper::*)())&pyj::gadget_SpeechRecogStringProxy_Wrapper::default_refresh,
             "refresh() -> Boolean\n"
             "Refreshes the proxy.  This attempts to lookup the device that\n"
             "we are proxying.  If the lookup fails, then we become\n"
             "stupified.  If not, then the proxy is pointed at this\n"
             "potentially new device."
         )
        .def("getDeviceName",
             (std::string (gadget::TypedProxy<gadget::SpeechRecogString>::*)() const)&gadget::TypedProxy<gadget::SpeechRecogString>::getDeviceName,
             (std::string (pyj::gadget_SpeechRecogStringProxy_Wrapper::*)() const)&pyj::gadget_SpeechRecogStringProxy_Wrapper::default_getDeviceName,
             "getDeviceName() -> string object\n"
             "Gets the name of the device that we are proxying."
         )
        .def("isStupified",
             (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified,
             (bool (pyj::gadget_SpeechRecogStringProxy_Wrapper::*)() const)&pyj::gadget_SpeechRecogStringProxy_Wrapper::default_isStupified,
             "isStupified() -> Boolean\n"
             "Is the proxy currently stupified?\n"
             "If the device we are proxying does not exist, then this will\n"
             "return True."
         )
        .def("getData", &gadget::SpeechRecogStringProxy::getData,
             "getData() -> float\n"
             "Gets the current string speech recognition data value."
         )
        .def("getStringData", &gadget::SpeechRecogStringProxy::getStringData,
             return_internal_reference< 1 >()
         )
        .def("getSpeechRecogStringPtr",
             &gadget::SpeechRecogStringProxy::getSpeechRecogStringPtr,
             return_internal_reference< 1 >(),
             "getSpeechRecogStringPtr() -> gadget.SpeechRecogString object\n"
         )
        .def("getUnit", &gadget::SpeechRecogStringProxy::getUnit,
             "getUnit() -> int\n"
             "Returns the unit index into the analog device from which this\n"
             "proxy is reading data."
         )
        .def("getElementType",
             &gadget::SpeechRecogStringProxy::getElementType,
             "getElementType() -> string object"
         )
        .def("getName", &gadget::Proxy::getName,
             "getName() -> string object\n"
             "Gets the name of the proxy."
         )
        .def("setName", &gadget::Proxy::setName,
             "setName(name)\n"
             "Sets the name of the proxy.\n"
             "Arguments:\n"
             "name -- The name for this proxy as a string object."
         )
        .def("stupify", &gadget::Proxy::stupify,
             "stupify(newState = True)\n"
             "Sets the stupification state of this proxy.\n"
             "Keyword arguments:\n"
             "newState -- The new state of stupification."
         )
        .staticmethod("getElementType")
    ;

}
