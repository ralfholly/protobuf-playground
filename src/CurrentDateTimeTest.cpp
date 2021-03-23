#include "current_date_time.pb.h"
#include <google/protobuf/util/json_util.h>

#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>

using namespace std;
using namespace protobuftest;
using namespace google::protobuf::util;

#define TIMEIT(code) timeit([&]() { code; })

inline double steadyClockDelta(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end)
{
    using steady_clock = std::chrono::steady_clock;
    return (end - start).count() * steady_clock::period::num / static_cast<double>(steady_clock::period::den);
}

inline double timeit(const std::function<void()>& lambda) {
    using steady_clock = std::chrono::steady_clock;
    auto start = steady_clock::now();
    lambda();
    auto end = steady_clock::now();
    return steadyClockDelta(start, end);
}

void buildMessage(CurrentDateTimeMessage* msg) {
    volatile static int m_year = 2000;
    msg->set_title("A simple test");
    msg->set_counter(42);
    CurrentDateTimeMessage::Date* date = msg->mutable_date();
    date->set_day(11);
    date->set_month(9);
    date->set_year(m_year++);
    date->set_weekday("Monday");
    CurrentDateTimeMessage::Time* time = msg->mutable_time();
    time->set_hours(1);
    time->set_minutes(2);
    time->set_seconds(3);
}

bool storeMessage(const CurrentDateTimeMessage* msg, const char* fname) {
    ofstream serializingStream(fname, ofstream::out | ofstream::binary | ofstream::trunc);
    return msg->SerializeToOstream(&serializingStream);
}

bool loadMessage(CurrentDateTimeMessage* msg, const char* fname) {
    ifstream serializingStream(fname, ofstream::binary);
    return msg->ParseFromIstream(&serializingStream);
}

void printMessage(const CurrentDateTimeMessage* msg) {
    cout << "Received a message:" << endl;
    cout << "\tTitle: " << msg->title() << endl;
    cout << "\tCounter: " << msg->counter() << endl;
    if (msg->has_date()) {
        const CurrentDateTimeMessage::Date& date = msg->date();
        cout << "\t" << date.weekday() << ", " << date.year() << "-" <<
            setw(2) << setfill('0') << date.month() << "-" <<
            setw(2) << setfill('0') << date.day() << endl;
    } else {
        cout << "No date element found!" << endl;
    }
    if (msg->has_time()) {
        const CurrentDateTimeMessage::Time& time = msg->time();
        cout << "\t" << setw(2) << setfill('0') << time.hours() << ":" <<
            setw(2) << setfill('0') << time.minutes() << ":" <<
            setw(2) << setfill('0') << time.seconds() << endl;
    } else {
        cout << "No time element found!" << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Please provide a filename!" << endl;
        exit(1);
    }
    const char* fname = argv[1];

    ifstream f(fname);
    if (!f.good()) {
        cout << "Creating current date/time entry" << endl;
        CurrentDateTimeMessage msg;
        buildMessage(&msg);
        storeMessage(&msg, fname);
    }
    CurrentDateTimeMessage msg2;
    if (loadMessage(&msg2, fname)) {
        printMessage(&msg2);
    } else {
        cerr << "Failed to load message!" << endl;
    }

    constexpr size_t reps = 10'000'000;
    double runtime = TIMEIT(
        for (size_t i = 0; i < reps; ++i) {
            CurrentDateTimeMessage msg;
            buildMessage(&msg);
        }
    );
    cout << "Runtime: " << runtime << endl;
    cout << "Average message build time: " << 1'000'000 * runtime / reps << " usec" << endl;

    return 0;
}
