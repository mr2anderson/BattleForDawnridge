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


#include <SFML/Network.hpp>
#include <functional>
#include <queue>
#include <atomic>


#pragma once


namespace bfdlib {


    namespace tcp_help {


        template<typename T> class safe_queue {
        public:
            safe_queue() = default;

            void push(const T& value) {
                std::lock_guard<std::mutex> lock(this->mutex);
                this->data.push(value);
            }

            T pop() {
                std::lock_guard<std::mutex> lock(this->mutex);
                T value = this->data.front();
                this->data.pop();
                return value;
            }

            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // You can rely on this method ONLY if the pop method is not called by other threads
            bool empty() const {
                std::lock_guard<std::mutex> lock(this->mutex);
                return this->data.empty();
            }
        private:
            std::queue<T> data;
            mutable std::mutex mutex;
        };


        typedef safe_queue<sf::Packet> packet_queue;


        void process_sending(sf::TcpSocket *socket, packet_queue *q, const std::atomic<bool> *flag);
        void process_receiving(sf::TcpSocket *socket, packet_queue *q, const std::atomic<bool> *flag);


    }


}