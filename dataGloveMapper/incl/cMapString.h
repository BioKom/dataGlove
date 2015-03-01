
//TODO check


/**
 * @file cMapString
 * file name: cMapString.h
 * @author Betti Oesterholz
 * @date 24.01.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class template to map a string to a value.
 *
 *
 * Copyright (C) @c GPL3 2015 Betti Oesterholz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (GPL) as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
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
 * This file specifies the class template to map a string to a value.
 *
 *
 */
/*
History:
24.01.2015  Oesterholz  created
*/


#ifndef ___C_MAP_STRING__
#define ___C_MAP_STRING__


#include "version.h"

#include <string>
#include <map>

#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	#include <regex>
#endif //FEATURE_READ_DATA_TEXT_WITH_REGEX



template  <class tMapTarget> class cMapString{
protected:
	
//members
	
	
	/**
	 * The map with the patterns and the targets for them.
	 * Elements:
	 * 	key: the pattern, which mapping should result in the value
	 * 	value: the target to which the key pattern should be mapped
	 */
	std::map< std::string, tMapTarget > mapPairs;
	
	/**
	 * The default value.
	 * All strings wich can not be mapped, will be mapped to this value.
	 */
	tMapTarget defaultValue;
	
	
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	/**
	 * The falgs for regex.
	 * @see regex()
	 */
	std::regex::flag_type flagsRegex;
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
public:

	/**
	 * The standard constructor for the interval.
	 */
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	cMapString( tMapTarget inDefaultValue,
		const std::regex::flag_type inFlagsRegex = std::regex_constants::ECMAScript ) :
		defaultValue( inDefaultValue ), flagsRegex( inFlagsRegex ) {
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	cMapString( tMapTarget inDefaultValue ) : defaultValue( inDefaultValue ) {
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
		//nothing to do
	}
	
	/**
	 * @return the name of this class "cMapString"
	 */
	std::string getName() const {
		
		return "cMapString";
	}
	
	
	/**
	 * @return The default value.
	 * All strings wich can not be mapped, will be mapped to this value.
	 * @see defaultValue
	 */
	tMapTarget getDefaultValue() {
		
		return defaultValue;
	}
	
	
	/**
	 * @return The falgs for regex.
	 * @see flagsRegex
	 * @see regex()
	 */
	std::regex::flag_type getFlagsRegex() {
		
		return flagsRegex;
	}
	
	
	/**
	 * @param lNewMinimum The minimal value, which the interval contains.
	 * 	@see lMinimum
	 */
	void setMapPair( const std::string & stPattern, tMapTarget inTargetForPattern ) {
		
		mapPairs.insert( std::pair< std::string, tMapTarget >(
			stPattern, inTargetForPattern ) );
	}
	
	
	//optimize()
	
	/**
	 * This operator maps the given string to the target
	 *
	 * @see mapPairs
	 * @see defaultValue
	 * @see setMapPair()
	 * @param strToMapString the string to map
	 * @return the target value for the string, or the default target,
	 * 	if no patten exists which matches the given string
	 */
	tMapTarget operator()( const std::string & strToMapString ) {
		
		for ( typename std::map< std::string, tMapTarget >::const_iterator
				itrMapPair = mapPairs.begin();
				itrMapPair != mapPairs.end();
				++itrMapPair ) {
			
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			if ( std::regex_match( strToMapString, std::regex( itrMapPair->first,
					flagsRegex ) ) ) {
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			if ( itrMapPair->first.compare( strToMapString ) == 0 ) {
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
				//matching string found -> return its target
				return itrMapPair->second;
			}
		}//return the default value
		return defaultValue;
	}
	
};//end class cMapString


#endif //___C_MAP_STRING__





