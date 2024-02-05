#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

int main() {
    map<string, string> failureLogs;
    ifstream fin("Q1_in.txt");
    ofstream fout("Q1_out.txt");
    
    string logData;
    while (getline(fin, logData)) {
        string dateTime, IPaddress, ping;
        istringstream logDataStream(logData);
        getline(logDataStream, dateTime, ',');
        getline(logDataStream, IPaddress, ',');
        getline(logDataStream, ping, ',');

        if (ping == "-") {
            if (!failureLogs.count(IPaddress)) failureLogs[IPaddress] = dateTime;
        } else {
            if (failureLogs.count(IPaddress)) {
                fout << failureLogs[IPaddress] << ',' << dateTime << ',' << IPaddress << endl;
                failureLogs.erase(IPaddress);
            }
        }
    }
    for (auto p : failureLogs) fout << p.second << ",-," << p.first << endl;
}
