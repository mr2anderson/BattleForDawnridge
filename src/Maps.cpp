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


#include "Maps.hpp"
#include "Textures.hpp"
#include "GO.hpp"


const uint32_t Maps::THUMBNAIL_SIZE = 64;


void Maps::add(const std::string &name, const std::string &path) {
    this->paths[name] = path;
    this->generateThumbnail(name);
}
void Maps::load(const std::string& name, Map *dst) {
    auto it = this->paths.find(name);
    dst->load(it->second);
}
void Maps::generateThumbnail(const std::string& name) {
    Map map;
    this->load(name, &map);

    float sx = std::min((float)THUMBNAIL_SIZE / (float)map.getStatePtr()->getMapSizePtr()->getWidth(), (float)THUMBNAIL_SIZE / (float)map.getStatePtr()->getMapSizePtr()->getHeight());
    float sy = sx;
    float scaleX = sx / 64.f;
    float scaleY = scaleX;

    sf::RenderTexture renderTexture;
    renderTexture.create(THUMBNAIL_SIZE, THUMBNAIL_SIZE);

    for (uint32_t i = 0; i < map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
        std::shared_ptr<GO> go = map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
        float x = go->getX() * sx;
        float y = (go->getY() + 1) * sy;
        sf::Sprite sprite;
        sprite.setTextureRect(go->getTextureRect());
        sprite.setTexture(*Textures::get().get(go->getTextureName()));
        sprite.setPosition(x, THUMBNAIL_SIZE - y);
        sprite.setScale(scaleX, scaleY);
        renderTexture.draw(sprite);
    }

    Textures::get().add(name, renderTexture.getTexture());
}