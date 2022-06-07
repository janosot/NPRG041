#include <string>
#include <vector>
#include <map>

class dictionary 
{
private:
    std::map<std::string, std::vector<std::string>> data;
public:
    void remove_all(const std::string& word);
    void add(const std::string& word, const std::string& translation);
    void remove(const std::string& word, const std::string& translation);
    std::vector<std::string> find(const std::string& word);
    bool has(const std::string& word);
};
