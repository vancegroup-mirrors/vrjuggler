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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.editors;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Insets;
//import java.awt.Graphics;
import java.awt.Color;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.*;
import java.beans.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
//import com.sun.java.swing.plaf.windows.WindowsTreeUI;

public class PropertySheet extends PropertyComponent implements ConfigElementListener
{
   public void setConfigElement(ConfigElement elm)
   {
      mConfigElement = elm;
   }

   public void init()
   {
      setBackground(mColor);
           // Create a frame
      setBounds (100, 100, 700, 300);

      // Create a TableLayout for the frame
      double border = 10;
      double f = TableLayout.FILL;
      double p = TableLayout.PREFERRED;
      double size[][] =
         {{f, f, 16},    // Columns
          {p, p}};   // Rows

      this.setLayout(new TableLayout(size)); 
   }

   public void nameChanged(ConfigElementEvent evt)
   {
      System.out.println("Name changed...");
   }
   public void propertyValueChanged(ConfigElementEvent evt)
   {
      System.out.println("Property value changed...");
   }
   public void propertyValueAdded(ConfigElementEvent evt)
   {
      System.out.println("Property value added...");
   }
   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      System.out.println("Property value removed...");
   }
   
   private ConfigElement      mConfigElement = null;
}
