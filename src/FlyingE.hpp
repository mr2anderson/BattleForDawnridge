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


#include "CameraDependentPopUpElement.hpp"
#include "Clock.hpp"


#pragma once


template<typename T> class FlyingE : public CameraDependentPopUpElement {
public:
	FlyingE() = default;

	void run(uint32_t windowW, uint32_t windowH) override {
        this->clock.restart();
        this->dst = 128;
    }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(this->t, states);
    }
	Events click() override {
        return Events();
    }
	void update() override {
        float dt = this->clock.getSecondsAsFloat();

        this->setTransparentColor(dt);
        this->setPosition(dt);

        if (dt >= TIME) {
            this->finish();
        }
    }
protected:
    void set(const T &t1) {
        this->t = t1;
        this->startX = this->t.getPosition().x;
        this->startY = this->t.getPosition().y;
    }

    virtual void setTransparentColor(float dt) = 0;
    sf::Color getTransparencyLevel(float dt) const {
        sf::Color color = sf::Color::White;
        color.a = 255 - std::min(255.f, 255 * dt / TIME);
        return color;
    }
    T* getTPtr() {
        return &this->t;
    }
private:
    T t;
    float startX, startY;
	Clock clock;
	float dst;

    static constexpr float TIME = 0.625;

    void setPosition(float dt) {
        this->t.setPosition(this->calcPosition(dt));
    }
	sf::Vector2f calcPosition(float dt) const {
        float v = this->dst / TIME;

        float currentX = this->startX;
        float currentY = this->startY - v * dt;

        return sf::Vector2f(currentX, currentY);
    }
};