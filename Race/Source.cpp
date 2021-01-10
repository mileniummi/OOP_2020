#include"Vehicles.h"

int main() {
	setlocale(LC_ALL, "Russian");
	try {
		vector<vehicle*> race_a = { new bactrian_camel(), new fast_camel(),new fast_camel(),
			new centaur(), new allterrain_boots() };
		vector<vehicle*> race_b = { new magic_carpet(),new stupa(), new broom() };
		vector<vehicle*> race_c = { new bactrian_camel(), new fast_camel(),
			new centaur(), new allterrain_boots(), new magic_carpet(),new stupa(), new broom() };
		racing_simulator a;	
		vector<string> ans;
		//наземная гонка

		int race_ID = a.create_race(environment::land, 100);
		a.register_vehicles(race_ID, race_a);
		ans = a.find_winner(race_ID);

		//вывод на экран результатов
		cout << "\n" <<"Winner!!!" << "\n";
		for (size_t i = 0; i < ans.size(); i++)
		{
			cout << ans[i] << " \n";
		}
		cout << "\n";
		//воздушная гонка

		race_ID = a.create_race(environment::air, 999);
		a.register_vehicles(race_ID, race_b);
		ans = a.find_winner(race_ID);

		//вывод на экран результатов
		cout << "\n" << "Winner!!!" << "\n";
		for (size_t i = 0; i < ans.size(); i++)
		{
			cout << ans[i] << " \n";
		}
		cout << "\n";
		//гонка везде
		race_ID = a.create_race(environment::all, 1000);
		a.register_vehicles(race_ID, race_c);
		ans = a.find_winner(race_ID);

		//вывод на экран результатов
		cout << "\n" << "Winner!!!" << "\n";
		for (size_t i = 0; i < ans.size(); i++)
		{
			cout << ans[i] << "\n";
		}
	}
	catch (exception & ex) {
		cout << ex.what();
	}
	return 0;
}