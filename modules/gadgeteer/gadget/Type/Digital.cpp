/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <gadget/Type/Digital.h>

namespace gadget
{

      vpr::ReturnStatus Digital::writeObject(vpr::ObjectWriter* writer)
      {
         writer->beginTag(Digital::getBaseType());
         //std::cout << "[Remote Input Manager] In Digital write" << std::endl;
         SampleBuffer_t::buffer_t& stable_buffer = mDigitalSamples.stableBuffer();
         writer->beginAttribute(gadget::tokens::DataTypeAttrib);
            writer->writeUint16(MSG_DATA_DIGITAL);                               // Write out the data type so that we can assert if reading in wrong place
         writer->endAttribute();

         writer->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
            writer->writeUint16(stable_buffer.size());                           // Write the # of vectors in the stable buffer
         writer->endAttribute();

         if ( !stable_buffer.empty() )
         {
            mDigitalSamples.lock();
            for ( unsigned j=0;j<stable_buffer.size();j++ )                               // For each vector in the stable buffer
            {
               writer->beginTag(gadget::tokens::BufferSampleTag);
               writer->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
                  writer->writeUint16(stable_buffer[j].size());                           // Write the # of DigitalDatas in the vector
               writer->endAttribute();
               for ( unsigned i=0;i<stable_buffer[j].size();i++ )                         // For each DigitalData in the vector
               {
                  writer->beginTag(gadget::tokens::DigitalValue);
                  writer->beginAttribute(gadget::tokens::TimeStamp);
                     writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
                  writer->endAttribute();
                  writer->writeUint32((vpr::Uint32)stable_buffer[j][i].getDigital());  // Write Digital Data(int)
                  writer->endTag();
               }
               writer->endTag();
            }
            mDigitalSamples.unlock();
         }
         writer->endTag();

         return vpr::ReturnStatus::Succeed;
      }

      vpr::ReturnStatus Digital::readObject(vpr::ObjectReader* reader)
      {
            //std::cout << "[Remote Input Manager] In Digital read" << std::endl;
         vprASSERT(reader->attribExists("rim.timestamp.delta"));
         vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

            // ASSERT if this data is really not Digital Data
         reader->beginTag(Digital::getBaseType());
         reader->beginAttribute(gadget::tokens::DataTypeAttrib);
            vpr::Uint16 temp = reader->readUint16();
         reader->endAttribute();

         vprASSERT(temp==MSG_DATA_DIGITAL && "[Remote Input Manager]Not Digital Data");

         std::vector<DigitalData> dataSample;

         unsigned numDigitalDatas;
         vpr::Uint32 value;
         vpr::Uint64 timeStamp;
         DigitalData temp_digital_data;

         reader->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
            unsigned numVectors = reader->readUint16();
         reader->endAttribute();

         //std::cout << "Stable Digital Buffer Size: "  << numVectors << std::endl;
         mDigitalSamples.lock();
         for ( unsigned i=0;i<numVectors;i++ )
         {
            reader->beginTag(gadget::tokens::BufferSampleTag);
            reader->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
               numDigitalDatas = reader->readUint16();
            reader->endAttribute();

            dataSample.clear();
            for ( unsigned j=0;j<numDigitalDatas;j++ )
            {
               reader->beginTag(gadget::tokens::DigitalValue);
               reader->beginAttribute(gadget::tokens::TimeStamp);
                  timeStamp = reader->readUint64();               // read Time Stamp vpr::Uint64
               reader->endAttribute();
               value = reader->readUint32();                   // read Digital Data(int)
               reader->endTag();

               temp_digital_data.setDigital(value);
               temp_digital_data.setTime(vpr::Interval(timeStamp + delta,vpr::Interval::Usec));
               dataSample.push_back(temp_digital_data);
            }
            mDigitalSamples.addSample(dataSample);
            reader->endTag();
         }
         mDigitalSamples.unlock();
         mDigitalSamples.swapBuffers();

         reader->endTag();

         return vpr::ReturnStatus::Succeed;
      }

} // End of gadget namespace


