#pragma once

class Command
{
    std::string m_command;
public:
    Command(const std::string &command_string = std::string()): m_command(command_string) { }
    void run() const noexcept
    {
        system(m_command.c_str());
    }

    std::string run_read_output() const
    {
        std::string out;
        char _buff[255];

        auto pipe = popen(m_command.c_str(), "r");
        if (nullptr == pipe)
            throw ("error in Command::run_read_output(): in popen()");

        while (fgets(_buff, sizeof(_buff)/sizeof(_buff[0]), pipe) != nullptr)
            out.append(_buff);

        pclose(pipe);
        return out;
    }

    Command& operator=(const std::string &command_str) noexcept
    {
        m_command = command_str;
        return *this;
    }

    const std::string &to_string() const noexcept
    {
        return m_command;
    }
};
