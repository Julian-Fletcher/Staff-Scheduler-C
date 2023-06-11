#include "schedule.h"
#define LINE_LENGTH 5000
#define MAX_COLUMNS 6


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
    staffDB.staffCount = staffCount;
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

    //Variable to store lines
    char line[LINE_LENGTH];
    //Variable to store columns
    char *columns[MAX_COLUMNS];
    //Token variable
    char *token;

    //Throw away the header line
    if(fgets(line, sizeof(line), fp) == NULL){
        puts("Failed to read header line");
        return 1;
    }


    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line, "\n")] = '\0'; //Remove newline character
       // printf("LINE READ: %s\n", line);
        int columnCount = 0;
        token = strtok(line, ",");
        while (token != NULL && columnCount < MAX_COLUMNS){
            //Checking if the current section is within quotation marks
            if(token[0] == '"'){
                char nextToken[LINE_LENGTH];
                strcpy(nextToken, token);
                while(token[strlen(token) - 1] != '"'){
                    token = strtok(NULL, ",");
                    strcat(nextToken, ",");
                    strcat(nextToken, token);
                }
                //printf("NEXT TOKEN: %s\n", nextToken);
                
                //Remove quotation marks
                memmove(nextToken, nextToken + 1, strlen(nextToken) -2);
                nextToken[strlen(nextToken) - 1] = '\0';
                
                //Allocate space and then copy the full day availability into the column
                columns[columnCount] = malloc(strlen(nextToken) + 1);
                strcpy(columns[columnCount], nextToken);
            }
            //For the last name
            else{
                //Allocate space and then copy the last name into the column
                columns[columnCount] = malloc(strlen(token) + 1);
                strcpy(columns[columnCount], token);
            }
            columnCount++;
            token=strtok(NULL, ",");
        }
        for(int i = 0;i < MAX_COLUMNS;i++){
            printf("COLUMN %d: %s\n", i, columns[i]);
        }

        /*Put columns into the corresponding staff member's availability*/

        //Once everything has been put into the databse, free the columns
        for(int i = 0; i < MAX_COLUMNS;i++){
            free(columns[i]);
        }
    }


    fclose(fp);
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
    return ((int *)array)[-1];
}

/*Frees an array created by MakeArray*/
void freeArray(void *array){
    free(((int *)array)-1);
}