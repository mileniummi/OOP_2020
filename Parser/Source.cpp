#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;
class ini_file
{
private:
	map<string, map<string, string>> info;
public:
	ini_file() = default;
	void add_section_variables(string section_name_,string param, string value) {
		auto res = info[section_name_].emplace(param, value);
		if (res.second == false) {
			throw(exception("Ini file incorrect, repeating sections"));
		}
	}
	void show() {
		for (auto cur = info.begin(); cur != info.end(); cur++)
		{	
			cout  << cur->first << "\n";
			for (auto cur_var = info[cur->first].begin(); cur_var != info[cur->first].end(); cur_var++)
			{
				cout << (*cur_var).first << ": ";
				cout << (*cur_var).second << "\n";
			}
		}
	}
	string get_value_string(string section_name_,string variable_) {
		regex string_("([\\w-./]+)");
		auto section_name = info[section_name_];
		auto it = section_name.find(variable_);
		if (it != section_name.end()) {
			return (it->second);
		}
		else
			throw(exception("No such section or variable"));
	}
	int get_value_int(string section_name_, string variable_) {
		string value = get_value_string(section_name_, variable_);
		return stoi(value.c_str());
	}
	double get_value_double(string section_name_, string variable_) {
		string value = get_value_string(section_name_, variable_);
		return stof(value.c_str());
	}
};
void file_init(string file_name, ini_file &dictionary) {
	regex ini_file("([\\w]+)""\\.""(ini)");
	//проверка формата файла
	if  (! regex_match(file_name, ini_file)) {
			throw exception("That is not .ini file");
	}
	ifstream fin(file_name);
	//проверка на ошибку файловой сиситемы
	if (fin.fail()) {
		throw exception("Fail to read ini file");
	}

	string str;
	cmatch result;
	string last_section = "-1";
	string str_value;
	vector<string> sections;
	//регул€рные выражени€ 
	regex section("(\\[)" "([\\w]+)""(\\])");
	regex parametr("[a-zA-Z]+");
	regex value("(=)""([\\s]*)""([\\w-./]+)");
	regex section_with_comment("(\\[)" "([\\w]+)""(\\])""(;)""(.+)");
	while (!fin.eof())
	{
		fin >> str;
		//провека если комментарий 
		if ((str == ";") || (str[0] == ';')) {
			getline(fin, str_value);
		}
		// проверка на секцию
		else if (regex_match(str.c_str(),result, section)) {
			str = result[2];
			last_section =str;
		}
		else if (regex_match(str.c_str(), result, section_with_comment)) {
			str = result[2];
			last_section = str;
			getline(fin, str_value);
		}
		// проверка на переменную со значением
		else if (regex_match(str.c_str(), parametr) && (last_section != "-1")) {
			getline(fin, str_value);
			if (regex_search(str_value.c_str(), result, value)) {
				str_value = result[3];
				dictionary.add_section_variables(last_section, str, str_value);
			}
			else
				throw(exception("Incorrect ini file"));
		}
		else
			throw(exception("Incorrect ini file"));

	}
}


int main() {	
	ini_file example_ini;
	try {
		file_init("file.ini", example_ini);
		example_ini.show();
		cout << ".....\n";
		cout << example_ini.get_value_string("COMMON", "DiskCachePath") << "\n";
		cout << example_ini.get_value_int("COMMON", "LogNCDM") << "\n";
		cout << example_ini.get_value_double("", "BufferLenSecons");
	} 
	catch(exception &ex) {
		cout << ex.what();	
	}
	return 0;
}