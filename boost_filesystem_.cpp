#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
struct finance
{
	std::string broker;
	std::string account;
	int date;
	int number;
	finance() = default;
	finance(std::string broker, std::string account, int date, int number)
	{
		this->broker = broker;
		this->account = account;
		this->date = date;
		this->number = number;
	}
	finance(boost::filesystem::path& p)
	{
		broker = p.parent_path().filename().string();//Директория 
		std::string number = "";
		std::string filename = p.filename().string();//Имя файла
		for (int i = 8; i < 16; i++)
			number += filename[i];
		account = number;
		number = " ";
		for (int i = 17; i < 25; i++)
			number += filename[i];
		date = atoi(number.c_str());
	}
};
//ib balance_00001234_20181001.txt
bool check_filename(boost::filesystem::path& p)
{
	std::string filename = p.filename().string();
	std::string type = "";
	if (filename.length() == 29)
	{
		for (int i = 0; i < 8; i++)
			type += filename[i];
		if (type == "balance_")
		{
			if (filename[16] == '_')
			{
				if (filename[21] <= '1')//Месяц 
				{
					if (filename[21] == '1' && filename[22] <= '2')//Месяц
					{
						if (filename[23] <= '3')//День
						{
							if (filename[23] == '3' && filename[24] > '1')//День
							{
								return false;
							}
							else
							{
								if (p.extension() == ".txt")
								{
									return true;
								}
								else
									return false;
							}
						}
						else
							return false;
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}
void search(const boost::filesystem::path& p, std::vector<finance>& files)
{
	boost::filesystem::path _path;
	if (exists(p))//Определяет,существует ли файл или директория по указанному пути
	{
		for (const boost::filesystem::directory_entry& x : boost::filesystem::directory_iterator{ p })
		{
			if (boost::filesystem::is_regular_file(x))
			{
				_path = x.path();
				if (check_filename(_path))
				{
					std::cout << _path.parent_path().filename() << ' ' << _path.filename() << std::endl;
					files.push_back(_path);
				}
			}
			else
			{
				if (boost::filesystem::is_directory(x))
				{
					_path = x.path();
					search(_path, files);
				}
			}
		}
	}
	else
	{
		std::cout << p << "does not exist" << std::endl;
	}
}
void print_finance(std::vector<finance> files)
{
	std::cout << std::endl;
	for (int i = 0; i < files.size(); i++)
	{
		std::cout << "broker:" << files[i].broker << ' ' << "account:" << files[i].account << ' ' << "files:" << files[i].number << ' ' << "lastdate:" << files[i].date << std::endl;
	}
}
void sorting(std::vector<finance>files)
{
	std::vector<finance> new_files;
	int number;
	double lastdate;
	std::string account = "";
	std::string broker = "";
	for (int i = 0; i < files.size(); i++)
	{
		if ((files[i].account != account) || (files[i].broker != broker))
		{
			number = 0;
			lastdate = files[i].date;
			for (int j = 0; j < files.size(); j++)
			{
				if (files[i].broker == files[j].broker)
				{
					if (files[i].account == files[j].account)
					{
						number++;
						if (files[j].date >= lastdate)
						{
							lastdate = files[j].date;
						}
					}
				}
			}
			finance fin(files[i].broker, files[i].account, lastdate, number);
			new_files.push_back(fin);
		}
		broker = files[i].broker;
		account = files[i].account;
	}
	print_finance(new_files);
}
int main()
{
	const boost::filesystem::path& p = "D:\\ftp";
	std::vector<finance> files;
	search(p, files);
	sorting(files);
	return 0;
}




