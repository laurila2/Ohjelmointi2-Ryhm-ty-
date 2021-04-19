/* Class CarePeriod
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * Class for describing a patient's care period in hospital.
 * 
 * */
#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include <string>

class CarePeriod
{
public:
    // Constructor, start date given as a string (ddmmyyyy).
    CarePeriod(const std::string& start, Person* patient);

    // Constructor, start date given as a Date object.
    CarePeriod(const Date& start, Person* patient);

    // Destructor.
    ~CarePeriod();

    // Obvious getter methods
    Person *get_patient();
    Date get_startdate();
    Date get_enddate();

    // Sets careperiods end date
    void set_enddate(Date end_date);

    // Adds staff for careperiod
    void add_staff(Person *staff);

    // Prints careperiod's information neatly
    void print();

    // Prints careperiod information for one staffmember
    void print_per_staff();

private:
    Person *patient_;
    std::vector<Person *> staffmembers_;
    Date start_;
    Date end_;
};

#endif // CAREPERIOD_HH
