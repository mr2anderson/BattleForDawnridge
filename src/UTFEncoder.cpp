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


#include <codecvt>
#include "UTFEncoder.hpp"



std::wstring UTFEncoder::utf8ToUtf16(const std::string &utf8) const {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::u16string utf16 = convert.from_bytes(utf8);
    std::wstring wstr(utf16.begin(), utf16.end());
    return wstr;
}
std::string UTFEncoder::utf16ToUtf8(const std::wstring& utf16) const {
    std::u16string u16str(utf16.begin(), utf16.end());
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::string utf8 = convert.to_bytes(u16str);
    return utf8;
}