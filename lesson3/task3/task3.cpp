#include <iostream>
#include <fstream>

enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};
class LogMessage {
public:
    LogMessage(Type type, const std::string& message) : type_(type), message_(message) {}
    Type type() const {
        return type_;
    }
    const std::string& message() const {
        return message_;
    }
private:
    Type type_;
    std::string message_;
};

class UnknownMessageHandler {
public:
    void handleMessage(LogMessage& msg) {
        if (msg.type() == Type::Unknown) {
            throw std::runtime_error(msg.message());
        }
        else {
            throw std::runtime_error("Message not handled");
        }
    }
};

class WarningHandler {
public:
    WarningHandler(std::unique_ptr<UnknownMessageHandler> next) : next_(std::move(next)) {}
    void handleMessage(LogMessage& msg) {
        if (msg.type() == Type::Warning) {
            std::cout << msg.message() << std::endl;
        }
        else {
            next_->handleMessage(msg);
        }
    }
private:
    std::unique_ptr<UnknownMessageHandler> next_;
};

class ErrorHandler {
public:
    ErrorHandler(std::unique_ptr<WarningHandler> next, const std::string& filename) : next_(std::move(next)), filename_(filename) {}
    void handleMessage(LogMessage& msg) {
        if (msg.type() == Type::Error) {
            std::ofstream file(filename_, std::ios::app);
            if (file.is_open()) {
                file << msg.message() << std::endl;
                file.close();
            }
            else {
                throw std::runtime_error("File not found");
            }
        }
        else {
            next_->handleMessage(msg);
        }
    }
private:
    std::string filename_;
    std::unique_ptr<WarningHandler> next_;
};

class FatalErrorHandler  {
public:
    FatalErrorHandler(std::unique_ptr<ErrorHandler> next) : next_(std::move(next)) {}
    void handleMessage(LogMessage& msg) {
        if (msg.type() == Type::FatalError) {
            throw std::runtime_error(msg.message());
        }
        else {
            next_->handleMessage(msg);
        }
    }
private:
    std::unique_ptr<ErrorHandler> next_;
};


int main()
{
    auto unknownMsgHandler = std::make_unique<UnknownMessageHandler>();
    auto warningHandler = std::make_unique<WarningHandler>(std::move(unknownMsgHandler));
    auto errorHandler = std::make_unique<ErrorHandler>(std::move(warningHandler), "log.txt");
    auto fatalErrorHandler = std::make_unique<FatalErrorHandler>(std::move(errorHandler));

    try {
        //LogMessage msg (Type::FatalError, "Fatal error message");
        //LogMessage msg(Type::Error, "Error message");
        //LogMessage msg(Type::Warning, "Warning message");
        LogMessage msg(Type::Unknown, "Unknown error");
        fatalErrorHandler->handleMessage(msg);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
	return 0;
}
