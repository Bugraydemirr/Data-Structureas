//
//  main.c
//  homework1
//
//  Created by buğra aydemir on 28.04.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

typedef struct sNode // student name linkedlist
{
    char val; // value of characher in name
    struct sNode* next; // next node of name char
} Node;
Node* CreateNode(const char* name) // create new linkedlist by name and return it
{
    int i = 1; // initialize first node
    Node* head = (Node*) malloc(sizeof(Node));
    head->val = (int) (*name);

    Node* prev = head;
    while (name[i] != 0) // while name has new character
    {
        Node* node = (Node*) malloc(sizeof(Node)); // new memory allocation for new char
        node->val = name[i]; // set char
        prev->next = node; // connect prev node with new created node
        prev = node; // set prev node into new created
        i++;
    }
    prev->next = NULL; // set last nodes next pointer to null

    return head;
}

// generate and set name from given linkedlist
void GenerateNameFromNode(Node* node, char name[50])
{
    int i = 0;

    while (node) // while more node avaible
    {
        if (node->val == '_') // if value equals _, set space instead of _
            name[i] = ' ';
        else // else set normally
            name[i] = node->val;
        node = node->next; // iterate into next node
        i++;
    }

    name[i] = '\0';
}

// Head struct for students
typedef struct sHead
{
    uint32_t number; // student number
    Node* name; // head node of name linkedlist
} Head;

// stack struct
typedef struct sStack
{
    uint32_t top; // index of top
    Head* data[1000]; // all data
} Stack;
void PushStack(Stack* s, Head* head)
{
    s->data[s->top] = head; // set top data into head
    s->top++; // increase top
}
void PopStack(Stack* s)
{
    s->top--; // just decrease top, dont free because each student is created once and should be free once after deleting from all nodes
}
Head* PeekStack(Stack* s)
{
    if (s->top == 0) return NULL; // if no more data(student) return null pointer
    return s->data[s->top-1]; // else return top student
}

// stacks for system
Stack stackID;
Stack stackYear;
Stack stackFaculty;

void CreateStacks()
{
    // set all stacks 0
    memset(&stackID, 0, sizeof(Stack));
    memset(&stackYear, 0, sizeof(Stack));
    memset(&stackFaculty, 0, sizeof(Stack));
}

Head* CreateStudent(uint32_t number, const char* name)
{
    Head* head = (Head*) malloc(sizeof(Head)); // memory allocate for student
    head->number = number; // set number
    head->name = CreateNode(name); // create name linkedlist and set
    return head;
}

// convert student number into different types
uint32_t StudentNumberToID(uint32_t number)
{
    uint32_t ID = number % 10000;
    return ID;
}
uint32_t StudentNumberToYear(uint32_t number)
{
    uint32_t ID = number % 10000;
    uint32_t year = ((number % 1000000) - ID) / 10000;
    return year;
}
uint32_t StudentNumberToFacultyCode(uint32_t number)
{
    uint32_t ID = number % 10000;
    uint32_t year = ((number % 1000000) - ID) / 10000;
    uint32_t facultyCode = (number - (year * 10000) - ID) / 1000000;
    return facultyCode;
}

