#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<map>

enum environment
{
	land,
	air,
	all
};

using namespace std;
//������� ����� ������������ �������
class vehicle {
private:
	string name;
protected:
	int speed;
public:
	vehicle(string name_, int speed_);
	string get_vehicle_name();
	virtual environment get_type() = 0;
	virtual double calc_race_time(int distance_);
};
//����� �������� ������������ �������
class land_vehicle :public vehicle {
protected:
	environment type;
public:
	land_vehicle(string name_, int speed_);
	environment get_type()override;
};
//����� ��������� ������������ �������
class air_vehicle :public vehicle {
protected:
	environment type;
public:
	air_vehicle(string name_, int speed_);
	environment get_type()override;
};
//��������� �������
class bactrian_camel :public land_vehicle {
	int rest_interval;
public:
	bactrian_camel();
	double calc_race_time(int distance_)override;
	double calc_rest_duration(size_t rest_number_);
};
//�������-���������
class fast_camel :public land_vehicle {
	int rest_interval;
public:
	fast_camel();
	double calc_race_time(int distance_)override;
	double calc_rest_duration(size_t rest_number_);
};
//�������
class centaur :public land_vehicle {
	int rest_interval;
	double rest_duration = 2;
public:
	centaur();
	double calc_race_time(int distance_)override;

};
//������� ���������
class allterrain_boots :public land_vehicle {
	int rest_interval;
public:
	allterrain_boots();
	double calc_race_time(int distance_)override;
	double calc_rest_duration(size_t rest_number_);
};
//�����-�������
class magic_carpet :public air_vehicle {
	int distance_reducer;
public:
	magic_carpet();
	double calc_race_time(int distance_)override;
	void calc_distance_reducer(size_t distance_);
};
//�����
class stupa :public air_vehicle {
	int distance_reducer = 6;
public:
	stupa();
	double calc_race_time(int distance_)override;
};
//�����
class broom :public air_vehicle {
public:
	broom();
	double calc_race_time(int distance_)override;
};
//�����
class race {
private:
	vector<vehicle*> participants;
	environment  race_type;
	multimap<double, string>results;
	int distance;
public:
	race(environment race_type_, int distance_);
	void add_participant(vehicle* paricipant_);
	vector<string> start_race();
	environment get_race_type();
};
//�������� �����
class racing_simulator {
private:
	map<int,race> races;
	int race_ID = 0;
public:
	// task1 ������� �����
	int create_race(environment race_type_, int distance_);
	// task2 ���������������� ����������(������������ ��������)
	void register_vehicles(int race_ID_, const vector<vehicle*>& vehicles_);
	//task3 ��������� ����� � ����� ����������
	vector<string> find_winner(int race_ID_);
};