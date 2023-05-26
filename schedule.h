#include <stdio.h>
#include <stdlib.h>

typedef struct Weekdays{
    double monday[48];
    double tuesday[48];
    double wednesday[48];
    double thursday[48];
    double friday[48];
}Weekdays;

typedef struct EmployeeData{
    char firstName[256];
    char lastName[256];
    char position[256];
    int shifts; //number of weekly shifts to be filled
    Weekdays availability;
    Weekdays schedule;
}Employee;

typedef struct StaffDB{
    Employee *dbInfo;
}StaffDB;

//This will create a databse variable for the user
StaffDB createStaffDatabase();
//This will import an excel file of Employees and put them into the databse
int importStaff(StaffDB staffDB, char *FILENAME);
//This will generate a schedule based on the number of shifts required to be filled, and the days/hours that are available
int createSchedule(StaffDB staffDB);
//Print off the weekly schedule for all employees
void printSchedule(StaffDB staffDB);
//Print off the weekly availability for all employees
void printAvailability(StaffDB staffDB);
//Print off the weekly schedule for one employee
void printIndividualSchedule(StaffDB staffDB, char *lastName);
//Print off the weekly availablity for one employee
void printIndividualAvailability(StaffDB staffDB, char *lastName);
//Prints all staff
void printStaff(StaffDB staffDB);
//Frees all associated memory with a staff database and voids the users variable
void freeStaffDatabase(StaffDB staffdb);


/* Array Functions*/
void * makeArray(int arraySize, int elementSize);
int getSize(void *array);
void freeArray(void *array);