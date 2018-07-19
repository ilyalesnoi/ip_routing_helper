#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include "CommandList.h"
#include "Settings.h"

/**
 * @brief make_find_template - определяет шаблоны поиска строк соответствующих созданным правилам (т.к. в системе могут быть не только наши правила)
 * @param patternSetupRouting - имя ключа для поиска
 * @param patternDelimeters - возможные разделители между ключом и его значением, значение ключа берется между его "именем" и разделителями
 * @param patternToSearchInCreatedToute - префикс для создания шаблона
 * @param command - строка по которой ведётся поиск
 * @return
 */
std::string make_find_template(const std::string &patternSetupRouting,
                               const std::string &patternDelimeters,
                               const std::string &patternToSearchInCreatedToute,
                               const std::string &command)
{
    auto skip_delimeters = [ ] (const std::string &str, const std::string &delim, size_t index)
    {
        for (size_t i = index; i < str.size(); i++) {
            bool no_delim = true;
            for (size_t j = 0; j < delim.size(); j++)
                if (str[i] == delim[j]) {
                    no_delim = false;
                    continue;
                }
            if (no_delim)
                return i;
        }
        return str.size();
    };

    size_t index = command.find(patternSetupRouting);

    if (std::string::npos == index)
        throw ("error in function make_find_template(): no correct 'pattern_by_setup_routing'");
    index += patternSetupRouting.size();
    index = skip_delimeters(command, patternDelimeters, index);
    return patternToSearchInCreatedToute + command.substr(index, command.size());
}

/**
 * @brief create_delete_command - модифицирует команду удалеения - добавлением подходящего индекса
 * @param strings - строки из которых нужно найти нужную
 * @param command - команда для удаления
 * @param pattern - паттерн поиска
 * @return
 */
std::string make_delete_command(const std::string &strings, const std::string &command, const std::string &pattern)
{
    std::istringstream sstream(strings);
    std::string buff;
    while(std::getline(sstream, buff, '\n')) {
        if (buff.find(pattern) != std::string::npos) {
            size_t delim_index = buff.find(' ');

            if (std::string::npos == delim_index)
                delim_index = buff.find('\t');

            if (std::string::npos == delim_index)
                throw ("error in function create_delete_command(): delete command is not created err01");

            buff.assign(buff.substr(0,delim_index));
            buff.insert(0, 1, ' ');
            buff.insert(0, command);
            return buff;
        }
    }
    throw ("error in function create_delete_command(): delete command is not created err02");
}

int main ( int argc, char * argv[] )
{
    try {
        SettingsContainer settings;
        settings.open("ip_routing_helper.conf");

        Command detectRemoteHost;
        CommandList enableRouting, detectRouting, disableRouting;

        detectRemoteHost = settings.getValue("DETECT_COM");
        enableRouting.assign(settings.getCommands("ip_routing_enable_commands"));
        detectRouting.assign(settings.getCommands("ip_routing_delete_prepare_commands"));
        disableRouting.assign(settings.getCommands("ip_routing_disable_commands"));


        std::string preroute_find_pattern = make_find_template("--to-destination", "\t ", "to:", enableRouting[1].to_string());
        std::string postroute_find_pattern = make_find_template("--to-source","\t ","to:", enableRouting[2].to_string());

        auto checkRouteEnable = [&](){
            return (detectRouting.front().run_read_output().find("DNAT") != std::string::npos
                    ||detectRouting.front().run_read_output().find("SNAT") != std::string::npos
                    ||detectRouting.back().run_read_output().find("DNAT") != std::string::npos
                    ||detectRouting.back().run_read_output().find("SNAT") != std::string::npos);
        };

        if(checkRouteEnable()) {
                // случай запуска, но уже готовой маршрутиризации, определяем индексы нужных нам записей
                disableRouting.assign(settings.getCommands("ip_routing_disable_commands"));
                disableRouting.reset_by_index(make_delete_command(detectRouting.front().run_read_output(), disableRouting[1].to_string(), preroute_find_pattern), 1);
                disableRouting.reset_by_index(make_delete_command(detectRouting.back().run_read_output(),  disableRouting[2].to_string(), postroute_find_pattern), 2);
        }

        while(true) {
            bool remoteHostIsAvaible =
                    std::string::npos != detectRemoteHost.run_read_output().find(settings.getValue("DETECT_COM_SUCCESS_PATTERN"));

            if (remoteHostIsAvaible && !checkRouteEnable()) {
                std::cout << "\tavaible remote host, IP_FORWARD ENABLE (remote host online) with commands:\n" << enableRouting<< '\n';
                enableRouting.run();
                // если роутинга не было, не было и записей - теперь генерируем по записям строки удаления
                disableRouting.assign(settings.getCommands("ip_routing_disable_commands"));
                disableRouting.reset_by_index(make_delete_command(detectRouting.front().run_read_output(), disableRouting[1].to_string(), preroute_find_pattern), 1);
                disableRouting.reset_by_index(make_delete_command(detectRouting.back().run_read_output(),  disableRouting[2].to_string(), postroute_find_pattern), 2);
            }
            if (!remoteHostIsAvaible && checkRouteEnable()) {
                std::cout << "\tIP_FORWARD DISABLE (remote host offline) with commands:\n" << disableRouting << '\n';
                disableRouting.run();
            }
            std::this_thread::sleep_for (std::chrono::seconds(std::stoi(settings.getValue("DETECT_TIMEOUT"))));
        }

    }
    catch(const char *e) {
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
