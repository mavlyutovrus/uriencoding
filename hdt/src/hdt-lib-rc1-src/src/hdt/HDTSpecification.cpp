/*
 * File: HDTSpecification.cpp
 * Last modified: $Date: 2012-08-13 23:00:07 +0100 (lun, 13 ago 2012) $
 * Revision: $Revision: 222 $
 * Last modified by: $Author: mario.arias $
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Contacting the authors:
 *   Mario Arias:               mario.arias@gmail.com
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *
 */

#include <sstream>

#include <HDTSpecification.hpp>
#include "../util/propertyutil.h"

namespace hdt {


HDTSpecification::HDTSpecification() {

}

HDTSpecification::HDTSpecification(std::string &filename) {
	if(filename!=""){
		try {
			PropertyUtil::read(filename.c_str(), map);
		} catch (char *except) {
			std::cerr << "WARNING: Could not read config file, using default options." << std::endl;
		}
	}
}

void HDTSpecification::setOptions(std::string options) {
	std::istringstream strm(options);
	std::string singleOption;
	while(getline(strm, singleOption, ';') ){
		size_t pos = singleOption.find(':');

		if(pos!=std::string::npos) {
			std::string property = singleOption.substr(0, pos);
			std::string value = singleOption.substr(pos+1);
			//std::cout << "Property= "<< property << "\tValue= " << value << std::endl;
			map[property] = value;
		}
	}
}

std::string HDTSpecification::get(std::string key) {
	return map[key];
}

void HDTSpecification::set(std::string key, std::string value) {
	map[key] = value;
}


}
