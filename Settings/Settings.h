#pragma once
#include <libconfig.h++>
#include <string>
#include <map>
#include <vector>

class SettingsContainer
{
    libconfig::Config *m_cfg;
    std::map<std::string, std::string> m_string_parameters;
    std::map<std::string, std::vector<std::string>> m_commands;
public:
    SettingsContainer();
    ~SettingsContainer();

    void open(const char *fname);

    const std::string &getValue(const std::string &paramName) const;
    const std::vector<std::string> &getCommands(const std::string &actionName) const;

private:
    void _parse();
    void _paramSubstitution();
};
