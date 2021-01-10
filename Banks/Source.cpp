#include "backup.h"
enum time_in_seconds {
	day = 86400,
	month = 2629743,
	year = 31536000
};

int main() {
	try {
		time_t cur_time_t = time(NULL);
		char cur_time[26];
		ctime_s(cur_time, sizeof(cur_time), &cur_time_t);
		cout << cur_time << "\n";
		// КеЙс 1

		//Я создаю беĸап, в ĸоторый добавляю 2 файла.
		vector<file_info> ini = { file_info("case1"),file_info("case1.1") };
		backup case1(ini, time(NULL), new count_cleaner(1));

		// запусĸаю алгоритм создания точĸи для этого беĸапа —
		//создается точĸа восстановления.

		vector<file_info> restore_point1 = { file_info("case1.2"), file_info("case1.3") };
		int point1 = case1.add_full_restore_point(restore_point1, time(NULL));

		//Я должен убедиться, что в этой точĸе лежит информация по
		//	двум файлам.
		cout << case1.get_restore_point(point1).get_restoring_files().size() << "\n";

		//Я создаю следующую точĸу восстановления для цепочĸи
		vector<file_info> restore_point2 = { file_info("case1.4") };
		int point2 = case1.add_full_restore_point(restore_point2, time(NULL));

		//Я применяю алгоритм очистĸи цепочĸи по принципу
		//ограничения маĸсимального ĸоличества
		case1.clean();

		//Я убеждаюсь, что в ответ получу цепочĸу длиной 1.
		cout << case1.get_points_number() << "\n";

		//КеЙс №2 
		//Я создаю беĸап день назад, в ĸоторый добавляю 2 файла 
		vector<file_info> case_2_files{ file_info("file.txt"), file_info("file1.txt") };
		backup case2(case_2_files, time(NULL) - time_in_seconds::day,
			new time_cleaner(time(NULL) - time_in_seconds::day / 2));
		// добавляю точку с текущей датой
		vector<file_info> point2_files{ file_info("file.txt"),file_info("file2.txt") };
		int point3 = case2.add_full_restore_point(point2_files, time(NULL));

		//Я убеждаюсь, что в ,бэкапе 1 точка
		cout << case2.get_points_number() << "\n";

		//я применяю алгоритм очистки с датой в пол дня назад
		case2.clean();

		//Я убеждаюсь, что в ,бэкапе не осталось точек
		cout << case2.get_points_number() << "\n";
	}
	catch (exception& ex) {
		cout << ex.what();
	}

}