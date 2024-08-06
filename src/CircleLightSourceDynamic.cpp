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


#include "CircleLightSourceDynamic.hpp"
#include "GlobalClock.hpp"


CircleLightSourceDynamic::CircleLightSourceDynamic(float centerX, float centerY, float avRadius, float deltaScale, float seconds) : CircleLightSource(centerX, centerY, avRadius* (1 + deltaScale)) {
	this->avRadius = avRadius;
	this->deltaScale = deltaScale;
	this->seconds = seconds;
}
float CircleLightSourceDynamic::getRadius() const {
	uint32_t roundMs = 1000 * this->seconds;
	uint32_t roundMsHalf = roundMs / 2;
	uint32_t currentRoundMs = GlobalClock::get()->getMs() % roundMs;
	uint32_t currentRoundMsDst = std::max(currentRoundMs, roundMsHalf) - std::min(currentRoundMs, roundMsHalf);

	float currentRoundMsDstFrac = (float)currentRoundMsDst / (float)roundMsHalf;
	float fFrac = this->getFormatedFunctionValue(currentRoundMsDstFrac);
	float currentDeltaScale = fFrac * (2 * this->deltaScale);

	return this->avRadius * (1 - this->deltaScale + currentDeltaScale);
}