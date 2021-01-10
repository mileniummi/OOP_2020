#pragma once
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<chrono>
#include<fstream>
#include<map>

enum point_type {
	full_point,
	increment_point
};

using  namespace std;

long GetFileSize(std::string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}


class file_info {
protected:
	short size;
	string name;
	struct stat creation_time;
public:
	file_info(string path_) {
		ifstream fin(path_);
		if (!fin.is_open()) {
			throw exception("Unable to open file");
		}
		fin.close();
		size = GetFileSize(path_);
		name = path_;
		stat(path_.c_str(), &creation_time);
	}
	string get_name() {
		return name;
	}
	short get_size() {
		return size;
	}
	int get_dateTime_diff(string other_) {
		struct stat other_stat;
		stat(other_.c_str(), &other_stat);
		return creation_time.st_size - other_stat.st_size;
	}
};

class restore_point {
protected:
	time_t creation_time = 0;
	vector<file_info> restoring_files;
	short size = 0;
	restore_point* prev;
public:
	vector<file_info> get_restoring_files() {
		vector<file_info> res;
		res = restoring_files;
		return res;
	}
	time_t get_creation_time() {
		return creation_time;
	}
	restore_point* get_dependency() {
		return prev;
	}
	restore_point* get_full_point() {
		auto cur = prev;
		while (cur != nullptr) {
			cur = cur->get_dependency();
		}
		return cur;
	}
	short get_size() {
		return size;
	}
};
class full_restore_point :public restore_point {
public:
	full_restore_point(const vector<file_info>& files_, time_t creation_time_) {
		creation_time = creation_time_;
		restoring_files = files_;
		short size_ = 0;
		for (size_t i = 0; i < restoring_files.size(); i++) {
			size += restoring_files[i].get_size();
		}
		size = size_;
		prev = nullptr;
	}

};


class increment_restore_point :public restore_point {
private:
public:
	increment_restore_point(vector<file_info>& files_, restore_point initial_point_, time_t creation_time_) {
		creation_time = creation_time_;
		auto initial_files = initial_point_.get_restoring_files();
		for (size_t i = 0; i < files_.size(); i++) {
			for (size_t j = 0; j < initial_files.size(); j++)
			{
				if (files_[i].get_name() == initial_files[j].get_name()) {
					if (files_[i].get_dateTime_diff(initial_files[j].get_name()) != 0) {
						restoring_files.push_back(files_[i]);
					}
				}
				else {
					restoring_files.push_back(files_[i]);
				}
			}
		}
		short size_ = 0;
		for (size_t i = 0; i < restoring_files.size(); i++) {
			size += restoring_files[i].get_size();
		}
		size = size_;
		prev = &initial_point_;
	}
};