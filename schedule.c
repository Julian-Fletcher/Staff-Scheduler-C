#include "schedule.h"
#define LINE_LENGTH 5000
#define MAX_WORKDAYS 5



//This will create a databse variable for the user
StaffDB createStaffDatabase(int staffCount){
    StaffDB db;
    db.dbInfo= makeArray(13, sizeof(Employee));
    db.staffCount = staffCount;
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

    printf("Staff Count: %d\n", staffDB.staffCount);
    if(!staffDB.dbInfo){
        puts("NO STAFF ARRAY");
        return 1;
    }
    //Scans from the csv file into the array of Employee Structs
    while(!feof(fp)){
        for(int i = 0; i<staffDB.staffCount;i++){
            fscanf(fp," %[^,], %[^,], %[^,], %d", staffDB.dbInfo[i].lastName, staffDB.dbInfo[i].firstName, staffDB.dbInfo[i].position, &staffDB.dbInfo[i].shifts);
        }
    }
    printStaff(staffDB); //Check that the scan worked!
    fclose(fp);
    return 0;
}

/*Import a csv file of Employees and put them into the database
* Takes a filename and staff database
* Returns 0 if successful, 1 if unsuccesfful
*/
int importAvailability(StaffDB staffDB, char *AVAILABILITY){
    FILE *fp = fopen(AVAILABILITY, "r");
    if(!fp){
        return -1;
    }
    char line[LINE_LENGTH];
    char delimiter[] = ",";
    char *token;
    char *tokenMemory;
    char *lastName;
    //array of weekday arrays to store availability
    double **weekdays = makeArray(MAX_WORKDAYS, sizeof(double *));


    //Throw away the header line
    if(fgets(line, sizeof(line), fp) == NULL){
        puts("Failed to read header line");
        return 1;
    }
    while(fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0'; // Null terminate the string
        token = strtok_r(line, delimiter, &tokenMemory);    //saves the location within line
        int dayCounter = 0;
        while (token != NULL && dayCounter < MAX_WORKDAYS){
            if(token[0] == '"'){
                puts("Tokenizing tokern");
                weekdays[dayCounter] = processWeekday(staffDB, token);
                dayCounter++;
            }   
            else{
            lastName = malloc(strlen(token) + 1);
            strcpy(lastName, token);
            printf("Last Name: %s\n", lastName);
            }
            token = strtok_r(NULL, delimiter, &tokenMemory);
        }
    }


    fclose(fp);
    
    return 0;
}

                    
                
double * processWeekday(StaffDB staffDB, char *token){
    int arraySize = 1;
    int arrayPosition = 0;
    char *newToken;
    char *newTokenMemory;
    char *delimiter = ",";
    char *string;
    printf("Token before cleaning: %s\n", token);
    //Cleaning up the token for use
    memmove(token, token + 1, strlen(token) -2);
    token[strlen(token) - 1] = '\0';
    printf("Token after cleaning: %s\n", token);

    newToken = strtok_r(token, delimiter, &newTokenMemory);

    printf("Token being converted: %s\n", newToken);

    double *array = makeArray(arraySize, sizeof(double *));
    array[arrayPosition] = strtod(newToken, &string);
    printf("DOUBLE IN ARRAY: %.2f\n", array[arrayPosition]);
    
    while(newToken != NULL){
        newToken = strtok_r(NULL, ", ", &newTokenMemory);
        arraySize++;
        arrayPosition++;
        resizeArray(array, arraySize + 1, sizeof(double));
        printf("SIZE: %d\n", getSize(array));
        printf("ARRAY SIZE VAR %d\n", arraySize);
        for(int i = 0; i < arraySize - 1; i++){
            printf("%f\n", array[i]);
        }
        array[arrayPosition - 1] = strtod(newToken, &string);
        puts("a");
        printf("DOUBLE IN ARRAY: %.2f\n", array[arrayPosition]);
    }


   return NULL;
}


//This will generate a schedule based on the number of shifts required to be filled, and the days/hours that are available
int createSchedule(StaffDB staffDB);
//Print off the weekly schedule for all employees
void printSchedule(StaffDB staffDB);
//Print off the weekly availability for all employees
void printAvailability(StaffDB staffDB);


//Returns the position number of the staff member with the matching last name in O(n) time
int getStaffPos(StaffDB staffDB, char *lastName){
    int position = 0;
    for(int i = 0;i < staffDB.staffCount ;i++){
        if(strcmp(lastName, staffDB.dbInfo[i].lastName) == 0){
            position = i;
            break;
        }
    }
    if(position == staffDB.staffCount){
        return -1;
    }    
    return position;
}

//Print off the weekly schedule for one employee
void printIndividualSchedule(StaffDB staffDB, char *lastName);
//Print off the weekly availablity for one employee
void printIndividualAvailability(StaffDB staffDB, char *lastName);


//Prints all staff
void printStaff(StaffDB staffDB){
    int size = getSize(staffDB.dbInfo);
    printf("%s, %s, %s, %s\n", "FIRST NAME", "LAST NAME", "POSITION", "SHIFTS");
    for(int i = 0;i<size;i++){
        printf("%s, %s, %s, %d\n",staffDB.dbInfo[i].lastName, staffDB.dbInfo[i].firstName, staffDB.dbInfo[i].position, staffDB.dbInfo[i].shifts);
    }
}
//Frees all associated memory with a staff database and voids the users variable
void freeStaffDatabase(StaffDB staffdb);

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
    return *(((int *)array) -1);
}

/*Frees an array created by MakeArray*/
void freeArray(void *array){
    free(((int *)array)-1);
}

void * resizeArray(void *array, int newArraySize, int elementSize){
    puts("we");
    int *originalArray = (((int *)array) - 1);
    int *resizedArray = realloc(originalArray, (newArraySize * elementSize) + sizeof(int));
    resizedArray[0] = newArraySize;
    return (void *)resizeArray;
}

//Strtok-R????

/* 
 * public domain strtok_r() by Charlie Gordon
 *
 *   from comp.lang.c  9/14/2007
 *
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

char* strtok_r(
    char *str, 
    const char *delim, 
    char **nextp)
{
    char *ret;

    if (str == NULL)
    {
        str = *nextp;
    }

    str += strspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
}