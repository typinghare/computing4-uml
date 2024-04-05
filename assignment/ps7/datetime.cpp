// date and time sample code
// Copyright (C) 2015 Fred Martin
// Tue Apr 21 17:37:46 2015

// compile with
// g++ datetime.cpp -lboost_date_time
// Y. Rykalova  4/12/2021

// http://www.boost.org/doc/libs/1_58_0/doc/html/date_time/gregorian.html
// http://www.boost.org/doc/libs/1_58_0/doc/html/date_time/posix_time.html

#include <iostream>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using boost::gregorian::date;
using boost::gregorian::date_duration;
using boost::gregorian::date_period;
using boost::gregorian::from_simple_string;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;

int main() {
    // Gregorian date stuff
    const string s("2015-01-01");
    const date d1(from_simple_string(s));
    const date d2(2015, boost::gregorian::Apr, 21);
    const date_period dp(d1, d2);  // d2 minus d1
    const date_duration dd = dp.length();

    cout << "duration in days " << dd.days() << endl;

    // Posix date stuff
    const ptime t1(d1, time_duration(0, 0, 0, 0));  // hours, min, secs, nanosecs
    const ptime t2(d2, time_duration(0, 0, 0, 0));
    const time_duration td = t2 - t1;

    cout << "duration in hours " << td.hours() << endl;
    cout << "duration in ms " << td.total_milliseconds() << endl;
}