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
#include "StartLocalGameEvent.hpp"
#include "LoadLocalGameEvent.hpp"
#include "DeleteSaveEvent.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "ChooseLanguageEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "MenuResponse.hpp"
#include "StartNetworkGameEvent.hpp"
#include "LoadNetworkGameEvent.hpp"
#include "ConnectToRoomEvent.hpp"
#include "MoveHorizontalSelectionWindowDownEvent.hpp"
#include "MoveHorizontalSelectionWindowUpEvent.hpp"
#include "ClosePopUpElementEvent.hpp"
#include "ChangeSoundVolumeEvent.hpp"
#include "ChangeMusicVolumeEvent.hpp"
#include "TurnOnIlluminationEvent.hpp"
#include "TurnOffIlluminationEvent.hpp"
#include "ChangeIlluminationBrightnessEvent.hpp"


#pragma once


class Menu {
public:
    Menu(sf::RenderWindow &window, const boost::optional<StringLcl> &additionalWelcomeMsg);
    Menu(const Menu& copy) = delete;
    MenuResponse run(sf::RenderWindow& window);
private:
    bool alreadyFinished;
    bool closeMenu;
	boost::optional<MenuResponse> response;
    std::queue<std::shared_ptr<Event>> events;
    std::shared_ptr<PopUpElement> element;
    std::vector<Button> buttons;

    void generateButtons();
    void regenerateButtons();

	void drawEverything(sf::RenderWindow& window);
    void drawButtons(sf::RenderWindow& window);

    void processEvents();
    void addButtonClickEventToQueue();
    void addEvents(Events &e);

    void handleEvent(std::shared_ptr<Event> e);
    void handleSoundEvent(std::shared_ptr<PlaySoundEvent> e);
    void handleCreateEEvent(std::shared_ptr<CreateEEvent> e);
    void handleCloseMenuEvent(std::shared_ptr<CloseMenuEvent> e);
    void handleStartLocalGameEvent(std::shared_ptr<StartLocalGameEvent> e);
    void handleLoadLocalGameEvent(std::shared_ptr<LoadLocalGameEvent> e);
    void handleStartNetworkGameEvent(std::shared_ptr<StartNetworkGameEvent> e);
    void handleLoadNetworkGameEvent(std::shared_ptr<LoadNetworkGameEvent> e);
    void handleConnectGameEvent(std::shared_ptr<ConnectToRoomEvent> e);
    void handleDeleteSaveEvent(std::shared_ptr<DeleteSaveEvent> e);
	void handleChooseLanguageEvent(std::shared_ptr<ChooseLanguageEvent> e);
    void handleMoveHorizontalSelectionWindowUpEvent(std::shared_ptr<MoveHorizontalSelectionWindowUpEvent> e);
    void handleMoveHorizontalSelectionWindowDownEvent(std::shared_ptr<MoveHorizontalSelectionWindowDownEvent> e);
    void handleClosePopUpElementEvent(std::shared_ptr<ClosePopUpElementEvent> e);
    void handleChangeSoundVolumeEvent(std::shared_ptr<ChangeSoundVolumeEvent> e);
    void handleChangeMusicVolumeEvent(std::shared_ptr<ChangeMusicVolumeEvent> e);
    void handleTurnOnIlluminationEvent(std::shared_ptr<TurnOnIlluminationEvent> e);
    void handleTurnOffIlluminationEvent(std::shared_ptr<TurnOffIlluminationEvent> e);
    void handleChangeIlluminationBrightnessEvent(std::shared_ptr<ChangeIlluminationBrightnessEvent> e);
};