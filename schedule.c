#include "schedule.h"

//This will create a databse variable for the user
StaffDB createStaffDatabase(){
    StaffDB db;
    db.dbInfo=NULL;
    return db;
}

/*This will import an excel file of Employees and put them into the databse
*Takes a filename and a staff variable
* Returns 0 if successful, 1 if unsuccessful
*/
int importStaff(StaffDB staffDB, char *FILENAME){
    FILE *fp = fopen(FILENAME,"r");
    if(!fp){
        return -1;
    }
    
    int staffCount = 0;
    printf("Number of Staff: ");
    scanf("%d", &staffCount);
    //fscanf(fp, "%d", &staffCount);
    printf("Staff Count: %d\n", staffCount);
    staffDB.dbInfo= makeArray(staffCount, sizeof(Employee));
    if(!staffDB.dbInfo) return 1;
    //Scans from the csv file into the array of Employee Structs
    while(!feof(fp)){
        for(int i = 0; i<staffCount;i++){
            fscanf(fp," %[^,], %[^,], %[^,], %d", staffDB.dbInfo[i].lastName, staffDB.dbInfo[i].firstName, staffDB.dbInfo[i].position, &staffDB.dbInfo[i].shifts);
        }
    }
    printStaff(staffDB); //Check that the scan worked!
    return 0;
}

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
void printStaff(StaffDB staffDB){
    int size = getSize(staffDB.dbInfo);
    printf("%s %20s %30s %50s\n", "FIRST NAME", "LAST NAME", "POSITION", "SHIFTS");
    for(int i = 0;i<size;i++){
        printf("%s, %s, %s, %d\n",staffDB.dbInfo[i].lastName, staffDB.dbInfo[i].firstName, staffDB.dbInfo[i].position, staffDB.dbInfo[i].shifts);
    }
}


/*Dynamically creates an array of a specificed size
* Requires the size of the array and element size as integers
*/
void * makeArray(int arraySize, int elementSize){
    int *array;
    array = malloc(arraySize * elementSize + sizeof(int));
    if(!array){
        return NULL;
    }
    array[0] = arraySize;
    return(void *)(array + 1);
}

/*Returns the integer size of an array created by MakeArray*/
int getSize(void *array){
    return ((int *)array)[-1];
}


/*Frees an array created by MakeArray*/
void freeArray(void *array){
    free(((int *)array)-1);
}