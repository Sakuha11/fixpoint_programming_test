#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    const int dateTimeLength = 14;
    const int N = 3;

    auto subnet = [](string IPaddress) {
        istringstream addStream(IPaddress);
        long long addInt = 0LL;
        string tmp, prefixLen;
        int i = 3;
        while (getline(addStream, tmp, '.')) {
            if (i > 0) {
                addInt += (1LL*stoi(tmp))<<(i*8);
            } else {
                istringstream iss(tmp);
                string add;
                getline(iss, add, '/');
                getline(iss, prefixLen, '/');
                addInt += stoi(add);
            }
            i--;
        }

        int prefixLenInt = stoi(prefixLen);
        long long subnetMask = ((1LL<<prefixLenInt)-1)<<(32-prefixLenInt);
        long long subnetInt = addInt & subnetMask;
        vector<int> subnetAdd(4);
        long long eightBit = (1LL<<8) - 1;
        for (int i = 3; i >= 0; i--) {
            subnetAdd[i] = subnetInt & eightBit;
            subnetInt >>= 8;
        }

        ostringstream ret;
        for (int i = 0; i < 4; i++) {
            ret << subnetAdd[i];
            if (i < 3) ret << ".";
        }
        return ret.str();
    };
    auto strAddInt = [](string& s, int n) { return s = to_string(stoi(s) + n); };

    ifstream fin1("Q4_in.txt");
    map<string, set<string>> subnetList;
    string logData;
    while (getline(fin1, logData)) {
        string dateTime, IPaddress, ping;
        istringstream logDataStream(logData);
        getline(logDataStream, dateTime, ',');
        getline(logDataStream, IPaddress, ',');
        getline(logDataStream, ping, ',');

        subnetList[subnet(IPaddress)].insert(IPaddress);
    }

    ifstream fin2("Q4_in.txt");
    ofstream fout("Q4_out.txt");
    map<string, string> failureLogs;
    map<string, string> subnetFailures;
    for (auto p : subnetList) subnetFailures[p.first] = "0";
    auto hasFailure = [&](string address) {
        return failureLogs[address].length() == dateTimeLength;
    };
    auto subnetHasFailure = [&](string subnetAdd) {
        string failLog = subnetFailures[subnetAdd];
        return (failLog.length() == dateTimeLength) || (stoi(failLog) == subnetList[subnetAdd].size());
    };
    while (getline(fin2, logData)) {
        string dateTime, IPaddress, ping;
        istringstream logDataStream(logData);
        getline(logDataStream, dateTime, ',');
        getline(logDataStream, IPaddress, ',');
        getline(logDataStream, ping, ',');
        string subnetAdd = subnet(IPaddress);

        if (ping == "-") {
            if (!failureLogs.count(IPaddress)) {
                if (N == 1) {
                    failureLogs[IPaddress] = dateTime;
                    strAddInt(subnetFailures[subnetAdd], 1);
                    if (subnetHasFailure(subnetAdd)) subnetFailures[subnetAdd] = dateTime;
                } else {
                    failureLogs[IPaddress] = "1";
                }
            } else if (!hasFailure(IPaddress)) {
                int cntTimeout = stoi(failureLogs[IPaddress]) + 1;
                if (cntTimeout < N) {
                    failureLogs[IPaddress] = to_string(cntTimeout);
                } else {
                    failureLogs[IPaddress] = dateTime;
                    strAddInt(subnetFailures[subnetAdd], 1);
                    if (subnetHasFailure(subnetAdd)) subnetFailures[subnetAdd] = dateTime;
                }
            }
        } else if (failureLogs.count(IPaddress)) {
            if (hasFailure(IPaddress)) {
                if (subnetHasFailure(subnetAdd)) {
                    fout << subnetFailures[subnetAdd] << ',' << dateTime << ',' << subnetAdd << endl;
                    subnetFailures[subnetAdd] = to_string(subnetList[subnetAdd].size() - 1);
                } else {
                    strAddInt(subnetFailures[subnetAdd], -1);
                }
            }
            failureLogs.erase(IPaddress);
        }
    }
    for (auto p : subnetFailures) {
        string subnetAdd = p.first, dateTime = p.second;
        if (subnetHasFailure(subnetAdd)) fout << dateTime << ",-," << subnetAdd << endl;
    }
}
