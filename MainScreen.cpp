/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "MainScreen.hpp"


MainScreen* MainScreen::singletone = nullptr;


int32_t MainScreen::run(sf::RenderWindow& window) {
	this->init(window);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return -1;
			}
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					this->prepareToReturnToMenu(window);
					return 0;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (this->elements.empty()) {
					if (endMoveButton.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {
						this->changeMove();
					}
					else if (buildButton.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {
						this->createBuildMenu();
					}
					else {
						this->handleGameObjectClick();
					}
				}
				else {
					if (event.type == sf::Event::MouseButtonPressed) {
						this->handleEvent(this->elements.front()->click());
					}
				}
			}
		}
		this->drawEverything(window);
		Playlist::get()->update();
		this->removeFinishedElements();
		this->moveView();
		if (!this->elements.empty()) {
			this->elements.front()->update();
		}
	}
}
void MainScreen::init(sf::RenderWindow& window) {
	this->initMap("ridge");
	this->removeOldPopUpWindows();
	this->initPlayers();
	this->initMoveCtr();
	this->initHighlightTable();
	this->initGraphics(window);
	this->changeMove();
}
void MainScreen::initMap(const std::string& name) {
	this->gameObjects = new std::vector<GO*>;
	this->units = new std::vector<Unit*>;
	this->resourcePoints = new std::vector<ResourcePoint*>;
	this->territoryBuildings = new std::vector<TerritoryB*>;
	this->territoryOrigins = new std::vector<TerritoryOriginB*>;
	this->territoryConductors = new std::vector<TerritoryConductorB*>;

	std::ifstream file(std::string(ROOT) + "/" + name + ".tmx");
	std::string line;
	bool read = false;
	uint32_t y = 0;
	uint32_t x;
	uint32_t currentPlayerId = 0;
	while (std::getline(file, line)) {
		if (line == "  <data encoding=\"csv\">") {
			read = true;
		}
		else if (line == "</data>") {
			break;
		}
		if (!read) {
			continue;
		}
		std::stringstream ss(line);
		std::string word;
		x = 0;
		while (std::getline(ss, word, ',')) {
			if (word == "10") {
				this->add(new Forest(x, y));
			}
			else if (word == "1") {
				Castle* c = new Castle(x, y, &this->players[currentPlayerId]);
				c->setMaxHp();
				this->add(c);
				currentPlayerId = currentPlayerId + 1;
			}
			else if (word == "18") {
				this->add(new Iron(x, y));
			}
			else if (word == "14") {
				this->add(new Stone(x, y));
			}
			else if (word == "22") {
				this->add(new Mountains(x, y));
			}
			x = x + 1;
		}
		y = y + 1;
	}
	file.close();

	this->plains = PlainsGeneration(x + 1, y + 1);
}
void MainScreen::removeOldPopUpWindows() {
	while (!this->elements.empty()) {
		PopUpElement* w = this->elements.front();
		delete w;
		this->elements.pop();
	}
}
void MainScreen::initPlayers() {
	this->players[0] = Player(1);
	this->players[1] = Player(2);
	return;
}
void MainScreen::initMoveCtr() {
	this->move = 0;
}
void MainScreen::initHighlightTable() {
	this->highlightTable.clear();
}
void MainScreen::initGraphics(sf::RenderWindow &window) {
	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
	this->view = new sf::View(window.getDefaultView());
	this->endMoveButton = Button(this->windowW - 20 - 150, this->windowH - 20 - 30, 150, 30, std::nullopt, L"Êîíåö õîäà");
	this->buildButton = Button(this->windowW - 20 - 150 - 20 - 64, this->windowH - 20 - 64, 64, 64, "hammer_icon", L"");
}
void MainScreen::handleEvent(const Event &e) {
	this->handleTryToAttackEvent(e);
	this->handleTryToTradeEvent(e);
	this->handleAddResourceEvent(e);
	this->handleSubResourceEvent(e);
	this->handleAddResourcesEvent(e);
	this->handleSubResourcesEvent(e);
	this->handleChangeHighlightEvent(e);
	this->handleCollectEvent(e);
	this->handleTryToUpgradeEvent(e);
	this->handleAddHpEvent(e);
	this->handleDecreaseUpgradeMovesLeftEvent(e);
	this->handleIncreaseLevelEvent(e);
	this->handleDecreaseCurrentTradeMovesLeft(e);
	this->handleTryToBuild(e);
	this->handleBuild(e);
	this->handlePlaySoundEvent(e);
	this->handleCreatePopUpElementEvent(e);
}
void MainScreen::handleTryToAttackEvent(const Event& e) {
	for (const auto& a : e.tryToAttack) {

	}
}
void MainScreen::handleTryToTradeEvent(const Event& e) {
	for (const auto& a : e.tryToTrade) {
		Market* m = std::get<Market*>(a);
		Trade t = std::get<Trade>(a);
		if (this->getCurrentPlayer()->getResource(t.sell.type) >= t.sell.n) {
			this->handleEvent(m->doTrade(t));
		}
		else {
			MessageW* w = new MessageW("click", "click", 
				L"ÍÅÄÎÑÒÀÒÎ×ÍÎ ÐÅÑÓÐÑÎÂ\n"
				"Âû íå ìîæåòå ñîâåðøèòü ýòó ñäåëêó.");
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddResourceEvent(const Event& e) {
	for (const auto& a : e.addResource) {
		this->getCurrentPlayer()->addResource(a);
	}
}
void MainScreen::handleSubResourceEvent(const Event& e) {
	for (const auto& a : e.subResource) {
		this->getCurrentPlayer()->subResource(a);
	}
}
void MainScreen::handleAddResourcesEvent(const Event& e) {
	for (const auto& a : e.addResources) {
		this->getCurrentPlayer()->addResources(a);
	}
}
void MainScreen::handleSubResourcesEvent(const Event& e) {
	for (const auto& a : e.subResources) {
		this->getCurrentPlayer()->subResources(a);
	}
}
void MainScreen::handleChangeHighlightEvent(const Event& e) {
	for (const auto& a : e.changeHighlight) {
		const Unit* u = std::get<0>(a);
		uint32_t x = std::get<1>(a);
		uint32_t y = std::get<2>(a);
		if (x >= this->plains.getW() or y >= this->plains.getH()) {
			continue;
		}
		this->highlightTable.mark(x, y, u);
	}
}
void MainScreen::handleCollectEvent(const Event& e) {
	for (const auto& a : e.collect) {
		ResourcePoint* resourcePoint = std::get<ResourcePoint*>(a);
		uint32_t n = std::get<uint32_t>(a);
		this->getCurrentPlayer()->addResource(Resource(resourcePoint->getResourceType(), n));
		resourcePoint->subHp(n);
	}
}
void MainScreen::handleTryToUpgradeEvent(const Event& e) {
	for (const auto& a : e.tryToUpgrade) {
		UpgradeableB* b = std::get<UpgradeableB*>(a);
		Resources cost = std::get<Resources>(a);
		if (this->getCurrentPlayer()->getResources() >= cost) {
			this->handleEvent(b->startUpgrade());
		}
		else {
			MessageW* w = new MessageW("click", "click", 
				L"ÍÅÄÎÑÒÀÒÎ×ÍÎ ÐÅÑÓÐÑÎÂ\n"
				"Âû íå ìîæåòå óëó÷øèòü ýòî çäàíèå.");
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddHpEvent(const Event& e) {
	for (const auto& a : e.addHp) {
		HPGO* go = std::get<HPGO*>(a);
		uint32_t n = std::get<uint32_t>(a);
		go->addHp(n);
	}
}
void MainScreen::handleDecreaseUpgradeMovesLeftEvent(const Event& e) {
	for (const auto& a : e.decreaseUpgradeMovesLeft) {
		a->decreaseUpgradeMovesLeft();
	}
}
void MainScreen::handleIncreaseLevelEvent(const Event& e) {
	for (const auto& a : e.increaseLevel) {
		a->increaseLevel();
	}
}
void MainScreen::handleDecreaseCurrentTradeMovesLeft(const Event& e) {
	for (const auto& a : e.decreaseCurrentTradeMovesLeft) {
		a->decreaseCurrentTradeMovesLeft();
	}
}
void MainScreen::handleTryToBuild(const Event& e) {
	for (const auto& a : e.tryToBuild) {
		if (this->getCurrentPlayer()->getResources() >= a->getCost()) {
			this->addPopUpWindow(new BuildingMode(a, this->view, this->gameObjects, this->territoryBuildings, this->getCurrentPlayer()));
		}
		else {
			MessageW* w = new MessageW("", "click",
				L"ÍÅÄÎÑÒÀÒÎ×ÍÎ ÐÅÑÓÐÑÎÂ\n"
				"Âû íå ìîæåòå ïîñòðîèòü ýòî çäàíèå");
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleBuild(const Event& e) {
	for (const auto& a : e.build) {
		this->add(a);
	}
}
void MainScreen::handlePlaySoundEvent(const Event& e) {
	for (const auto& a : e.playSound) {
		SoundQueue::get()->push(Sounds::get()->get(a));
	}
}
void MainScreen::handleCreatePopUpElementEvent(const Event& e) {
	for (const auto& a : e.createE) {
		this->addPopUpWindow(a);
	}
}
void MainScreen::removeFinishedElements() {
	bool remove = false;
	while (!this->elements.empty()) {
		if (!this->elements.front()->finished()) {
			break;
		}
		delete this->elements.front();
		this->elements.pop();
		remove = true;
	}
	if (remove and !this->elements.empty()) {
		this->handleEvent(this->elements.front()->run(this->windowW, this->windowH));
	}
}
void MainScreen::changeMove() {
	this->move = this->move + 1;
	SoundQueue::get()->push(Sounds::get()->get("click"));
	this->updatePlayerViewPoint();
	this->highlightTable.clear();
	for (uint32_t i = 0; i < this->gameObjects->size(); i = i + 1) {
		this->handleEvent(this->gameObjects->at(i)->newMove(*this->getCurrentPlayer()));
	}
}
void MainScreen::createBuildMenu() {
	std::vector<SelectionWComponent> components;
	components.emplace_back("hammer_icon", L"Âûéòè", true, true, Event());

	Event buildEvent;
	buildEvent.tryToBuild.push_back(new Road(0, 0, this->getCurrentPlayer(), this->territoryOrigins, this->territoryConductors));
	components.emplace_back(Road().getTextureName(), GET_BUILD_DESCRIPTION(new Road()), true, true, buildEvent);
	
	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Farm(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Farm().getTextureName(), GET_BUILD_DESCRIPTION(new Farm()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Sawmill(0, 0, this->getCurrentPlayer(), this->resourcePoints));
	components.emplace_back(Sawmill().getTextureName(), GET_BUILD_DESCRIPTION(new Sawmill()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Quarry(0, 0, this->getCurrentPlayer(), this->resourcePoints));
	components.emplace_back(Quarry().getTextureName(), GET_BUILD_DESCRIPTION(new Quarry()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Mine(0, 0, this->getCurrentPlayer(), this->resourcePoints));
	components.emplace_back(Mine().getTextureName(), GET_BUILD_DESCRIPTION(new Mine()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Market(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Market().getTextureName(), GET_BUILD_DESCRIPTION(new Market()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Wall(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Wall().getTextureName(), GET_BUILD_DESCRIPTION(new Wall()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Castle(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Castle().getTextureName(), GET_BUILD_DESCRIPTION(new Castle()), true, true, buildEvent);

	SelectionW* w = new SelectionW("click", "click", components);
	this->addPopUpWindow(w);
}
std::wstring MainScreen::GET_BUILD_DESCRIPTION(Building* b) {
	return
		b->getDescription() + L"\n" +
		L"Ñòîèìîñòü: " + b->getCost().getReadableInfo();
}
Player* MainScreen::getCurrentPlayer() {
	return &this->players[(move - 1) % 2];
}
void MainScreen::handleGameObjectClick() {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view->getCenter().x - this->windowW / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view->getCenter().y - this->windowH / 2;
	for (uint32_t i = 0; i < this->gameObjects->size(); i = i + 1) {
		this->handleEvent(this->gameObjects->at(i)->click(*this->getCurrentPlayer(), mouseX, mouseY));
	}
}
void MainScreen::addPopUpWindows(std::queue<PopUpElement*> q) {
	while (!q.empty()) {
		this->addPopUpWindow(q.front());
		q.pop();
	}
}
void MainScreen::addPopUpWindow(PopUpElement* w) {
	this->elements.push(w);
	if (this->elements.size() == 1) {
		this->handleEvent(w->run(this->windowW, this->windowH));
	}
}
void MainScreen::prepareToReturnToMenu(sf::RenderWindow &window) {
	window.setView(window.getDefaultView());
	Playlist::get()->restartMusic();
}
void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(*this->view);
	this->drawCells(window);
	for (uint32_t i = 0; i < this->gameObjects->size(); i = i + 1) {
		window.draw(*this->gameObjects->at(i));
	}
	if (!this->elements.empty()) {
		window.draw(*this->elements.front());
	}
	window.draw(*this->getCurrentPlayer()->getConstResourceBarPtr());
	window.draw(endMoveButton);
	window.draw(buildButton);
	window.display();
}
void MainScreen::drawCells(sf::RenderWindow &window) {
	for (uint32_t i = 0; i < this->plains.getW(); i = i + 1) {
		for (uint32_t j = 0; j < this->plains.getH(); j = j + 1) {
			sf::Sprite s;
			s.setTexture(*Textures::get()->get(std::to_string(this->plains.getType(i, j) + 1)));
			s.setPosition(32 * i, 32 * j);
			window.draw(s);
			if (this->highlightTable.highlighted(i, j)) {
				sf::RectangleShape r;
				r.setPosition(s.getPosition());
				r.setSize(sf::Vector2f(32, 32));
				r.setFillColor(CELL_COLOR_HIGHLIGHTED);
				window.draw(r);
			}
		}
	}
}
void MainScreen::moveView() {
	auto p = sf::Mouse::getPosition();
	if (p.x < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->moveViewToWest();
	}
	else if (p.x > this->windowW - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->moveViewToEast();
	}
	if (p.y < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->moveViewToNorth();
	}
	else if (p.y > this->windowH - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->moveViewToSouth();
	}
}
void MainScreen::updatePlayerViewPoint() {
	if (this->getCurrentPlayer()->getId() == 1) {
		this->view->setCenter(sf::Vector2f(this->windowW / 2, this->windowH / 2));
	}
	else {
		this->view->setCenter(sf::Vector2f(32 * this->plains.getW() - this->windowW / 2, 32 * this->plains.getH() - this->windowH / 2));
	}
}
void MainScreen::moveViewToNorth() {
	float d = std::min(10.f, this->view->getCenter().y - this->windowH / 2);
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(0, d));
}
void MainScreen::moveViewToSouth() {
	float d = std::min(10.f, 32 * this->plains.getH() - this->windowH / 2 - this->view->getCenter().y);
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(0, d));
}
void MainScreen::moveViewToWest() {
	float d = std::min(10.f, this->view->getCenter().x - this->windowW / 2);
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(d, 0));
}
void MainScreen::moveViewToEast() {
	float d = std::min(10.f, 32 * this->plains.getW() - this->windowW / 2 - this->view->getCenter().x);
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(d, 0));
}
void MainScreen::add(GO* go) {
	this->gameObjects->push_back(go);

	ResourcePoint* rp = dynamic_cast<ResourcePoint*>(go);
	if (rp) {
		this->resourcePoints->push_back(rp);
	}
	else {
		Unit* u = dynamic_cast<Unit*>(go);
		if (u) {
			this->units->push_back(u);

			TerritoryB* tb = dynamic_cast<TerritoryB*>(go);
			if (tb) {
				this->territoryBuildings->push_back(tb);

				TerritoryOriginB* tob = dynamic_cast<TerritoryOriginB*>(go);
				if (tob) {
					this->territoryOrigins->push_back(tob);
				}
				else {
					TerritoryConductorB* tcb = dynamic_cast<TerritoryConductorB*>(go);
					if (tcb) {
						this->territoryConductors->push_back(tcb);
					}
				}
			}
		}
	}
}