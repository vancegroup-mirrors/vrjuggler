/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

// library of file utility functions, useful to IRIS Performer apps

// Performer includes
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfEarthSky.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfTraverser.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>
#include <Performer/pf/pfSwitch.h>
#include <Performer/pr.h>

#include "pfFileIO.h" // my header.

std::string pfFileIO::filePath = ".:";

// trys to make performer load whatever filename is given
// uses pfFileIO:filePath as the current path
// uses pfdLoadFile to load the given filename
// exits application on failure
// outputs useful stuff, to let you know current status.
pfNode* pfFileIO::loadFile( const std::string& filename )
{
   pfdInitConverter( filename.c_str() );
   pfFilePath( filePath.c_str() );
   pfNode* node = pfdLoadFile( filename.c_str() );

   if (node == NULL)
   {
      cout << "pfFileIO::loadFile: COULDN'T FIND "<<filename.c_str()
           << "\tusing filepath: " << filePath << endl;
   }

   return node;
}

// get the name of the optimized file given the original name
std::string pfFileIO::optimizedName( std::string originalFltName )
{
   std::string cachedPreFix = ".cached";
   std::string cachedPostFix = ".pfb";
   return originalFltName + cachedPreFix + cachedPostFix;
}

// stores the geometry under node, in file "optimizedName"
// NOTE: usually pass in the name returned by optimizedName() function
void pfFileIO::writeOptimizedFile( pfNode* node, std::string optimizedName )
{
   pfdStoreFile( node, optimizedName.c_str() );
}

//: check the filename.  Does it look like it is already optimized?
//  returns true if so.
bool pfFileIO::isOptimized( const std::string& fileName )
{
   int size = fileName.find( ".pfb" );

   cout<<"OP: "<<size<<"\n"<<flush;
   
   if (size > 0)
      return true;
   else
      return false;
}

// use this function just like the pfdLoadFile performer function.
// this function automatically keeps track of optimised versions of the filename.
// if there is no .pfb file, or the .pfb is older than filename, then
// function generates a new pfb file.
// if there is a current pfb file, then functino uses it instead.
// TODO: add time stamp check - not implemented yet.
pfNode* pfFileIO::autoloadFile( const std::string& fileName, const pfFileIO::units& un )
{
   pfNode* node = NULL;
   
   std::string optimizedFileName = optimizedName( fileName );
   if (pfFileIO::isOptimized( fileName ))
   {
      cout<<"Loading(o) "<<fileName.c_str()<<".\n "<<flush;
      node = loadFile( fileName );
   }   
   else if (fileIO::fileExists(optimizedFileName))
   {
      // don't need to output this, because performer already does. :)
      //cout<<"Loading "<<optimizedFileName.c_str()<<"\n"<<flush;
      node = loadFile( optimizedFileName );
   }
   else
   {
      cout<<"Loading(c) "<<fileName.c_str()<<"... "<<flush;
      node = loadFile( fileName );
      if(node != NULL)
      {
         cout<<"Caching to "<<optimizedFileName.c_str()<<"...\n"<<flush;
         writeOptimizedFile( node, optimizedFileName );
      }
      // TODO: consider if the user has write access,
      //       for demos, they may not.
      // one workaround is to always distribute the app with the pfb's.
   }

   //assert( node != NULL );

   // convert it if requested.
   if ((NULL != node) && (un != NOCONVERT))
   {
      pfDCS* conversionDCS = pfFileIO::newConversionDCS( un );
      conversionDCS->addChild( node );
      return conversionDCS;
   }
   else
   {
      return node;
   }
}
