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

#include "socketUtil.h"

void main(int argc, char** argv)
{
    std::string address = "129.186.232.64";
    int port = 4550;
    
    if (argc == 2)
	address = argv[1];
    
    if (argc == 3)
	port = atoi(argv[2]);
    //-------------------------------------------
    
    socketUtil sUtil;
    socketUtil::Message message;
    
    
    sUtil.setRole( socketUtil::CLIENT );
    sUtil.setRemote(address, port);
    
    sUtil.connect();
    
    for (int y = 0; y < 500000; ++y)
    {
	cout<<"."<<flush;
	sUtil.collectIncomingData();
	
	for (int x = 0; sUtil.queueSize(); ++x)
	{
	    sUtil.getCurrentMessage( message );
	    sUtil.dequeue();
	    switch(message.messageType)
	    {
	    case socketUtil::STRING:
		cout<< "STRING: "<<message.data<<"\n"<<flush;
		break;
	    case socketUtil::COMMAND:
		cout<< "COMMAND: "<<message.data<<"\n"<<flush;
		break;
	    }
	}
    }

    sUtil.disconnect();
}
