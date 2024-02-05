#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

int main() {
    const int dateTimeLength = 14;
    const int N = 5, m = 10, t = 500;

    map<string, string> overloadLogs;
    map<string, pair<int, deque<int>>> pingTimes;
    
    ifstream fin("Q3_in.txt");
    ofstream fout("Q3_out.txt");

    string logData;
    while (getline(fin, logData)) {
        string dateTime, IPaddress, ping;
        istringstream logDataStream(logData);
        getline(logDataStream, dateTime, ',');
        getline(logDataStream, IPaddress, ',');
        getline(logDataStream, ping, ',');

        if (ping == "-") continue;

        int pingInt = stoi(ping);
        pingTimes[IPaddress].first += pingInt;
        pingTimes[IPaddress].second.push_front(pingInt);
        if (pingTimes[IPaddress].second.size() > m) {
            pingTimes[IPaddress].first -= pingTimes[IPaddress].second.back();
            pingTimes[IPaddress].second.pop_back();
        }
        if (pingTimes[IPaddress].second.size() == m) {
            double pingAvg = 1.0*pingTimes[IPaddress].first/m;
            if (pingAvg > t) {
                if (!overloadLogs.count(IPaddress)) overloadLogs[IPaddress] = dateTime;
            } else if (overloadLogs.count(IPaddress)) {
                fout << overloadLogs[IPaddress] << ',' << dateTime << ',' << IPaddress << endl;
                overloadLogs.erase(IPaddress);
            }
        }
    }
    for (auto p : overloadLogs) fout << p.second << ",-," << p.first << endl;
}
