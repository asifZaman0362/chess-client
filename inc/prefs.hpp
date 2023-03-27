#ifndef PREFS_HPP
#define PREFS_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

namespace zifmann {
namespace chess {

class Prefs {
   private:
    const static std::filesystem::path basedir;
    const static std::filesystem::path filename;
    static Prefs* m_instance;
    Prefs();

    std::unordered_map<std::string, bool> m_booleans;
    std::unordered_map<std::string, std::string> m_strings;
    std::unordered_map<std::string, int> m_integers;
    std::unordered_map<std::string, float> m_floats;

   public:
    static Prefs* GetInstance();
    Prefs* SetBool(const std::string& key, bool value);
    Prefs* SetInt(const std::string& key, int value);
    Prefs* SetFloat(const std::string& key, float value);
    Prefs* SetString(const std::string& key, const std::string& value);
    int GetInt(const std::string& key);
    bool GetBool(const std::string& key);
    float GetFloat(const std::string& key);
    std::string GetString(const std::string& key);
    bool Save();
};

}  // namespace chess
}  // namespace zifmann

#endif
