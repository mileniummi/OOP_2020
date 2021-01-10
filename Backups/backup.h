#pragma once
#pragma once
#include"cleaner.h"

short get_point_size(vector<file_info> files_) {
	short size = 0;
	for (size_t i = 0; i < files_.size(); i++) {
		size += files_[i].get_size();
	}
	return size;
}


class backup {
private:
	int ID;
	short size = 0;
	time_t creation_time;
	vector<file_info> initial_point;
	map <int, restore_point> points;
	ICleanable* backup_cleaner;
	int points_ID = 0;
public:
	backup(const vector<file_info>& files_, time_t cur_time_, ICleanable* cleaner_) {
		creation_time = cur_time_;
		initial_point = files_;
		size += get_point_size(files_);
		backup_cleaner = cleaner_;
	}
	int add_full_restore_point(const vector<file_info>& files_, time_t cur_time_) {
		restore_point new_point = full_restore_point(files_, cur_time_);
		points_ID++;
		points.emplace(make_pair(points_ID, new_point));
		return points_ID;
	}
	int add_increment_restore_point(vector<file_info>& files_, restore_point initial_point_, time_t cur_time_) {
		restore_point new_point = increment_restore_point(files_, initial_point_, cur_time_);
		points_ID++;
		points.emplace(make_pair(points_ID, new_point));
		return points_ID;
	}
	restore_point get_restore_point(int ID_) {
		auto  cur = points.find(ID_);
		if (cur != points.end()) {
			return cur->second;
		}
		else
			throw exception("No such point");
	}
	void change_cleaner(ICleanable* new_cleaner_) {
		backup_cleaner = new_cleaner_;
	}
	void clean() {
		backup_cleaner->delete_points(points);
	}
	int get_points_number() {
		return points.size();
	}

};