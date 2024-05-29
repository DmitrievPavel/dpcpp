#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class FileCommand : public LogCommand {
public:
    FileCommand(const std::string& filename) {
        file.open(filename);
    }

    ~FileCommand() override {
        file.close();
    }

    void print(const std::string& message) override {
        if (file.is_open()) {
            file << message << std::endl;
        }
        else {
            std::cout << "File not found" << std::endl;
        }
    }
private:
    std::ofstream file;
};

void print(LogCommand& log) {
    log.print("log_message");
}

int main()
{
    ConsoleCommand com;
    FileCommand fcom("log.txt");
    print(com);
    print(fcom);
	return 0;
}
