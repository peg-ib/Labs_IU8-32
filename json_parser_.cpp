#include <iostream>
#include <any>
#include <map>
#include <vector>
#include <fstream>
#include <exception>
#include <string>
class JSON
{
private:
	std::map<std::string, std::any> object;
	std::vector<std::any> array;
public:
	JSON() = default;
	JSON(const std::string& s, int& position, bool is_file)
	{
		if (is_file == true)
		{
			parse_file(s);
		}
		else
		{
			parse(s, position);
		}
	}
	bool is_array()
	{
		if (array.empty())
			return false;
		else
			return true;
	}
	bool is_object()
	{
		if (object.empty())
			return false;
		else
			return true;
	}
	std::any& operator[](const std::string& key)
	{
		if (is_object() == true)
		{
			return object[key];
		}
		else
		{
			throw std::logic_error("is not an object");
		}
	}
	std::any& operator[](int index)
	{
		if (is_array() == true)
		{
			return array[index];
		}
		else
		{
			throw std::logic_error("is not an array");
		}
	}
	std::string parse_string(const std::string& s, int& position)
	{
		std::string str;
		position++;
		for (; position < s.size(); position++)
		{
			if (s[position] != '"')
			{
				str += s[position];
			}
			else
			{
				return str;
			}
		}
		throw std::logic_error("\nInvalid input format\n");
	}
	bool parse_bool(const std::string s, int& position)
	{
		bool result = true;
		bool is_bool = true;
		if (s[position] == 't')
		{
			position++;
			if (s[position] == 'r')
			{
				position++;
				if (s[position] == 'u')
				{
					position++;
					if (s[position] == 'e')
					{
						result = true;
					}
					else is_bool = false;
				}
				else is_bool = false;
			}
			else is_bool = false;
		}
		else
		{
			if (s[position] == 'f')
			{
				position++;
				if (s[position] == 'a')
				{
					position++;
					if (s[position] == 'l')
					{
						position++;
						if (s[position] == 's')
						{
							position++;
							if (s[position] == 'e')
							{
								result = false;
							}
							else is_bool = false;
						}
						else is_bool = false;
					}
					else is_bool = false;
				}
				else is_bool = false;
			}
			else is_bool = false;
		}
		if (is_bool)
		{
			return result;
		}
		else
			throw std::logic_error("\nInvalid input format\n");
	}
	double parse_number(const std::string s, int& position)
	{
		std::string number;
		number += s[position];
		position++;
		for (; position < s.size(); position++)
		{
			if (iswdigit(s[position]) || s[position] == '.')
			{
				number += s[position];
			}
			else
			{
				if (s[position] == ' ' || s[position] == ',' || s[position] == ']' || s[position] == '}') {
					position--;
					return stod(number);
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
		}
		throw std::logic_error("\nInvalid input format\n");
	}

	/*state_array =
	{
	 0 - Поиск начала ключа или окончания;
	 1 - Поиск двоеточия;
	 2 - Поиск значения;
	 3 - Поиск запятой или окончания;
	 }*/
	std::map<std::string, std::any> parse_object(const std::string& s, int& position)
	{
		std::map<std::string, std::any> result;
		std::string key;
		std::any value;
		int state_object = 0;
		position++;
		for (; position < s.size(); position++) {
			if (s[position] == '"')
			{
				if (state_object == 0)
				{
					key = parse_string(s, position);
					state_object = 1;
					continue;
				}
				if (state_object == 2)
				{
					value = parse_string(s, position);
					result.insert(std::pair<std::string, std::any>(key, value));
					state_object = 3;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == ':')
			{
				if (state_object == 1)
				{
					state_object = 2;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == '}')
			{
				if (state_object == 0 or state_object == 3)
				{
					return result;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == ',')
			{
				if (state_object == 3)
				{
					state_object = 0;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == '{')
			{
				if (state_object == 2)
				{
					JSON J(s, position, false);
					result.insert(std::pair<std::string, std::any>(key, J));
					state_object = 3;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == '[')
			{
				if (state_object == 2)
				{
					JSON J(s, position, false);
					result.insert(std::pair<std::string, std::any>(key, J));
					state_object = 3;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == 't' || s[position] == 'f')
			{
				if (state_object == 2)
				{
					value = parse_bool(s, position);
					result.insert(std::pair<std::string, std::any>(key, value));
					state_object = 3;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (iswdigit(s[position]))
			{
				if (state_object == 2)
				{
					value = parse_number(s, position);
					result.insert(std::pair<std::string, std::any>(key, value));
					state_object = 3;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
		}
		throw std::logic_error("\nInvalid input format\n");
	}
	/* state_array =
	{
	 0 - Поиск значения или окончания;
	 1 - Поиск запятой или окончания;
	 }*/
	std::vector<std::any> parse_array(const std::string& s, int& position)
	{
		std::vector<std::any> result;
		std::any value;
		int state_array = 0;
		position++;
		for (; position < s.size(); position++)
		{
			if (s[position] == '{')
			{
				if (state_array == 0)
				{
					JSON J(s, position, false);
					result.push_back(J);
					state_array = 1;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == '[')
			{
				if (state_array == 0)
				{
					JSON J(s, position, false);
					result.push_back(J);
					state_array = 1;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == '"')
			{
				if (state_array == 0)
				{
					value = parse_string(s, position);
					result.push_back(value);
					state_array = 1;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == ',')
			{
				if (state_array == 1)
				{
					state_array = 0;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (iswdigit(s[position]))
			{
				if (state_array == 0)
				{
					value = parse_number(s, position);
					result.push_back(value);
					state_array = 1;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == 't' || s[position] == 'f')
			{
				if (state_array == 0) {
					value = parse_bool(s, position);
					result.push_back(value);
					state_array = 1;
					continue;
				}
				else throw std::logic_error("\nInvalid input format\n");
			}
			if (s[position] == ']')
			{
				if (state_array == 1 or state_array == 0)
				{
					return result;
				}
			}
		}
		throw std::logic_error("\nInvalid input format\n");
	}
	JSON parse(const std::string& s, int& position)
	{
		for (; position < s.size(); position++)
		{
			if (s[position] == '{')
			{
				object = parse_object(s, position);
				break;
			}
			if (s[position] == '[')
			{
				array = parse_array(s, position);
				break;
			}
		}
		return *this;
	}
	JSON parse_file(const std::string& path_file)
	{
		std::ifstream JSON_file(path_file);
		std::string line, file;
		if (JSON_file.is_open())
		{
			while (!JSON_file.eof())
			{
				getline(JSON_file, line);
				file += line;
			}
			int position = 0;
			parse(file, position);
			JSON_file.close();
			return *this;
		}
		else throw std::logic_error("\nFile does not exist\n");
	}
};
int main()
{
	int p = 0;
	std::string a = "     {\"lastname\":\"Ivanov\",\"firstname\" : \"Ivan\",\"age\" : 25,\"islegal\" : false,\"marks\" : [ 4, 5, 5, 5, 2, 3] ,\"address\" : {\"city\" : \"Moscow\",\"street\" : \"Vozdvijenka\"}} ";
	std::string d = "C:\\Users\\PEG\\source\\repos\\JSON.txt";
	JSON J1;
	J1 = J1.parse(a, p);
	std::cout << std::any_cast<std::string>(std::any_cast<JSON>(J1["address"])["city"]) << std::endl;
	std::cout << std::any_cast<double>(std::any_cast<JSON>(J1["marks"])[2]) << std::endl;//Вывод 3 оценки
	return 0;
}