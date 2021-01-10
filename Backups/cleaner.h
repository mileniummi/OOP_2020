#pragma once
#pragma once
#include "points.h"

enum priority {
	bigger,
	less
};

class ICleanable {
public:
	virtual int find_points_to_clear(map<int, restore_point>& points_) = 0;
	virtual void delete_points(map<int, restore_point>& points_) = 0;
};

class count_cleaner :public ICleanable {
private:
	int count;
public:
	count_cleaner(int count_) {
		count = count_;
	}
	int find_points_to_clear(map<int, restore_point>& points_)override {
		if (count < points_.size()) {
			return points_.size() - count;
		}
		return 0;
	}
	void delete_points(map<int, restore_point>& points_)override {
		vector<restore_point*> unremovable_point;
		bool is_unremovable = false;
		for (auto cur = points_.begin(); cur != points_.end(); cur++) {
			if (cur->first <= count) {
				if (cur->second.get_dependency() != nullptr) {
					unremovable_point.push_back(cur->second.get_full_point());
				}
			}
		}
		vector<int> points_to_delete;
		for (auto var : points_)
		{
			if (var.first > count) {
				is_unremovable = false;
				for (auto point : unremovable_point) {
					if (&(var.second) == point)
						is_unremovable = true;
				}
				if (!is_unremovable)
					points_to_delete.push_back(var.first);
			}
		}
		for (int var : points_to_delete) {
			points_.erase(var);
		}
	}
};

class time_cleaner :public ICleanable {
private:
	time_t deadline;
public:
	time_cleaner(time_t deadline_) {
		deadline = deadline_;
	}
	int find_points_to_clear(map<int, restore_point>& points_)override {
		int res = 0;
		for (auto cur = points_.begin(); cur != points_.end(); cur++) {
			if (cur->second.get_creation_time() > deadline)
				res++;
		}
		return res;
	}
	void delete_points(map<int, restore_point>& points_)override {
		vector<restore_point*> unremovable_point;
		bool is_unremovable = false;
		for (auto cur = points_.begin(); cur != points_.end(); cur++) {
			if (cur->second.get_creation_time() > deadline) {
				if (cur->second.get_dependency() != nullptr) {
					unremovable_point.push_back(cur->second.get_full_point());
				}
			}
		}
		vector<int> points_to_delete;
		for (auto cur = points_.begin(); cur != points_.end(); cur++) {
			if (cur->second.get_creation_time() > deadline) {
				is_unremovable = false;
				for (auto point : unremovable_point) {
					if (&(cur->second) == point)
						is_unremovable = true;
				}
				if (!is_unremovable)
					points_to_delete.push_back(cur->first);
			}
		}
		for (int var : points_to_delete) {
			points_.erase(var);
		}
	}
};

class size_cleaner :public ICleanable {
private:
	short size;
public:
	size_cleaner(short size_) {
		size = size_;
	}
	int find_points_to_clear(map<int, restore_point>& points_)override {
		int res = 0;
		short acceptable_size = 0;
		for (auto cur = points_.rend(); cur != points_.rbegin(); cur++) {
			short added_size = cur->second.get_size();
			if (size < (acceptable_size + added_size)) {
				res++;
			}
			else {
				acceptable_size += added_size;
			}
		}
		return res;
	}
	void delete_points(map<int, restore_point>& points_)override {
		vector<restore_point*> unremovable_point;
		bool is_unremovable = false;
		short acceptable_size = 0;
		for (auto cur = points_.rend(); cur != points_.rbegin(); cur++) {
			short added_size = cur->second.get_size();
			if (size < (acceptable_size + added_size)) {
				if (cur->second.get_dependency() != nullptr) {
					unremovable_point.push_back(cur->second.get_full_point());
				}
			}
		}
		vector<int> points_to_delete;
		for (auto cur = points_.begin(); cur != points_.end(); cur++) {
			short acceptable_size = 0;
			short added_size = cur->second.get_size();
			if (size < (acceptable_size + added_size)) {
				is_unremovable = false;
				for (auto point : unremovable_point) {
					if (&(cur->second) == point)
						is_unremovable = true;
				}
				if (!is_unremovable)
					points_to_delete.push_back(cur->first);
			}
		}
		for (int var : points_to_delete) {
			points_.erase(var);
		}
	}
};

class combined_cleaner :public  ICleanable {
private:
	ICleanable* f_cleaner;
	ICleanable* s_cleaner;
	priority type;
public:
	combined_cleaner(ICleanable* first_cleaner_, ICleanable* second_cleaner_, priority type_) {
		f_cleaner = first_cleaner_;
		s_cleaner = second_cleaner_;
		type = type_;
	}
	int find_points_to_clear(map<int, restore_point>& points_)override {
		int first_res = f_cleaner->find_points_to_clear(points_);
		int second_res = s_cleaner->find_points_to_clear(points_);
		if (type == priority::bigger) {
			if (first_res > second_res) {
				return first_res;
			}
			else {
				return second_res;
			}
		}
		if (type == priority::less) {
			if (first_res < second_res) {
				return first_res;
			}
			else {
				return second_res;
			}
		}
	}
	void delete_points(map<int, restore_point>& points_)override {
		int first_res = f_cleaner->find_points_to_clear(points_);
		int second_res = s_cleaner->find_points_to_clear(points_);
		if (type == priority::bigger) {
			if (first_res > second_res) {
				f_cleaner->delete_points(points_);
			}
			else {
				s_cleaner->delete_points(points_);
			}
		}
		if (type == priority::less) {
			if (first_res < second_res) {
				f_cleaner->delete_points(points_);
			}
			else {
				s_cleaner->delete_points(points_);
			}
		}
	}
};
