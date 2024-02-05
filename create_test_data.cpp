#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    const int numData = 201000;
    const int subnet1size = 5, subnet2size = 1, subnet3size = 5;
    const int numServers = subnet1size + subnet2size*subnet3size;
    const vector<int> monthEndDays = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    vector<string> dateTimes(numData);
    int year = 2023, month = 1, day = 10, hour = 12, minute = 34, second = 0;
    for (int i = 0; i < numData; i++) {
        second += 10;
        if (second >= 60) { second = 0; minute++; }
        if (minute >= 60) { minute = 0; hour++; }
        if (hour >= 24) { hour = 0; day++; }
        if (day > monthEndDays[month]) { day = 1; month++; }
        if (month > 12) { month = 1; year++; }

        ostringstream ss;
        ss << setw(4) << setfill('0') << year;
        ss << setw(2) << setfill('0') << month;
        ss << setw(2) << setfill('0') << day;
        ss << setw(2) << setfill('0') << hour;
        ss << setw(2) << setfill('0') << minute;
        ss << setw(2) << setfill('0') << second;
        dateTimes[i] = ss.str();
    }

    vector<string> IPaddresses(numServers);
    for (int i = 1; i <= subnet1size; i++) {
        ostringstream ss; ss << "10.20.30." << i << "/16";
        IPaddresses[i-1] = ss.str();
    }
    for (int i = 1; i <= subnet2size; i++) for (int j = 1; j <= subnet3size; j++) {
        ostringstream ss; ss << "192.168." << i << "." << j << "/24";
        int idx = subnet3size*(i-1) + j + subnet1size - 1;
        IPaddresses[idx] = ss.str();
    }

    ofstream fout("Q4_in.txt");
    random_device seed;
    default_random_engine engine(seed());
    uniform_int_distribution<> addressId(0, numServers-1);
    uniform_int_distribution<> pingTime(1, 999);
    uniform_real_distribution<> failureRnd(0.0, 1.0);
    for (int i = 0; i < numData; i++) {
        fout << dateTimes[i] << ',' << IPaddresses[addressId(engine)] << ',';
        if (failureRnd(engine) < 0.5) {
            fout << '-' << endl;
        } else {
            fout << pingTime(engine) << endl;
        }
    }
}
