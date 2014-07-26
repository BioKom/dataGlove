
//TODO check

/**
 * @file startDataGlove
 * file name: startDataGlove.cpp
 * @author Betti Oesterholz
 * @date 02.12.2012
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This programm runs the data glove programm.
 *
 * Copyright (C) @c GPL3 2010 Betti Oesterholz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This programm runs the data glove programm.
 * The programm will recive data from the data glove and translate them to
 * actions.
 *
 *
 * call: startDataGlove [DATA_GLOVE_PORT]
 * 	(Note: You will need root right for using the USB port.)
 *
 * parameters:
 * 	DATA_GLOVE_PORT
 * 		The path to the data glove port (e. g. "/dev/ttyS0")
 * 		standrt value: "/dev/ttyS0"
 */
/*
History:
02.12.2012  Oesterholz  created
*/

//TODO rework


#include "version.h"

#include "cDataGloveDGTechVHand.h"

#include "cMessageGetIdFromDataGlove.h"


#include <string>
#include <iostream>
#include <fstream>


using namespace std;
using namespace nDataGlove;
using namespace nDataGlove::nModelDataGloveDGTechVHand;


int main(int argc, char* argv[]){
	
	//loading the parameter
	//TODO
	string szDataGlovePath( "/dev/ttyUSB0" );
	
	/*if ( argc < 2 ){
		cout<<" call: startDataGlove [DATA_GLOVE_PORT}"<<endl;
		cout<<""<<endl;
		cout<<"  parameters:"<<endl;
		cout<<"  	DATA_GLOVE_PORT"<<endl;
		cout<<"  		The path to the data glove port (e. g. \"/dev/ttyS0\")"<<endl;
		cout<<"  		standrt value: \"/dev/ttyS0\""<<endl;
		cout<<endl;
		return 1;
	}*/
	if ( 1 < argc ){
		szDataGlovePath = string( argv[1] );
	}
	
	const bool isValid =
		cDataGloveDGTechVHand::isLiveDataGlove( szDataGlovePath.c_str() );
	
	if ( isValid ) {
		cout<<"The device \""<<szDataGlovePath<<"\" is a valid data glove DGTech VHand."<<endl;
		
		cDataGloveDGTechVHand dataGloveDGTechVHand(  szDataGlovePath.c_str()  );
		
		cMessageGetIdFromDataGlove * pMessageGetIdFromDataGlove =
			dataGloveDGTechVHand.getDataGloveID();
		
		if ( pMessageGetIdFromDataGlove != NULL ) {
			cout<<"Device type: "<< pMessageGetIdFromDataGlove->getDeviceTypeString() <<
				"  with microprocessor type: "<< pMessageGetIdFromDataGlove->getMicroTypeString()<<endl;
			cout<<"   Id: "<<pMessageGetIdFromDataGlove->getId()<<endl;
			cout<<"   Ip: "<<pMessageGetIdFromDataGlove->getIpString()<<
				"  mask: "<<pMessageGetIdFromDataGlove->getMaskString()<<
				"  gateway: "<<pMessageGetIdFromDataGlove->getGatewayString()<<endl;
		}
		
		
	} else {
		cout<<"The device \""<<szDataGlovePath<<"\" is not a valid data glove DGTech VHand."<<endl;
	}
	
	/*
	filebuf fbDataGlove;
	
	fbDataGlove.open(szDataGlovePath.c_str(), ios_base::binary );
	
	iostream streamDataGlove( &fbDataGlove );
	//open( szDataGlovePath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );
	
	cDataGloveDGTechVHand::isLiveDataGloveStraem( &streamDataGlove );
	
	fbDataGlove.close();
	*/
	/*TODO old
	//create object for data glove port
	cDataGloveSerialPort dataGlovePortReader( szDataGlovePath, 8 );
	
	//create object for data glove output
	cOutputDataGloveData dataGloveOutput;
	dataGlovePortReader.registerHandGestureListener( &dataGloveOutput );
	
	//init data glove port reader
	if ( ! dataGlovePortReader.init() ){
		cerr<<"Error: data glove port reader could not be init"<<endl;
		return 1;
	}
	//start data glove port reader
	if ( ! dataGlovePortReader.run() ){
		cerr<<"Error: data glove port reader has not run correctly"<<endl;
		return 2;
	}
	
	//destroy data glove port reader
	if ( ! dataGlovePortReader.destroy() ){
		cerr<<"Error: data glove port reader could not be destroyed"<<endl;
		return 3;
	}
	*/
	
	return 0;
}
















