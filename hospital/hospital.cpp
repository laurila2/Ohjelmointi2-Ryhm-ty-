#include "hospital.hh"
#include "utils.hh"
#include <algorithm>
#include <iostream>
#include <set>

Hospital::Hospital()
{
}

Hospital::~Hospital()
{
    // Deallocating staff
    for( std::map<std::string, Person*>::iterator
         iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Deallocate patients
    for (std::map<std::string, Person *>::iterator iter = all_patients_
                                                              .begin();
         iter != all_patients_.end();
         ++iter)
    {
        delete iter->second;
    }

    // Deallocate careperiods
    for (std::map<std::string, std::vector<CarePeriod *>>::iterator iter_periods
         = all_careperiods_.begin();
         iter_periods != all_careperiods_.end();
         ++iter_periods)
    {
        for (auto period : iter_periods->second)
        {
            delete period;
        }
    }
}

void Hospital::recruit(Params params)
{
    std::string specialist_id = params.at(0);
    if (staff_.find(specialist_id) != staff_.end())
    {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }
    Person *new_specialist = new Person(specialist_id);
    staff_.insert({specialist_id, new_specialist});

    std::cout << STAFF_RECRUITED << std::endl;
}

void Hospital::enter(Params params)
{
    std::string patient_id = params.at(0);
    if (current_patients_.find(patient_id) != current_patients_.end())
    {
        std::cout << ALREADY_EXISTS << patient_id << std::endl;
        return;
    }

    Person *new_patient = nullptr;
    if (all_patients_.find(patient_id) != all_patients_.end())
    {
        new_patient = all_patients_.at(patient_id);
    }
    else
    {
        new_patient = new Person(patient_id);
        all_patients_.insert({patient_id, new_patient});
    }

    current_patients_.insert({patient_id, new_patient});

    CarePeriod *new_careperiod = new CarePeriod(utils::today, new_patient);
    all_careperiods_[patient_id].push_back(new_careperiod);

    std::cout << PATIENT_ENTERED << std::endl;
}

void Hospital::leave(Params params)
{
    std::string patient_id = params.at(0);
    if (current_patients_.find(patient_id) == current_patients_.end())
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    current_patients_.erase(patient_id);

    for (CarePeriod *period : all_careperiods_[patient_id])
    {
        period->set_enddate(utils::today);
    }

    std::cout << PATIENT_LEFT << std::endl;
}

void Hospital::assign_staff(Params params)
{
    std::string staff_id = params.at(0);
    std::string patient_id = params.at(1);

    std::map<std::string, Person *>::const_iterator patient_iter
        = current_patients_.find(patient_id);
    if (patient_iter == current_patients_.end())
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    std::map<std::string, Person *>::const_iterator staff_iter = staff_.find(
        staff_id);
    if (staff_iter == staff_.end())
    {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    all_careperiods_[patient_id].back()->add_staff(staff_iter->second);
    staffmembers_patients_[staff_iter->first].push_back(patient_iter->second);

    std::cout << STAFF_ASSIGNED << patient_id << std::endl;
}

void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);
    if( not utils::is_numeric(strength, true) or
        not utils::is_numeric(dosage, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;
}

void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

void Hospital::print_patient_info(Params params)
{
    std::string patient_id = params.at(0);

    std::map<std::string, Person *>::const_iterator patient_iter
        = all_patients_.find(patient_id);
    if (patient_iter == all_patients_.end())
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    for (CarePeriod *period : all_careperiods_.at(patient_id))
    {
        period->print();
    }

    // Medicines
    std::cout << "* Medicines:";
    patient_iter->second->print_medicines("  - ");
}

void Hospital::print_care_periods_per_staff(Params params)
{
    std::string staff_id = params.at(0);

    std::map<std::string, std::vector<Person *>>::const_iterator staff_iter
        = staffmembers_patients_.find(staff_id);

    std::map<std::string, Person *>::const_iterator staff_iter2 = staff_.find(
        staff_id);

    if (staff_iter == staffmembers_patients_.end()
        && staff_iter2 != staff_.end())
    {
        std::cout << "None" << std::endl;
        return;
    }

    else if (staff_iter == staffmembers_patients_.end())
    {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    for (Person *person : staffmembers_patients_[staff_id])
    {
        for (CarePeriod *period : all_careperiods_[person->get_id()])
        {
            period->print_per_staff();
        }
    }
}

void Hospital::print_all_medicines(Params)
{
    if (all_careperiods_.empty())
    {
        std::cout << "None" << std::endl;
        return;
    }

    std::map<std::string, std::vector<std::string>> all_medicines;

    for (auto &patient : all_patients_)
    {
        std::vector<std::string> laakkeet = patient.second->get_medicines();

        for (std::string &laake : laakkeet)
        {
            all_medicines[laake].push_back(patient.first);
        }
    }

    for (auto &medicine : all_medicines)
    {
        std::cout << medicine.first << " prescribed for" << std::endl;
        std::sort(medicine.second.begin(), medicine.second.end());

        for (auto &patient : medicine.second)
        {
            std::cout << "* " << patient << std::endl;
        }
    }
}

void Hospital::print_all_staff(Params)
{
    if( staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }
    for( std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        std::cout << iter->first << std::endl;
    }
}

void Hospital::print_all_patients(Params)
{
    if (all_patients_.empty())
    {
        std::cout << "None" << std::endl;
        return;
    }
    for (std::map<std::string, Person *>::const_iterator patient_iter
         = all_patients_.begin();
         patient_iter != all_patients_.end();
         ++patient_iter)
    {
        patient_iter->second->print_id();
        std::cout << std::endl;

        for (CarePeriod *period : all_careperiods_.at(patient_iter->first))
        {
            period->print();
        }

        // Medicines
        std::cout << "* Medicines:";
        patient_iter->second->print_medicines("  - ");
    }
}

void Hospital::print_current_patients(Params)
{
    if (current_patients_.empty())
    {
        std::cout << "None" << std::endl;
        return;
    }
    for (std::map<std::string, Person *>::const_iterator iter
         = current_patients_.begin();
         iter != current_patients_.end();
         ++iter)
    {
        std::cout << iter->first << std::endl;
    }
}

void Hospital::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not utils::is_numeric(day, false) or
        not utils::is_numeric(month, false) or
        not utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utils::today.print();
    std::cout << std::endl;
}

void Hospital::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    utils::today.print();
    std::cout << std::endl;
}
