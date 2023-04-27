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
std::unordered_map<IncomingMessageType,
                   std::vector<std::function<void(IncomingMessage)>>>
    NetworkManager::m_callbacks;
std::unordered_map<IncomingMessageType,
                   std::vector<std::function<void(IncomingMessage)>>>
    NetworkManager::m_subscribers;

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

void NetworkManager::UpdateWrite() {
    if (writeBuffer == nullptr) {
        if (!outgoingQueue.empty()) {
            auto message = outgoingQueue.front().Serialize() + "\n";
            log_debug("sending " + message);
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
            log_error("youre disconnected dummy!");
        }
    }
}

void NetworkManager::UpdateRead() {
    char buffer[1024];
    auto status = m_socket.receive(buffer, 1024, readBytes);
    readBuffer += buffer;
    if (readBytes > 0) {
        log_info("read %u bytes", readBytes);
    }
    if (auto pos = readBuffer.find('\n'); pos != std::string::npos) {
        log_info(readBuffer);
        auto data = readBuffer.substr(0, pos);
        readBuffer = readBuffer.substr(pos + 1);
        log_info(data);
        auto deserRes = FromString<IncomingMessage>(data);
        if (deserRes.success) {
            auto message = deserRes.value;
            if (!m_callbacks[message.type].empty()) {
                for (auto callback : m_callbacks[message.type]) {
                    if (callback != nullptr) {
                        callback(message);
                    }
                }
                m_callbacks[message.type].clear();
            }
        } else {
            log_error("corrupted message received!");
        }
    }
}

void NetworkManager::AddCallback(IncomingMessageType type,
                                 std::function<void(IncomingMessage)> action,
                                 bool persist) {
    if (persist)
        m_subscribers[type].push_back(action);
    else
        m_callbacks[type].push_back(action);
}

}  // namespace network
}  // namespace chess
}  // namespace zifmann
