#include <iostream>
#include <fstream>
#include <vector>

class Observer {
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class ErrorClass {
public:
    void warning(const std::string& message) const {
        for (auto obs : observers) {
            if (auto ptr = obs.lock()) {
                ptr->onWarning(message);
            }
        }
    }

    void error(const std::string& message) const{
        for (auto obs : observers) {
            if (auto ptr = obs.lock()) {
                ptr->onError(message);
            }
        }
    }

    void fatalError(const std::string& message) const{
        for (auto obs : observers) {
            if (auto ptr = obs.lock()) {
                ptr->onFatalError(message);
            }
        }
    }

    void addObserver(std::weak_ptr<Observer> obs) {
        observers.push_back(obs);
    }


private:
    std::vector<std::weak_ptr<Observer>> observers;
};

class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning observer: " << message << std::endl;
    }
    void onError(const std::string& message) override {}
    void onFatalError(const std::string& message) override {}
};

class ErrorObserver : public Observer {
public:
    ErrorObserver(const std::string& filename) : filename_(filename) {}

    void onError(const std::string& message) override {
        std::ofstream file(filename_, std::ios::app);
        if (file.is_open()) {
            file << "Error observer: " << message << std::endl;
            file.close();
        }
        else {
            std::cout << "File not found" << std::endl;
        }
    }
    void onWarning(const std::string& message) override {}
    void onFatalError(const std::string& message) override {}
private:
    std::string filename_;
};

class FatalErrorObserver : public Observer {
public:
    FatalErrorObserver(const std::string& filename) : filename_(filename) {}

    void onFatalError(const std::string& message) override {
        std::ofstream file(filename_, std::ios::app);
        if (file.is_open()) {
            std::cout << "Fatal error observer: " << message << std::endl;
            file << "Fatal error observer: " << message << std::endl;
            file.close();
        }
        else {
            std::cout << "File not found" << std::endl;
        }
    }
    void onWarning(const std::string& message) override {}
    void onError(const std::string& message) override {}
private:
    std::string filename_;
};

int main()
{
    ErrorClass errorclass;
    WarningObserver warObs;
    ErrorObserver errObs("log.txt");
    FatalErrorObserver fatObs("log.txt");
    auto warningObs = std::make_shared<WarningObserver>(warObs);
    auto errorObs = std::make_shared<ErrorObserver>(errObs);
    auto fatalErrorObs = std::make_shared<FatalErrorObserver>(fatObs);
    errorclass.addObserver(warningObs);
    errorclass.addObserver(errorObs);
    errorclass.addObserver(fatalErrorObs);
    errorclass.warning("warning message");
    errorclass.error("error message");
    errorclass.fatalError("fatal error message");
	return 0;
}
