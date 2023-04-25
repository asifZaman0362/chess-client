#include "networkclient.hpp"

#include <string_view>

NetworkManager* NetworkManager::m_instance;

NetworkManager::NetworkManager() {}

NetworkManager* NetworkManager::GetInstance() {
    if (!m_instance) m_instance = new NetworkManager();
    return m_instance;
}

bool NetworkManager::Connect(const std::string& address, int port) {
    auto res = m_socket.connect(address, port);
    return res == sf::Socket::Done;
}
