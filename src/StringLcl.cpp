/*
 *  Battle for Dawnridge
 *  Copyright (C) 2024 mr2anderson
 *
 *  Battle for Dawnridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Battle for Dawnridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battle for Dawnridge.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <iostream>
#include "StringLcl.hpp"
#include "Locales.hpp"
#include "UTFEncoder.hpp"



StringLcl::StringLcl() = default;
StringLcl::StringLcl(const std::string& data) {
	this->data = data;
}
std::wstring StringLcl::get() const {
	try {
		std::wstring result;

		std::string buff;
		bool isLocaleKey = false;
		for (uint32_t i = 0; i < this->data.size(); i = i + 1) {
			if (this->data.at(i) == '{') {
				result = result + UTFEncoder::get()->utf8ToUtf16(buff);
				buff.clear();
				isLocaleKey = true;
				continue;
			}
			if (this->data.at(i) == '}') {
				isLocaleKey = false;
				result = result + *Locales::get()->get(buff);
				buff.clear();
				continue;
			}
			buff = buff + this->data.at(i);
		}
		result = result + UTFEncoder::get()->utf8ToUtf16(buff);
		return result;
	}
	catch (std::exception&) {
		std::cout << "Invalid string lcl format: " << this->data << std::endl;
		try {
			return UTFEncoder::get()->utf8ToUtf16(this->data);
		}
		catch (std::exception&) {
			return UTFEncoder::get()->utf8ToUtf16("COULDNT_FORMAT_STRING_LCL_CONTENT_REPORT_DEVELOPER");
		}
	}
}
std::string StringLcl::toRawString() const {
	return this->data;
}
StringLcl StringLcl::operator+(const StringLcl& b) const {
	return StringLcl(this->data + b.data);
}
StringLcl StringLcl::operator+(const std::string & b) const {
	return StringLcl(this->data + b);
}
StringLcl StringLcl::operator+(char c) const {
	StringLcl result(this->data);
	result.data.push_back(c);
	return result;
}
void StringLcl::clear() {
	this->data.clear();
}