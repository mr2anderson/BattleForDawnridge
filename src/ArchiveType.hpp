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


#pragma once


#define USE_EOS_PORTABLE_ARCHIVE



#if defined(USE_EOS_PORTABLE_ARCHIVE)


#include "eos/portable_archive.hpp"
typedef eos::portable_iarchive iarchive;
typedef eos::portable_oarchive oarchive;


#else


// Boost binary archives are implementation-definied
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
typedef boost::archive::text_iarchive iarchive;
typedef boost::archive::text_oarchive oarchive;


#endif