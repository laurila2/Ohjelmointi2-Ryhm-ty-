/* Class Person
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * Class for describing a person that can be a patient or a staff member
 * in hospital.
 *
 * */
#ifndef PERSON_HH
#define PERSON_HH

#include "date.hh"
#include <map>
#include <string>
#include <vector>

class Person
{
public:
    // Default constructor.
    Person();

    // Constructor with an id.
    Person(const std::string& id);

    // Constructor with an id and a date of birth (actually useless)
    Person(const std::string& id, const std::string& date_of_birth);

    // Destructor.
    ~Person();

    // Obvious getter methods.
    std::string get_id() const;
    std::vector<std::string> get_medicines() const;

    // Adds a medicine for the person.
    void add_medicine(const std::string& name,
                      unsigned int strength,
                      unsigned int dosage);

    // Removes medicine from the person.
    void remove_medicine(const std::string& name);

    // Prints person's id.
    void print_id() const;

    // Prints person's medicines.
    void print_medicines(const std::string &pre_text) const;

    // Comparison operator, enables forming a set of Person objects.
    bool operator<(const Person &rhs) const;

private:
    std::string id_;     // Can be a name or any other identifier
    Date date_of_birth_; // Not actually needed

    struct Prescription
    {
        unsigned int strength_;
        unsigned int dosage_;
    };
    // Medicine names and their prescriptions
    std::map<std::string, Prescription> medicines_;
};

#endif // PERSON_HH
