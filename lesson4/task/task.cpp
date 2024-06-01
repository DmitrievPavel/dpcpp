#include <fstream>

class Data {
public:
	virtual ~Data() = default;
	virtual std::string print() const = 0;
};

class TextData : public Data {
private:
	std::string data_;
public:
	TextData(std::string data) : data_(std::move(data)) {}
	std::string print() const override {
		return data_;
	}
};

class HTMLData : public Data {
private:
	std::string data_;
public:
	HTMLData(std::string data) : data_(std::move(data)) {}
	std::string print() const override {
		return "<html>" + data_ + "<html/>";
	}
};

class JSONData : public Data {
private:
	std::string data_;
public:
	JSONData(std::string data) : data_(std::move(data)) {}
	std::string print() const override {
		return "{ \"data\": \"" + data_ + "\"}";
	}
};

void saveTo(std::ofstream& file, const Data& data) {
	file << data.print() << std::endl;
}

void saveToAsText(std::ofstream& file, const TextData& data) {
	saveTo(file, data);
}

void saveToAsHTML(std::ofstream& file, const HTMLData& data) {
	saveTo(file, data);
}

void saveToAsJSON(std::ofstream& file, const JSONData& data) {
	saveTo(file, data);
}
int main()
{
	std::ofstream file("file.txt", std::ios::app);
	std::string data = "Data string";
	TextData textdata(data);
	HTMLData htmldata(data);
	JSONData jsondata(data);
	saveToAsText(file, textdata);
	saveToAsHTML(file, htmldata);
	saveToAsJSON(file, jsondata);
	return 0;
}
