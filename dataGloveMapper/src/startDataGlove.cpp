
//TODO check

/**
 * @file startDataGlove
 * file name: startDataGlove.cpp
 * @author Betti Oesterholz
 * @date 05.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This programm runs the data glove programm.
 *
 *
 * Copyright (C) @c GPL3 2014 Betti Oesterholz
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
05.08.2014  Oesterholz  created
*/

//TODO rework


#include "version.h"

#include "cDataGloveDGTechVHand.h"


#include "cEvaluateDataGloveState.h"
#include "cThread.h"
#include "cThreadMessageHandler.h"


#include <string>
#include <iostream>
#include <fstream>


using namespace std;
using namespace nDataGlove;
using namespace nDataGlove::nMapper;
using namespace nDataGlove::nModelDataGloveDGTechVHand;


int main(int argc, char* argv[]){
	
	//loading the parameter
	string szDataGlovePath( "/dev/ttyUSB0" );
	string szDataGloveStatesPath( "./map/mapDataGlove.txt" );
	
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
		//load data glove states
		cEvaluateDataGloveState evaluateDataGloveState;
		const bool bStatesLoaded = evaluateDataGloveState.loadDataGloveStates(
			szDataGloveStatesPath );
		
		if ( ! bStatesLoaded ) {
			//Error: data glove states could not be loaded
			return -1;
		}
		
		cThreadMessageHandler threadMessageHandler;
		
		cDataGloveDGTechVHand dataGloveDGTechVHand( szDataGlovePath.c_str() );
		threadMessageHandler.setDataGloveDevice( &dataGloveDGTechVHand );
		threadMessageHandler.setEvaluateDataGloveState( &evaluateDataGloveState );
		
		dataGloveDGTechVHand.startSampling( 1 );
		
		//start the data glove message handler thread
		threadMessageHandler.start();
		//wait till the thread is done
		cThread::msleep( 10000 );
		
		
		dataGloveDGTechVHand.stopSampling();
		
		threadMessageHandler.quit();
		//TODO needed?: threadMessageHandler.join();
		
		while ( threadMessageHandler.isRunning() ) {
			
			cThread::shortSleep();
		}
		
	} else {
		cout<<"The device \""<<szDataGlovePath<<"\" is not a valid data glove DGTech VHand."<<endl;
	}
	
	
	return 0;
}
















