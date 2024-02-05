#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

int main() {
    const int dateTimeLength = 14;
    const int N = 5;

    map<string, string> failureLogs;
    auto hasFailure = [&](string address) {
        return failureLogs[address].length() == dateTimeLength;
    };

    ifstream fin("Q2_in.txt");
    ofstream fout("Q2_out.txt");
    
    string logData;
    while (getline(fin, logData)) {
        string dateTime, IPaddress, ping;
        istringstream logDataStream(logData);
        getline(logDataStream, dateTime, ',');
        getline(logDataStream, IPaddress, ',');
        getline(logDataStream, ping, ',');

        if (ping == "-") {
            if (!failureLogs.count(IPaddress)) {
                failureLogs[IPaddress] = (N == 1 ? dateTime : "1");
            } else if (!hasFailure(IPaddress)) {
                int cntTimeout = stoi(failureLogs[IPaddress]) + 1;
                failureLogs[IPaddress] = (cntTimeout == N ? dateTime : to_string(cntTimeout));
            }
        } else if (failureLogs.count(IPaddress)) {
            if (hasFailure(IPaddress)) {
                fout << failureLogs[IPaddress] << ',' << dateTime << ',' << IPaddress << endl;
            }
            failureLogs.erase(IPaddress);
        }
    }
    for (auto p : failureLogs) {
        string IPaddress = p.first, dateTime = p.second;
        if (hasFailure(IPaddress)) fout << dateTime << ",-," << IPaddress << endl;
    }
}
