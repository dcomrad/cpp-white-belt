#include <iostream>
#include <string>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

class Date {
public:
	Date(int new_year, int new_month, int new_day) {
		if (new_month < 1 || new_month > 12) {
			throw out_of_range("Month value is invalid: " + to_string(new_month));
		}
		if (new_day < 1 || new_day > 31) {
			throw out_of_range("Day value is invalid: " + to_string(new_day));
		}

		year = new_year;
		month = new_month;
		day = new_day;
	}

  friend bool operator<(const Date& lhs, const Date& rhs) {
  	string lhsDate = to_string(lhs.year) + to_string(lhs.month) + to_string(lhs.day);
  	string rhsDate = to_string(rhs.year) + to_string(rhs.month) + to_string(rhs.day);
  	return stoi(lhsDate) < stoi(rhsDate);
  }

	friend ostream& operator<<(ostream& os, const Date& date) {
		return os << setw(4) << setfill('0') << date.year << '-'
							<< setw(2) << setfill('0') << date.month << '-'
							<< setw(2) << setfill('0') << date.day;
	}

private:
	int year, month, day;
};

Date ParseDate(const string& input) {
	bool is_ok = true;
	stringstream ss(input);

	int year;
	is_ok = is_ok && (ss >> year);
	is_ok = is_ok && (ss.peek() == '-');
	ss.ignore();

	int month;
	is_ok = is_ok && (ss >> month);
	is_ok = is_ok && (ss.peek() == '-');
	ss.ignore();

	int day;
	is_ok = is_ok && (ss >> day);
	is_ok = is_ok && ss.eof();

	if (!is_ok) {
		throw runtime_error("Wrong date format: " + input);
	}

	return {year, month, day};
}

//********************************************

class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		events[date].insert(event);
	}

	bool DeleteEvent(const Date& date, const string& event) {
		if (events.count(date) && events[date].count(event)) {
			events[date].erase(event);
			return true;
		} else {
			return false;
		}
	}

	int DeleteDate(const Date& date) {
		int result = 0;
		if (events.count(date)) {
			result = events[date].size();
			events.erase(date);
		}
		return result;
	}

	set<string> Find(const Date& date) const {
		if (events.count(date)) {
			return events.at(date);
		} else {
			return {};
		}
	}

	void Print() const {
		for (const auto& date : events) {
			for (const auto& event : date.second) {
				cout << date.first << ' ' << event << endl;
			}
		}
	}

private:
	map<Date, set<string>> events;
};

int main () {
	Database db;

	string request;
	while (getline(cin, request)) {
		if (request.empty()) {
			continue;
		}

		stringstream ss(request);
		string command;
		ss >> command;

		try {
			if (command == "Add") {
				string date, event;
				ss >> date >> event;
				db.AddEvent(ParseDate(date), event);
			} else if (command == "Del") {
				string date;
				ss >> date;
				if (ss.eof()) {
					//Delete all events for certain date
					int count = db.DeleteDate(ParseDate(date));
					cout << "Deleted " << count << " events" << endl;
				} else {
					//Delete certain event for certain date
					string event;
					ss >> event;
					if (db.DeleteEvent(ParseDate(date), event)) {
						cout << "Deleted successfully" << endl;
					} else {
						cout << "Event not found" << endl;
					}
				}
			} else if (command == "Find") {
				string date;
				ss >> date;
				set<string> events = db.Find(ParseDate(date));
				for (const auto& event : events) {
					cout << event << endl;
				}
			} else if (command == "Print") {
				db.Print();
			} else {
				cout << "Unknown command: " << request << endl;
				return 0;
			}
		} catch (exception& ex) {
			cout << ex.what() << endl;
			return 0;
		}
	}

	return 0;
}
