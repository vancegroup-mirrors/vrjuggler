// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/Type/EventWindow/MouseEvent.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_MouseEvent_Wrapper: gadget::MouseEvent
{
    gadget_MouseEvent_Wrapper(PyObject* self_, const gadget::MouseEvent& p0):
        gadget::MouseEvent(p0), self(self_) {}

    gadget_MouseEvent_Wrapper(PyObject* self_, const gadget::EventType& p0, const gadget::Keys& p1, const int& p2, const int& p3, const int& p4, const int& p5, const int& p6, const long unsigned int& p7):
        gadget::MouseEvent(p0, p1, p2, p3, p4, p5, p6, p7), self(self_) {}

    gadget_MouseEvent_Wrapper(PyObject* self_):
        gadget::MouseEvent(), self(self_) {}

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::MouseEvent::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::MouseEvent::readObject(p0);
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_MouseEvent()
{
    scope* gadget_MouseEvent_scope = new scope(
    class_< gadget::MouseEvent, bases< gadget::Event >, pyj::gadget_MouseEvent_Wrapper >("MouseEvent", init<  >())
        .def(init< const gadget::MouseEvent& >())
        .def(init< const gadget::EventType&, const gadget::Keys&, const int&, const int&, const int&, const int&, const int&, const long unsigned int& >())
        .def("writeObject", (vpr::ReturnStatus (gadget::MouseEvent::*)(vpr::ObjectWriter*) )&gadget::MouseEvent::writeObject, (vpr::ReturnStatus (pyj::gadget_MouseEvent_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_MouseEvent_Wrapper::default_writeObject)
        .def("readObject", (vpr::ReturnStatus (gadget::MouseEvent::*)(vpr::ObjectReader*) )&gadget::MouseEvent::readObject, (vpr::ReturnStatus (pyj::gadget_MouseEvent_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_MouseEvent_Wrapper::default_readObject)
        .def("getButton", &gadget::MouseEvent::getButton, return_value_policy< copy_const_reference >())
        .def("getX", &gadget::MouseEvent::getX, return_value_policy< copy_const_reference >())
        .def("getY", &gadget::MouseEvent::getY, return_value_policy< copy_const_reference >())
        .def("getGlobalX", &gadget::MouseEvent::getGlobalX, return_value_policy< copy_const_reference >())
        .def("getGlobalY", &gadget::MouseEvent::getGlobalY, return_value_policy< copy_const_reference >())
        .def("getState", &gadget::MouseEvent::getState, return_value_policy< copy_const_reference >())
        .def("type", &gadget::Event::type, return_value_policy< copy_const_reference >())
        .def("setType", &gadget::Event::setType)
        .def("time", &gadget::Event::time, return_value_policy< copy_const_reference >())
    );
    register_ptr_to_python< boost::shared_ptr< gadget::MouseEvent > >();
    delete gadget_MouseEvent_scope;

}