void AddStackID(Head* student)
{
    // get student information from number
    uint32_t ID = StudentNumberToID(student->number);
    uint32_t year = StudentNumberToYear(student->number);
    uint32_t facultyCode = StudentNumberToFacultyCode(student->number);

    // create temp stack for adding sorted
    Stack tempS;
    memset(&tempS, 0, sizeof(Stack));
    Head* tempHead = NULL;

    while ((tempHead = PeekStack(&stackID))) // while more students in stack
    {
        // convert his number into usable values
        uint32_t temp_ID = StudentNumberToID(tempHead->number);
        uint32_t temp_year = StudentNumberToYear(tempHead->number);
        uint32_t temp_facultyCode = StudentNumberToFacultyCode(tempHead->number);

        if (temp_ID >= ID)
        {
            // pop from global stack and push into temp stack
            PopStack(&stackID);
            PushStack(&tempS, tempHead);
        }
        else // if that position is for new student, break
            break;
    }
    // push new student into global stack
    PushStack(&stackID, student);

    // push previous student from temp stack into global stack
    while ((tempHead = PeekStack(&tempS)))
    {
        PopStack(&tempS);
        PushStack(&stackID, tempHead);
    }
}
void AddStackYear(Head* student)
{
    // get student information from number
    uint32_t ID = StudentNumberToID(student->number);
    uint32_t year = StudentNumberToYear(student->number);
    uint32_t facultyCode = StudentNumberToFacultyCode(student->number);

    // create temp stack for adding sorted
    Stack tempS;
    memset(&tempS, 0, sizeof(Stack));
    Head* tempHead = NULL;

    while ((tempHead = PeekStack(&stackYear)))
    {
        // convert his number into usable values
        uint32_t temp_ID = StudentNumberToID(tempHead->number);
        uint32_t temp_year = StudentNumberToYear(tempHead->number);
        uint32_t temp_facultyCode = StudentNumberToFacultyCode(tempHead->number);

        if (temp_year >= year)
        {
            // pop from global stack and push into temp stack
            PopStack(&stackYear);
            PushStack(&tempS, tempHead);
        }
        else // if that position is for new student, break
            break;
    }
    // push new student into global stack
    PushStack(&stackYear, student);

    // push previous student from temp stack into global stack
    while ((tempHead = PeekStack(&tempS)))
    {
        PopStack(&tempS);
        PushStack(&stackYear, tempHead);
    }
}
void AddStackFaculty(Head* student)
{
    // get student information from number
    uint32_t ID = StudentNumberToID(student->number);
    uint32_t year = StudentNumberToYear(student->number);
    uint32_t facultyCode = StudentNumberToFacultyCode(student->number);

    // create temp stack for adding sorted
    Stack tempS;
    memset(&tempS, 0, sizeof(Stack));
    Head* tempHead = NULL;

    while ((tempHead = PeekStack(&stackFaculty)))
    {
        // convert his number into usable values
        uint32_t temp_ID = StudentNumberToID(tempHead->number);
        uint32_t temp_year = StudentNumberToYear(tempHead->number);
        uint32_t temp_facultyCode = StudentNumberToFacultyCode(tempHead->number);

        if (temp_facultyCode >= facultyCode)
        {
            // pop from global stack and push into temp stack
            PopStack(&stackFaculty);
            PushStack(&tempS, tempHead);
        }
        else // if that position is for new student, break
            break;
    }
    // push new student into global stack
    PushStack(&stackFaculty, student);

    // push previous student from temp stack into global stack
    while ((tempHead = PeekStack(&tempS)))
    {
        PopStack(&tempS);
        PushStack(&stackFaculty, tempHead);
    }
}

void AddStudent(uint32_t number, const char* name)
{
    // create new student by number and name
    Head* newStudent = CreateStudent(number, name);
    // add this student into all stacks
    AddStackID(newStudent);
    AddStackYear(newStudent);
    AddStackFaculty(newStudent);
}

