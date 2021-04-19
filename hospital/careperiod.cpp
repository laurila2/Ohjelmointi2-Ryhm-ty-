#include "careperiod.hh"
#include <iostream>
#include <memory>

CarePeriod::CarePeriod(const std::string &start, Person *patient)
    : patient_(patient), start_(start)
{
}

CarePeriod::CarePeriod(const Date &start, Person *patient)
    : patient_(patient), start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

Person *CarePeriod::get_patient()
{
    return patient_;
}

Date CarePeriod::get_startdate()
{
    return start_;
}

Date CarePeriod::get_enddate()
{
    return end_;
}

void CarePeriod::set_enddate(Date end_date)
{
    end_ = end_date;
}

void CarePeriod::add_staff(Person *staff)
{
    staffmembers_.push_back(staff);
}

void CarePeriod::print()
{
    std::cout << "* Care period: ";
    start_.print();
    std::cout << " - ";

    if (!end_.is_default())
    {
        end_.print();
    }
    std::cout << std::endl;

    std::cout << "  - Staff:";
    if (staffmembers_.empty())
    {
        std::cout << " None";
    }
    else
    {
        for (Person *staff : staffmembers_)
        {
            std::cout << " ";
            staff->print_id();
        }
    }
    std::cout << std::endl;
}

void CarePeriod::print_per_staff()
{
    start_.print();
    std::cout << " - ";

    if (!end_.is_default())
    {
        end_.print();
    }
    std::cout << std::endl;

    std::cout << "  * Patient: ";
    patient_->print_id();
    std::cout << std::endl;
}
