#include "Settings.h"

SettingsContainer::SettingsContainer(): m_cfg(new (std::nothrow) libconfig::Config)
{
    if (m_cfg==nullptr)
        throw ("SettingsContainer can not allocate memmory");
}

SettingsContainer::~SettingsContainer()
{
    if (m_cfg) delete m_cfg;
}

void SettingsContainer::open(const char *fname)
{
    try {
        m_cfg->readFile(fname);
        _parse();
    }
    catch(const libconfig::FileIOException &e) {
        throw("libconfig++ I/O error while reading file");
    }
    catch(const libconfig::ParseException &e) {
        std::string err; err = "libconfig++ parse error at " + std::string(e.getFile()) + ":" + std::to_string(e.getLine())
                 + " - " + e.getError();
        throw (err.c_str());
    }
    catch(...) {
        throw ("libconfig++ unknown error");
    }
    delete m_cfg;
    m_cfg = nullptr;

}

const std::string &SettingsContainer::getValue(const std::string &paramName) const
{
    auto it = m_string_parameters.find(paramName);
    if (it == std::end(m_string_parameters)) {
        std::string err;
        err = "Settings: parameter ["+paramName+"] not found in config file";
        throw (err.c_str());
    }
    return it->second;
}

const std::vector<std::string> &SettingsContainer::getCommands(const std::string &actionName) const
{
    auto it = m_commands.find(actionName);
    if (it == std::end(m_commands)) {
        std::string err;
        err = "Settings: action group ["+actionName+"] not found in config file";
        throw (err.c_str());
    }
    return it->second;
}

void SettingsContainer::_parse()
{
    auto &settings = m_cfg->getRoot();
    for (int i = 0; i < settings.getLength(); i++) {
        if (settings[i].getType() == libconfig::Setting::TypeArray
                         ||settings[i].getType() == libconfig::Setting::TypeList) {
            auto &subSettings = settings[i];
            if (m_commands.find(settings[i].getName()) == std::end(m_commands))
                m_commands[settings[i].getName()];
            for (int j = 0; j < subSettings.getLength(); j++)
                m_commands[settings[i].getName()].push_back(subSettings[j].c_str());
        }
        else if (settings[i].getType() == libconfig::Setting::TypeString)
            m_string_parameters[settings[i].getName()] = settings[i].c_str();
        else if (settings[i].getType() == libconfig::Setting::TypeInt)
            m_string_parameters[settings[i].getName()] = std::to_string(static_cast<int>(settings[i]));
    }
    _paramSubstitution();
}

void SettingsContainer::_paramSubstitution()
{
    std::map<std::string, std::vector<std::string>>::iterator it = std::begin(m_commands); // why not auto? :) - just for fun.
    for (; it != std::end(m_commands); ++it) {
        for (size_t i = 0; i < it->second.size(); i++) {
            std::string &str = it->second[i];
            std::map<std::string, std::string>::iterator parametersIterator = std::begin(m_string_parameters);
            while(parametersIterator != std::end(m_string_parameters)) {
                size_t idx = str.find(parametersIterator->first);
                if (idx != std::string::npos) {
                    str.replace(idx, parametersIterator->first.length(), parametersIterator->second);
                }
                ++parametersIterator;
            }
        }
    }
}
