#include "networkclient.hpp"

#include "logger.hpp"
#include "message.hpp"

namespace zifmann {
namespace chess {
namespace network {

NetworkManager* NetworkManager::m_instance;

NetworkManager::NetworkManager() {}

NetworkManager* NetworkManager::GetInstance() {
    if (!m_instance) m_instance = new NetworkManager();
    return m_instance;
}

bool NetworkManager::Connect(const std::string& address, int port) {
    auto res = m_socket.connect(address, port);
    m_socket.setBlocking(false);
    return res == sf::Socket::Done;
}

void NetworkManager::SendMessage(const OutgoingMessage& message) {
    outgoingQueue.push_back(message);
}

void NetworkManager::UpdateWrite() {
    if (writeBuffer == nullptr) {
        if (!outgoingQueue.empty()) {
            auto message = this->outgoingQueue.front().Serialize() + "\n";
            outgoingQueue.pop_front();
            remainingWrite = message.length();
            writeBuffer = new char[remainingWrite];
            memcpy(writeBuffer, message.c_str(), remainingWrite);
        }
    }
    if (writeBuffer == nullptr) {
        return;
    } else {
        size_t writtenThisFrame = 0;
        auto status = m_socket.send(writeBuffer + writtenBytes, remainingWrite,
                                    writtenThisFrame);
        writtenBytes += writtenThisFrame;
        remainingWrite -= writtenBytes;
        if (status == sf::Socket::Done) {
            delete writeBuffer;
            writeBuffer = nullptr;
            writtenBytes = 0;
        } else if (status == sf::Socket::Error) {
            log_error("failed to send packet! something unexpected happened!");
        } else if (status == sf::Socket::Disconnected) {
            log_error("your disconnected dumbass!");
        }
    }
}

void NetworkManager::UpdateRead() {
    char buffer[1024];
    auto status = m_socket.receive(buffer, 1024, readBytes);
    readBuffer += buffer;
    if (auto pos = readBuffer.find('\n'); pos != std::string::npos) {
        // auto message = IncomingMessage::from();
    }
}

}  // namespace network
}  // namespace chess
}  // namespace zifmann
