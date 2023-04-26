#include "networkclient.hpp"

#include <cstring>
#include <utility>

#include "logger.hpp"
#include "message.hpp"

namespace zifmann {
namespace chess {
namespace network {

sf::TcpSocket NetworkManager::m_socket;
std::deque<OutgoingMessage> NetworkManager::outgoingQueue;
std::deque<IncomingMessage> NetworkManager::incomingQueue;

size_t NetworkManager::writtenBytes;
size_t NetworkManager::readBytes;
size_t NetworkManager::remainingWrite;
size_t NetworkManager::remainingRead;

char* NetworkManager::writeBuffer;

std::string NetworkManager::readBuffer;

bool NetworkManager::Connect(const std::string& address, int port) {
    auto res = m_socket.connect(address, port);
    m_socket.setBlocking(false);
    return res == sf::Socket::Done;
}

bool NetworkManager::SendMessage(const OutgoingMessage& message,
                                 bool blocking) {
    if (blocking) {
        auto serialized = message.Serialize() + "\n";
        size_t len = serialized.length();
        m_socket.setBlocking(true);
        if (m_socket.send(serialized.c_str(), len) != sf::Socket::Done) {
            m_socket.setBlocking(false);
            return false;
        } else {
            m_socket.setBlocking(false);
            return true;
        }
    } else {
        outgoingQueue.push_back(message);
        return true;
    }
}

std::pair<bool, IncomingMessage> NetworkManager::ReadMessage() {
    if (incomingQueue.empty())
        return std::make_pair(false, IncomingMessage());
    else {
        auto msg = incomingQueue.front();
        incomingQueue.pop_front();
        return std::make_pair(true, msg);
    }
}

void NetworkManager::UpdateWrite() {
    if (writeBuffer == nullptr) {
        if (!outgoingQueue.empty()) {
            auto message = outgoingQueue.front().Serialize() + "\n";
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
        log_info("written %u bytes", writtenThisFrame);
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
