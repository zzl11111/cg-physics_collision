#include "contact.h"
bool CompareContacts(const Contact *contact1, const Contact *contact2){
    return contact1->time_of_impact < contact2->time_of_impact;
}