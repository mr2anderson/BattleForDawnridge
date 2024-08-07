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


#include "PAKScreen.hpp"
#include "Textures.hpp"
#include "Fonts.hpp"
#include "Sounds.hpp"
#include "Music.hpp"
#include "Locales.hpp"
#include "SoundQueue.hpp"
#include "ColorTheme.hpp"
#include "ClueManager.hpp"
#include "Label.hpp"
#include "SoundQueue.hpp"


PAKScreen* PAKScreen::singletone = nullptr;


bool PAKScreen::run(sf::RenderWindow &window) {
	this->init(window);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					return false;
				}
				Music::get()->get("intro")->stop();
				return true;
			}
		}
		this->drawEverything(window);
	}
}
void PAKScreen::init(sf::RenderWindow &window) {
	window.setMouseCursorVisible(false);

    Music::get()->get("intro")->play();
    Music::get()->get("intro")->setVolume(50);

	this->s.setTexture(*Textures::get()->get("loading_screen"));
	this->s.setPosition(window.getSize().x - this->s.getLocalBounds().width, window.getSize().y - this->s.getLocalBounds().height);

	this->t.setFont(*Fonts::get()->get("1"));
	this->t.setString(*Locales::get()->get("press_any_key_to_continue"));
	this->t.setCharacterSize(31);
	this->t.setFillColor(sf::Color::White);
	this->t.setOutlineColor(sf::Color::Black);
	this->t.setOutlineThickness(2);
	this->t.setPosition((window.getSize().x - t.getLocalBounds().width) / 2, window.getSize().y - t.getLocalBounds().height - 125);
}
void PAKScreen::drawEverything(sf::RenderWindow& window) {
	window.clear(sf::Color::Black);
	window.draw(this->s);
	window.draw(ClueManager::get()->getClueLabel(window.getSize().x, window.getSize().y));
	window.draw(this->t);
	window.display();
}
void PAKScreen::handleEvent(Events& e) {
	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e.at(i))) {
			this->handlePlaySoundEvent(playSoundEvent);
		}
		else if (std::shared_ptr<NextClueEvent> nextClueEvent = std::dynamic_pointer_cast<NextClueEvent>(e.at(i))) {
			this->handleNextClueEvent(nextClueEvent);
		}
	}
}
void PAKScreen::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e) {
	SoundQueue::get()->push(Sounds::get()->get(e->getSoundName()));
}
void PAKScreen::handleNextClueEvent(std::shared_ptr<NextClueEvent> e) {
	ClueManager::get()->nextClue();
}