Head* RemoveStackID(uint32_t number)
{
    Head* head = NULL;

    // crweated temp stack to keep up sorting
    Stack tempS;
    memset(&tempS, 0, sizeof(Stack));
    Head* tempHead = NULL;

    // pop from global stack and push into temp stack until found student by number
    while ((tempHead = PeekStack(&stackID)))
    {
        PopStack(&stackID);

        if (number == tempHead->number)
        {
            // when student found, pop it but not push
            head = tempHead;
            break;
        }

        PushStack(&tempS, tempHead);
    }

    // pop from temp stack and push into global stack
    while ((tempHead = PeekStack(&tempS)))
    {
        PopStack(&tempS);
        PushStack(&stackID, tempHead);
    }

    // return that founded student for free()
    return head;
}
Head* RemoveStackYear(uint32_t number)
{
    Head* head = NULL;

    // crweated temp stack to keep up sorting
    Stack tempS;
    memset(&tempS, 0, sizeof(Stack));
    Head* tempHead = NULL;

    // pop from global stack and push into temp stack until found student by number
    while ((tempHead = PeekStack(&stackYear)))
    {
        PopStack(&stackYear);

        if (number == tempHead->number)
        {
            // when student found, pop it but not push
            head = tempHead;
            break;
        }

        PushStack(&tempS, tempHead);
    }

    // pop from temp stack and push into global stack
    while ((tempHead = PeekStack(&tempS)))
    {
        PopStack(&tempS);
        PushStack(&stackYear, tempHead);
    }

    // return that founded student for free()
    return head;
}
Head* RemoveStackFaculty(uint32_t number)
{
    Head* head = NULL;

    // crweated temp stack to keep up sorting
    Stack tempS;
    memset(&tempS, 0, sizeof(Stack));
    Head* tempHead = NULL;

    // pop from global stack and push into temp stack until found student by number
    while ((tempHead = PeekStack(&stackFaculty)))
    {
        PopStack(&stackFaculty);

        if (number == tempHead->number)
        {
            // when student found, pop it but not push
            head = tempHead;
            break;
        }

        PushStack(&tempS, tempHead);
    }

    // pop from temp stack and push into global stack
    while ((tempHead = PeekStack(&tempS)))
    {
        PopStack(&tempS);
        PushStack(&stackFaculty, tempHead);
    }

    // return that founded student for free()
    return head;
}

void RemoveStudent(uint32_t number)
{
    // remove student from all stacks
    Head* head = RemoveStackID(number);
    RemoveStackYear(number);
    RemoveStackFaculty(number);

    // and free memory
    if (head != NULL)
        free(head);
}

void DisplayByID()
{
    printf("Sorted By ID:\n");
    // for all students in stack
    for (size_t i = 0; i < stackID.top; i++)
    {
        Head* tempHead = stackID.data[i];
        char name[50];
        // generate its name from linkedlist
        GenerateNameFromNode(tempHead->name, name);
        // and print into screen
        printf("%u %s\n", tempHead->number, name);
    }
}
void DisplayByYear()
{
    printf("Sorted By year:\n");
    // for all students in stack
    for (size_t i = 0; i < stackYear.top; i++)
    {
        Head* tempHead = stackYear.data[i];
        char name[50];
        // generate its name from linkedlist
        GenerateNameFromNode(tempHead->name, name);
        // and print into screen
        printf("%u %s\n", tempHead->number, name);
    }
}
void DisplayByFaculty()
{
    printf("Sorted By Faculty Code:\n");
    // for all students in stack
    for (size_t i = 0; i < stackFaculty.top; i++)
    {
        Head* tempHead = stackFaculty.data[i];
        char name[50];
        // generate its name from linkedlist
        GenerateNameFromNode(tempHead->name, name);
        // and print into screen
        printf("%u %s\n", tempHead->number, name);
    }
}

int main(int argc, char const *argv[])
{
    // init stacks
    CreateStacks();

    // user's selected
    int select = 1;

    // while user's select int 6(exit)
    while (select != 6)
    {
        // print menu
        printf("MENU\n");
        printf("1- Enter New Record\n");
        printf("2- Display By ID\n");
        printf("3- Display By Year\n");
        printf("4- Display By Faculty Code\n");
        printf("5- Delete a Record\n");
        printf("6- Exit\n");
        printf("Enter choice: ");
        // get selection from user
        scanf("%d", &select);

        switch (select)
        {
        case 1:
        {
            // if selection equals adding student, get informations and add new student
            uint32_t number = 0;
            char name[50];
            printf("Enter school number: ");
            scanf("%u", &number);
            printf("Enter name and surname: ");
            scanf("%s", name);
            AddStudent(number, name);
            break;
        }
        case 2:
        {
            // display selection
            DisplayByID();
            break;
        }
        case 3:
        {
            // display selection
            DisplayByYear();
            break;
        }
        case 4:
        {
            // display selection
            DisplayByFaculty();
            break;
        }
        case 5:
        {
            // get student id and remove student
            uint32_t number = 0;
            char name[50];
            printf("Enter school number: ");
            scanf("%u", &number);
            RemoveStudent(number);
            break;
        }
        }

        /*
        705102005 Ali_Say
        705102010 Ayse_say
        706122022 Mehmet_Yuce
        */

    }
    return 0;
}
