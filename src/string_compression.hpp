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


#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <sstream>


#pragma once


namespace bfdlib {
    namespace string_compression {

        static std::string get_compressed(const std::string& input) {
            std::string compressed;
            boost::iostreams::filtering_ostream out;

            out.push(boost::iostreams::bzip2_compressor());
            out.push(boost::iostreams::back_insert_device(compressed));

            out.write(input.data(), input.size());
            out.flush();

            return compressed;
        }
        static std::string get_decompressed(const std::string& input) {
            std::ostringstream decompressed;
            boost::iostreams::filtering_istream in;

            in.push(boost::iostreams::bzip2_decompressor());
            in.push(boost::iostreams::array_source(input.data(), input.size()));

            decompressed << in.rdbuf();

            return decompressed.str();
        }
    }
}