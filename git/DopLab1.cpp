#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <set>
#include <map>
using namespace std;

int main(){
    ifstream file("textinput.txt");
    if (!file.is_open()){
        cout << "файл не открывается" << endl;
        return 1;
    }
    // первая строка
    int N;
    file >> N; //колво участников
    vector<string> names;
    for (int i = 0; i < N; i++){
        string name;
        file >> name; //имена участников
        names.push_back(name);
    }

    map<string, double> potratil; //сколько потратил
    map<string, double> potrebil; //сколько потребил

    for (string name : names){
        potratil[name] = 0;
        potrebil[name] = 0;
    }

    string line;
    getline(file, line);

    while (getline(file, line)){
        if (line.empty()) continue; // если строка пустая, идет дальше
        int dvoetoc = line.find(":"); // эта двоеточие
        string platil = line.substr(0, dvoetoc); // тот кто платит
        // убираю пробелы
        while (platil.front() == ' '){
            platil.erase(0,1);
        }
        while (platil.back() == ' '){
            platil.pop_back();
        }

        string ostat = line.substr(dvoetoc+1);
        double summ; // потраченная участником сумма 
        vector<string> iscluch; // участники не включенные в трату
        int slash = ostat.find('/');
        if (slash != -1){
            string summ_str = ostat.substr(0, slash);
            summ = stod(summ_str); 
            string iskl_str = ostat.substr(slash+1); // имя участника не включенных в трату

            stringstream ss(iskl_str);
            string imya;
            while (getline(ss, imya, ',')){
                while (imya.front() == ' ') imya.erase(0,1); // убираю пробелы
                while (imya.back() == ' ') imya.pop_back();
                iscluch.push_back(imya);
            }
        } else {
            summ = stod(ostat);
        }
        // кто сколько потратил
        potratil[platil] += summ;

        vector<string> uchastniki; // кто участвовал в трате
        for (string name : names){
            bool iscluchen = false;
            for (string iskl : iscluch){
                if (name == iskl){
                    iscluchen = true;
                    break;
                }
            }
            if (!iscluchen){
                uchastniki.push_back(name); 
            }
        }
        double dolya = summ / uchastniki.size(); // сколько потребил
        for (string uchastnik : uchastniki){
            potrebil[uchastnik] += dolya;
        }
    }

    file.close();

    cout << fixed << setprecision(1); //чтоб выводилось с одним знаком после запятой
    for (string name : names){
        cout << name << " " << potratil[name] << " " << potrebil[name] << endl; // вывод сколько потретили и потребили для каждого 
    }
    vector<string> dolznik; // тот кто должен
    vector<double> skolko_dolz; // сколько должен 
    vector<string> komu_dolz; // кому должен
    vector<double> kredit; // сумма кому должны

    for (string name : names){
        double balance = potratil[name] - potrebil[name];

        if (balance < -0.001){ //должен
            dolznik.push_back(name);
            skolko_dolz.push_back(-balance);
        }
        else if (balance > 0.001){ // ему должны
            komu_dolz.push_back(name);
            kredit.push_back(balance);
        }
    }
    // определяю переводы
    int i = 0;
    int j = 0;
    while (i < dolznik.size() && j < komu_dolz.size()){
        double perevod = min(skolko_dolz[i], kredit[j]);
        if (perevod > 0.001){
            cout << dolznik[i] << " " << komu_dolz[j] << endl;
        }

        skolko_dolz[i] -= perevod;
        kredit[j] -= perevod;

        if (skolko_dolz[i] < 0.001) i++;
        if (kredit[j] < 0.001) j++;
    }

    return 0;
}