#include "Vehicles.h"

vehicle::vehicle(string name_, int speed_)
	:name(name_)
	, speed(speed_)
{
}

string vehicle::get_vehicle_name()
{
	return name;
}

double vehicle::calc_race_time(int distance_)
{
	return (double)distance_ / speed;
}

land_vehicle::land_vehicle(string name_, int speed_)	
	:vehicle(name_, speed_)
{
	type = environment::land;
}

environment land_vehicle::get_type()
{
	return type;
}

air_vehicle::air_vehicle(string name_, int speed_)
	:vehicle(name_, speed_)
{
	type = environment::air;
}

environment air_vehicle::get_type()
{
	return type;
}

bactrian_camel::bactrian_camel()
	:land_vehicle("двугорбый верблюд", 10)
{
	rest_interval = 30;
}

double bactrian_camel::calc_race_time(int distance_)
{
	double time = (double)distance_ / speed;
	double total_time = time;
	for (int rest_number = time / rest_interval; rest_number > 0; rest_number--) {
		total_time += calc_rest_duration(rest_number);
	}
	return total_time;
}

double bactrian_camel::calc_rest_duration(size_t rest_number_)
{
	if (rest_number_ == 1)
		return 5;
	else
		return 8;
}

fast_camel::fast_camel()
	:land_vehicle("верблюд-быстроход", 40)
{
	rest_interval = 10;
}

double fast_camel::calc_race_time(int distance_)
{
	double time = (double)distance_ / speed;
	double total_time = time;
	for (int rest_number = time / rest_interval; rest_number > 0; rest_number--) {
		total_time += calc_rest_duration(rest_number);
	}
	return total_time;
}

double fast_camel::calc_rest_duration(size_t rest_number_)
{
	if (rest_number_ == 1)
		return 5;
	if (rest_number_ == 2)
		return 6.5;
	else
		return 8;
}

centaur::centaur()
	:land_vehicle("кентавр", 15)
{
	rest_interval = 8;
}

double centaur::calc_race_time(int distance_)
{
	double time = (double)distance_ / speed;
	double total_time = time;
	for (int rest_number = time / rest_interval; rest_number > 0; rest_number--) {
		total_time += 2;
	}
	return total_time;
}

allterrain_boots::allterrain_boots()
	:land_vehicle("ботинки-вездеходы", 6)
{
	rest_interval = 60;
}

double allterrain_boots::calc_race_time(int distance_)
{
	double time = (double)distance_ / speed;
	double total_time = time;
	for (int rest_number = time / rest_interval; rest_number > 0; rest_number--) {
		total_time += calc_rest_duration(rest_number);
	}
	return total_time;
}

double allterrain_boots::calc_rest_duration(size_t rest_number_)
{
	if (rest_number_ == 1)
		return 10;
	else
		return  5;
}

magic_carpet::magic_carpet()
	:air_vehicle("ковер-самолет", 10)
{
}

double magic_carpet::calc_race_time(int distance_)
{
	calc_distance_reducer(distance_);
	double reduced_distance = (double)distance_reducer * distance_ / 100;
	return ((double)(distance_ - reduced_distance) / speed);
}

void magic_carpet::calc_distance_reducer(size_t distance_)
{
	if (distance_ < 1000)
		distance_reducer = 0;
	else if (distance_ < 5000)
		distance_reducer = 3;
	else if (distance_ < 1000)
		distance_reducer = 10;
	else
		distance_reducer = 5;
}

stupa::stupa()
	:air_vehicle("ступа", 8)
{
	
}

double stupa::calc_race_time(int distance_)
{
	double reduced_distance = (double)distance_reducer * distance_ / 100;
	return ((double)(distance_ - reduced_distance) / speed);
}

broom::broom()
	:air_vehicle("метла", 20)
{
}

double broom::calc_race_time(int distance_)
{ 
	int dist = 1000;
	double distance = distance_;
	while (dist <= distance_) {
		distance -= ((double)distance_ / 100);
		dist+=1000;
	}
	return (double)(distance/ speed);
}

race::race(environment race_type_, int distance_)
{
	race_type = race_type_;
	distance = distance_;
}

void race::add_participant(vehicle* paricipant_)
{
	participants.push_back(paricipant_);
}

vector<string> race::start_race()
{
	for (size_t i = 0; i < participants.size(); i++)
	{
		results.emplace(participants[i]->calc_race_time(distance), participants[i]->get_vehicle_name());
	}
	for (auto cur = results.begin(); cur != results.end(); cur++) {
		cout << cur->first << " " << cur->second << "\n";
	}
	vector<string> winners;
	auto res = results.begin();
	if (!results.empty()) {
		auto cur = results.begin();
		while (cur != results.end()) {
			if (cur->first == results.begin()->first)
				winners.push_back(cur->second);
			cur++;
		}
	}
	else
		throw (exception("There is no such product in any of the shops"));
	return winners;
}

environment race::get_race_type()
{
	return race_type;
}

int racing_simulator::create_race(environment race_type_, int distance_)
{
	race_ID++;
	race race_(race_type_, distance_);
	races.emplace(race_ID,race_);
	return race_ID;
}

void racing_simulator::register_vehicles(int race_ID_, const vector<vehicle*>& vehicles_)
{
	auto cur_race = races.find(race_ID_);
	if (cur_race != races.end()) {
		for (size_t i = 0; i < vehicles_.size(); i++)
		{
			if ((cur_race->second.get_race_type() == vehicles_[i]->get_type()
				|| cur_race->second.get_race_type() == environment::all)) {
				cur_race->second.add_participant(vehicles_[i]);
			}
			else
				throw(exception(" ласс транспортного средства не соответствует допустимым классам в гонке"));
		}
	}
	else {
		throw(exception("“акого транспортного средства в гонке нет"));
	}

}

vector<string> racing_simulator::find_winner(int race_ID_)
{
	auto cur = races.find(race_ID_);
	if (cur != races.end()) {
		return cur->second.start_race();
	}
	else
		throw exception("Ќе существует конки с таким ID");
}
