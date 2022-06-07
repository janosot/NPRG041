#include <string>
#include <vector>
#include <map>

class dictionary {
private:
    std::map<std::string, std::vector<std::string>> data;
public:
    int remove_all(const std::string& word);
    int add(const std::string& word, const std::string& translation);
    int remove(const std::string& word, const std::string& translation);
    std::vector<std::string> find(const std::string& word) const;
    bool has(const std::string& word) const;
    std::vector<std::string> pfind(const std::string& prefix) const;
    //Finds all translations that start with 'prefix'
    std::vector<std::string> pafind(const std::string& prefix) const;
    std::vector<std::string> rfind(const std::string& translation) const;
};
