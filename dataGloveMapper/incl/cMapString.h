
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
	 * The map with the patterns and the mapper targets for them.
	 * Elements:
	 * 	key: the pattern, which mapping should result in the value
	 * 	value: the mapper which should be used to evaluate the target
	 *
	 * If the map with pairs mapPairs is very big (lots of entries) the
	 * search will be slow. To speed up the search sub mappers will be used
	 * if optimized.
	 *
	 * @see mapPairs
	 */
	std::map< std::string, cMapString * > mapIndex;
	
	
	/**
	 * The falgs for regex.
	 * @see regex()
	 */
	std::regex::flag_type flagsRegex;
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
public:

	/**
	 * The standard constructor for the mapper.
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
	
	
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	/**
	 * The constructor for the mapper with given pairs.
	 */
	cMapString( tMapTarget inDefaultValue,
			const std::regex::flag_type inFlagsRegex,
			const std::map< std::string, tMapTarget > inMapPairs ) :
			mapPairs( inMapPairs ),
			defaultValue( inDefaultValue ), flagsRegex( inFlagsRegex ) {
		//nothing to do
	}
	
	
	/**
	 * destructor
	 */
	~cMapString() {
		
		for ( typename std::map< std::string, cMapString * >::const_iterator
				itrMapIndex = mapIndex.begin();
				itrMapIndex != mapIndex.end();
				++itrMapIndex ) {
			
			if ( itrMapIndex->second != NULL ) {
				delete (itrMapIndex->second);
			}
		}
	}
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
	
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
	
	
	
	
	//TODO check
	
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
		
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
		if ( ! mapIndex.empty() ) {
			
			const unsigned int uiIndexSize = (mapIndex.begin())->first.size();
			
			if ( ( flagsRegex & std::regex_constants::icase ) == 0 ) {
				
				for ( typename std::map< std::string, cMapString * >::const_iterator
						itrMapIndex = mapIndex.begin();
						itrMapIndex != mapIndex.end();
						++itrMapIndex ) {
		
					//case sensitive
					if ( strToMapString.compare( 0, uiIndexSize, itrMapIndex->first ) == 0 ) {
						//string begin found
						if ( itrMapIndex->second != NULL ) {
							//matching string found -> use its mapper to evaluate a match
							return itrMapIndex->second->operator()( strToMapString );
						}
					}
				}
			} else { //ignore case
				std::string strToMapStringUpper = strToMapString;
				std::transform(strToMapStringUpper.begin(),
					strToMapStringUpper.end(),strToMapStringUpper.begin(),
					::toupper);
					
				for ( typename std::map< std::string, cMapString * >::const_iterator
						itrMapIndex = mapIndex.begin();
						itrMapIndex != mapIndex.end();
						++itrMapIndex ) {
					if ( strToMapStringUpper.compare( 0, uiIndexSize, itrMapIndex->first ) == 0 ) {
						//string begin found
						if ( itrMapIndex->second != NULL ) {
							//matching string found -> use its mapper to evaluate a match
							return itrMapIndex->second->operator()( strToMapStringUpper );
						}
					}
				}
			}
		}
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
		
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
	
	
	/**
	 * Optimize the data, so the search is speeded up.
	 *
	 * @returns true if an optimization occured, else false
	 */
	bool optimize( const unsigned int uiMinNumberOfIndexChars = 0 ) {
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
		if ( ( flagsRegex & std::regex_constants::ECMAScript ) == 0 ) {
			//don't try to optimize if not ECMAScript
			return false;
		}
		if ( mapPairs.size() <= 4 ) {
			//don't try to optimize if just some pairs existe
			return false;
		}
		
		//clear old mapIndex
		for ( typename std::map< std::string, cMapString * >::const_iterator
				itrMapIndex = mapIndex.begin();
				itrMapIndex != mapIndex.end();
				++itrMapIndex ) {
			
			if ( itrMapIndex->second != NULL ) {
				delete (itrMapIndex->second);
			}
		}
		mapIndex.clear();
		
		
		//the sorted entries
		//Elements:
		// 	key: the index
		// 	value.key: the pattern for the index, which mapping should
		// 		result in the value
		// 	value.value: the target for the index to which the key pattern
		// 		should be mapped
		std::map< std::string, std::map< std::string, tMapTarget > >
			mapIndexPairs;
		bool bIndexPairsAreGood;
		bool bMatchingMapFound;
		std::string szNewIndex;
		
		//stop if one of ths chars is found
		const std::string szStopChars = ".*+?{\\(|";
		
		typename std::map< std::string, tMapTarget >::const_iterator itrNextMapPair;
		//try to sort the pairs into different categoriers with the same start
		
		//index is the string, for wich the pairs are sorted
		//how many characters the index has
		unsigned int uiCharsToCompare;
		for ( unsigned int uiActualIndex = uiMinNumberOfIndexChars;  ;
				++uiActualIndex ) {
			
			uiCharsToCompare = uiActualIndex + 1;
			mapIndexPairs.clear();
			bIndexPairsAreGood = true;
			
			for ( typename std::map< std::string, tMapTarget >::const_iterator
					itrMapPair = mapPairs.begin();
					itrMapPair != mapPairs.end();
					++itrMapPair ) {
				
				if ( itrMapPair->first.size() <= uiActualIndex ) {
					//not enough characters
					bIndexPairsAreGood = false;
					break;
				}
				//if '.', '*', '+', '?', '{', '\', '(', '|' or string end create
				//mappers for one less char (if possible)
				if ( szStopChars.find( itrMapPair->first[ uiActualIndex ] ) !=
						std::string::npos ) {
					bIndexPairsAreGood = false;
					break;
				}
				//check if a mapIndexPairs entry exists for the given key
				bMatchingMapFound = false;
				
				if ( ( flagsRegex & std::regex_constants::icase ) == 0 ) {
					for ( typename std::map< std::string, std::map< std::string,
							tMapTarget > >::iterator
								itrActualPair = mapIndexPairs.begin();
							itrActualPair != mapIndexPairs.end();
							++itrActualPair ) {
						
						if ( itrMapPair->first.compare( 0, uiCharsToCompare,
								itrActualPair->first ) == 0 ) {
							//matching string found -> use its mapper to evaluate a match
							bMatchingMapFound = true;
							itrActualPair->second.insert( *itrMapPair );
							break;
						}
					}
				} else { //ignore case
					std::string strToMapStringUpper = itrMapPair->first;
					std::transform(strToMapStringUpper.begin(),
						strToMapStringUpper.end(),strToMapStringUpper.begin(),
						::toupper);
						
					for ( typename std::map< std::string, std::map< std::string,
							tMapTarget > >::iterator
								itrActualPair = mapIndexPairs.begin();
							itrActualPair != mapIndexPairs.end();
							++itrActualPair ) {
						
						if ( strToMapStringUpper.compare( 0, uiCharsToCompare,
								itrActualPair->first ) == 0 ) {
							//matching string found -> use its mapper to evaluate a match
							bMatchingMapFound = true;
							itrActualPair->second.insert( *itrMapPair );
							break;
						}
					}
				}
				
				if ( ! bMatchingMapFound ) {
					//create a new mapIndexPairs entry
					szNewIndex = itrMapPair->first.substr( 0, uiActualIndex + 1 );
					
					itrNextMapPair = itrMapPair;
					itrNextMapPair++;
					
					if ( ( flagsRegex & std::regex_constants::icase ) == 0 ) {
						//case insensitiv -> use uppercase index
						std::transform(szNewIndex.begin(), szNewIndex.end(),
							szNewIndex.begin(), ::toupper);
					}
					
					mapIndexPairs.insert(
						std::pair< std::string, std::map< std::string, tMapTarget > >(
							szNewIndex,
							std::map< std::string, tMapTarget >( itrMapPair, itrNextMapPair ) ) );
				}
			}
			
			if ( ! bIndexPairsAreGood ) {
				return false;  //can not find valid optimization
			}  //else bIndexPairsAreGood
			if ( 1 < mapIndexPairs.size() ) {
				//actual pairs are good pairs -> good index found
				
				cMapString * pMaperForString;
				
				for ( typename std::map< std::string, std::map< std::string,
							tMapTarget > >::const_iterator
						itrGoodPairs = mapIndexPairs.begin();
						itrGoodPairs != mapIndexPairs.end();
						++itrGoodPairs ) {
					
					pMaperForString = new cMapString( defaultValue, flagsRegex,
						itrGoodPairs->second );
					
					mapIndex.insert( std::pair< std::string, cMapString * >(
						itrGoodPairs->first, pMaperForString ) );
					
					//optimize new mapper
					pMaperForString->optimize( uiActualIndex + 1 );
				}
				break;
			}
		}
		return ( ! mapIndex.empty() );
#else //FEATURE_READ_DATA_TEXT_WITH_REGEX
		//don't optimize if not FEATURE_READ_DATA_TEXT_WITH_REGEX
		return false;
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	}
	
	
};//end class cMapString


#endif //___C_MAP_STRING__





