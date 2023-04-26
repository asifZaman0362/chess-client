#include "prefs.hpp"

#include <string.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "logger.hpp"

namespace zifmann {
namespace chess {

Prefs* Prefs::m_instance;
const std::filesystem::path Prefs::filename = "prefs.dat";
const std::filesystem::path Prefs::basedir = "data";

std::tuple<std::string, std::string, std::string> split_twice(
    const std::string& line, char delim) {
    size_t first_len = 0, second_len = 0, last = 0;
    while (last < line.length()) {
        last++;
        if (line[last - 1] == delim) {
            if (!first_len)
                first_len = last;
            else if (!second_len)
                second_len = last;
        }
    }

    return std::make_tuple(line.substr(0, first_len - 1),
                           line.substr(first_len, second_len - first_len - 1),
                           line.substr(second_len, last - second_len));
}

Prefs::Prefs() {
    std::ifstream file(basedir / filename, std::ios::in);
    if (!file.is_open()) return;
    char line[1024];
    while (!file.eof()) {
        file.getline(line, 1024);
        if (strlen(line) == 0) continue;
        auto entry = split_twice(std::string(line), ':');
        std::string type = std::get<0>(entry);
        std::string key = std::get<1>(entry);
        std::string val = std::get<2>(entry);
        if (type.compare("int") == 0) {
            m_integers[key] = std::stoi(val);
        } else if (type.compare("string") == 0) {
            m_strings[key] = val;
        } else if (type.compare("float") == 0) {
            m_floats[key] = std::stof(val);
        } else if (type.compare("bool") == 0) {
            if (val.compare("1") == 0)
                m_booleans[key] = true;
            else if (val.compare("0") == 0)
                m_booleans[key] = false;
            else
                log_error("invalid value %s on boolean %s", val.c_str(),
                          key.c_str());
        } else {
            log_error("invalid type %s in preference entry %s", type.c_str(),
                      key.c_str());
        }
    }
}

Prefs* Prefs::GetInstance() {
    if (!m_instance) m_instance = new Prefs();
    return m_instance;
}

Prefs* Prefs::SetBool(const std::string& key, bool value) {
    m_booleans[key] = value;
    return this;
}

Prefs* Prefs::SetString(const std::string& key, const std::string& value) {
    m_strings[key] = value;
    return this;
}

Prefs* Prefs::SetInt(const std::string& key, int value) {
    m_integers[key] = value;
    return this;
}

Prefs* Prefs::SetFloat(const std::string& key, float value) {
    m_floats[key] = value;
    return this;
}

bool Prefs::GetBool(const std::string& key) { return m_booleans[key]; }

std::string Prefs::GetString(const std::string& key) { return m_strings[key]; }

int Prefs::GetInt(const std::string& key) { return m_integers[key]; }

float Prefs::GetFloat(const std::string& key) { return m_floats[key]; }

bool Prefs::Save() {
    if (!std::filesystem::is_directory(basedir))
        std::filesystem::create_directory(basedir);
    std::ofstream outfile(basedir / filename, std::ios::out);
    if (!outfile.is_open()) {
        log_error("Failed to write preferences! Filepath invalid: %s",
                  (basedir / filename).string().c_str());
        return false;
    }
    for (auto kv : m_booleans)
        outfile << "bool:" << kv.first << ":" << kv.second << "\n";
    for (auto kv : m_strings)
        outfile << "string:" << kv.first << ":" << kv.second << "\n";
    for (auto kv : m_integers)
        outfile << "int:" << kv.first << ":" << kv.second << "\n";
    for (auto kv : m_floats)
        outfile << "float:" << kv.first << ":" << kv.second << "\n";
    return true;
}

}  // namespace chess
}  // namespace zifmann
