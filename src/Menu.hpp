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


#include <queue>
#include "CloseMenuEvent.hpp"
#include "StartGameEvent.hpp"
#include "LoadGameEvent.hpp"
#include "GenerateChooseSaveWindowEvent.hpp"
#include "GenerateNetworkGameSettingsWindowEvent.hpp"
#include "InvertIsServerStateEvent.hpp"
#include "DeleteSaveEvent.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "ChooseLanguageEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "MenuResponse.hpp"
#include "MenuBg.hpp"


#pragma once


class Menu {
public:
    Menu(sf::RenderWindow &window);
    Menu(const Menu& copy) = delete;
    MenuResponse run(sf::RenderWindow& window);
private:
    bool alreadyFinished;
    bool closeMenu;
	std::optional<MenuResponse> response;
    std::queue<std::shared_ptr<Event>> events;
    std::shared_ptr<PopUpElement> element;
	std::vector<Button> buttons;
	MenuBg bg;

	void drawEverything(sf::RenderWindow& window);
    void processEvents(sf::RenderWindow& window);
    void addButtonClickEventToQueue();
    void addEvents(Events &e);

    void handleEvent(std::shared_ptr<Event> e, sf::RenderWindow& window);
    void handleSoundEvent(std::shared_ptr<PlaySoundEvent> e);
    void handleCreateEEvent(std::shared_ptr<CreateEEvent> e, sf::RenderWindow& window);
    void handleCloseMenuEvent(std::shared_ptr<CloseMenuEvent> e);
    void handleStartGameEvent(std::shared_ptr<StartGameEvent> e);
    void handleLoadGameEvent(std::shared_ptr<LoadGameEvent> e);
    void handleGenerateChooseSaveWindowEvent(std::shared_ptr<GenerateChooseSaveWindowEvent> e);
    void handleGenerateNetworkGameSettingsWindowEvent(std::shared_ptr<GenerateNetworkGameSettingsWindowEvent> e);
    void handleInvertIsServerStateEvent(std::shared_ptr<InvertIsServerStateEvent> e);
    void handleDeleteSaveEvent(std::shared_ptr<DeleteSaveEvent> e);
	void handleChooseLanguageEvent(std::shared_ptr<ChooseLanguageEvent> e);
};