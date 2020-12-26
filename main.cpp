#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <exception>
#include <iomanip>


using namespace std;


class Date {
public:
    Date(int y = 0, int m = 0, int d = 0){
        year = y;
        month = m;
        day = d;
    }
    int GetYear() const{
        return year;
    }
    int GetMonth() const{
        return month;
    }
    int GetDay() const{
        return day;
    }
private:
    int year;
    int month;
    int day;
};

bool operator<(const Date& lhs, const Date& rhs){
    if(lhs.GetYear() == rhs.GetYear() ) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        }
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else
        return lhs.GetYear() < rhs.GetYear();

}

class Database {
public:
    void AddEvent(const Date& date, const string& event){
        if(!event.empty())
            db[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event){
        if (db.count(date) > 0) {
            set<string>::iterator it;
            it = db[date].find(event);
            if(db[date].end() != it)
                db[date].erase(it);
            else
                return false;
            if (db[date].empty())
                db.erase(date);
            return true;
        }
        else {
            if (db[date].empty())
                db.erase(date);
            return false;
        }
    }
    int  DeleteDate(const Date& date){
        int elements = db[date].size();
        if(db.count(date) > 0)
            db.erase(date);
        return elements;
    }

    void Find(const Date& date) const{
        if(db.count(date) > 0){
            for(const auto& e: db.at(date)){
                    cout << e << endl;
            }
        }
    }

    void Print() const{
        for(const auto& [key, value]: db) {
            for(const auto& e: value){
                cout << setfill('0') << setw(4) << key.GetYear() << "-" << setw(2) << key.GetMonth() << "-" << setw(2) << key.GetDay() << " ";
                    cout << e << endl;
            }
        }

    }
private:
    map< Date, set<string>> db;
};
void EnsureNextSymbol (stringstream& stream, const string& date) {
    if(stream.peek() != '-')
    {
        stringstream ss;
        ss << "Wrong date format: " << date;
        throw runtime_error(ss.str());
    }
    else
        stream.ignore(1);
}

void EnsureDateIsRight(const Date& date){

    if (date.GetMonth() <= 0 || date.GetMonth() >12){
        stringstream ss;
        ss << "Month value is invalid: " << date.GetMonth();
        throw runtime_error(ss.str());
    }
    else if(date.GetDay() <= 0 || date.GetDay() >31){
        stringstream ss;
        ss << "Day value is invalid: " << date.GetDay();
        throw runtime_error(ss.str());
    }
}

Date ParseDate(const string& s){
    stringstream stream(s);
    int year, month, day;
    stringstream ss;
    ss << "Wrong date format: " << s;
        if( !(stream >> year) )
            throw  runtime_error(ss.str());
        EnsureNextSymbol(stream, s);
        if( !(stream >> month) )
            throw  runtime_error(ss.str());
        EnsureNextSymbol(stream, s);
        if( !(stream >> day))
            throw  runtime_error(ss.str());
        string Else;
        if(stream >> Else)
            throw  runtime_error(ss.str());
        Date date = Date{year, month, day};
        EnsureDateIsRight(date);
        return date;
}

Date AddDate(stringstream& stream){
    string sdate;
    stream >> sdate;
    stream.ignore(1);
    Date date;
    date = ParseDate(sdate);
    return date;
}
int main() {
    Database db;

    string command;
    while (getline(cin, command)) {
        stringstream c(command);
        string operation;
        c >> operation;
        c.ignore(1);
        if(operation == "Add"){
                try {
                    Date date = AddDate(c);
                    string event;
                    c >> event;
                    db.AddEvent(date, event);
                }
                catch (exception& ex) {
                    cout << ex.what() << endl;
                    //break;
                }
        }
        else if(operation == "Find"){
            try {
                Date date = AddDate(c);
                db.Find(date);
            }
            catch (exception& ex) {
                cout << ex.what()<< endl;
                //break;
            }

        }
        else if(operation == "Print"){
            db.Print();
        }
        else if(operation == "Del"){
            try {
                Date date = AddDate(c);
                string event;
                if(c >> event) {
                    bool d = db.DeleteEvent(date, event);
                    if(d)
                        cout << "Deleted successfully" << endl;
                    else
                        cout << "Event not found" << endl;
                }
                else {
                    int count = db.DeleteDate(date);
                    cout << "Deleted " << count << " events" << endl;
                }
            }
            catch (exception& ex) {
                cout << ex.what()<< endl;
                //break;
            }



        }
        else if(operation == "")
            continue;
        else
            cout << "Unknown command: "<< operation << endl;
    }

    return 0;
}