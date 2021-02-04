/**************************************************************************************
* Author: Ben Keil
* Course: EGR 226 - 905
* Date: 01/28/2021
* Project: Lab 2 part 2 Book Database
* File: lab 2 Keil part 2.c
* Description: This code allows the user to search for a book by title, author name,
* or ISBN. The code will then print out all matches for the user entry found in its
* database.
**************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 FILE* infile;
typedef struct book{
char title[255];
char author_name[50];
char ISBN[10];
char pages[4];
char year_published[4];
}book;

struct book book_array[360];
char booklist[512];
int parse_file(char booklist[], book book_array[]);
char option;
char user_title[255];
char user_author_name[50];
char user_ISBN[10];
int results = 0;
char year[4];
void print_book(int);
/**-----------------------------------------------------------
* Function: main
* Description: The function prompts the user to choose what
* to search by. Once a valid option has been selected the
* program calls the respective function.
* Inputs:
* (char) option: user choice of search method
* (int) results: number of matches found
*---------------------------------------------------------*/
int main(){
parse_file(booklist, book_array);
printf("What would you like to search by?\n");
printf("Enter 0 to search by title. Enter 1 to search by author. Enter 2 to search by ISBN.\n");
scanf(" %s", &option);
while (option != '0' && option != '1' && option != '2'){
printf("Invalid! Please try again\n");
scanf(" %s", &option);
}
if (option == '0'){
search_title ();
}
if (option == '1'){
search_author ();
}
if (option == '2'){
search_ISBN ();
}
if (results == 0){
printf("No results found!\n");
}
return 0;
}


/**-----------------------------------------------------------
* Function: parse_file
* Description: Sift through a file of books to populate
* an empty book array
* Inputs:
* (char*) booklist: file to open and parse
* (book*) book_array: array of book structures to
* populate
* Outputs:
* (int) parse status, 1 if successful, 0 otherwise.
*---------------------------------------------------------*/
int parse_file(char booklist[], book book_array[]) {

 FILE* infile=fopen("BookList.csv", "r"); // Attempt to open file
 if (infile == NULL) // Return 0 (failure) if file could not open
 return 0;

 char buffer[512]; // Create temporary string buffer variable
 int i = 0; // Indexer for book array

 while (fgets(buffer, 512, infile)) { // Loop collecting each line from the file

 char * ptr = strtok(buffer,","); // Parse string by commas and newline
 if(strcmp(ptr,"N/A")) // Validate string
 strcpy(book_array[i].title,ptr);// First parse is title
 ptr = strtok(NULL,",\n");
 if(strcmp(ptr,"N/A"))
    strcpy(book_array[i].author_name,ptr);
 ptr = strtok(NULL,",\n");
  if(strcmp(ptr,"N/A"))
 strcpy(book_array[i].ISBN,ptr);
 ptr = strtok(NULL,",\n");
  if(strcmp(ptr,"N/A"))
 strcpy(book_array[i].pages,ptr);
 ptr = strtok(NULL ,",\0");
if(strcmp(ptr,"N/A"))
 strcpy(book_array[i].year_published,ptr);
 i++;
 }
 return 1;
}

/**-----------------------------------------------------------
* Function: search_title
* Description: takes user inputted book title and finds any
* matches
* Inputs:
* (char) user_title: book title provided by user
* (book*) book_array: array of book structures
* Outputs:
* (int) result: used with strcmp to determine if there is a
* match
* (int) n: array number of matching book (in this context)
*---------------------------------------------------------*/
void search_title (){
    int n;
printf("Enter book title.\n");
scanf(" %[^\n]%*c", &user_title);
int result;
for (n = 0; n < 360; ++n){
    result = strcmp(book_array[n].title, user_title);
    if(result == 0){
    print_book(n);
    }
    }
}
/**-----------------------------------------------------------
* Function: search_author
* Description: takes user inputted author name and finds any
* matches
* Inputs:
* (char) user_author_name: author name provided by user
* (book*) book_array: array of book structures
* Outputs:
* (int) result: used with strcmp to determine if there is a
* match
* (int) n: array number of matching book (in this context)
*---------------------------------------------------------*/
void search_author (){
    int n;
printf("Enter author name.\n");
scanf(" %[^\n]%*c", &user_author_name);
int result;
for (n = 0; n < 360; ++n){
    result = strcmp(book_array[n].author_name, user_author_name);
    if(result == 0){
    print_book(n);
    }
    }
}

/**-----------------------------------------------------------
* Function: search_ISBN
* Description: takes user inputted ISBN and finds any matches
* Inputs: book title
* (char) user_ISBN: ISBN provided by user
* (book*) book_array: array of book structures
* Outputs:
* (int) result: used with strcmp to determine if there is a
* match
* (int) n: array number of matching book (in this context)
*---------------------------------------------------------*/
void search_ISBN (){
    int n;
printf("Enter ISBN.\n");
scanf(" %[^\n]%*c", &user_ISBN);
int result;
for (n = 0; n < 360; ++n){
    result = strcmp(book_array[n].ISBN, user_ISBN);
    if(result == 0){
    print_book(n);
    }
    }
}
/**-----------------------------------------------------------
* Function: print_book
* Description: prints out any matches found
* Inputs:
* (int) j: book number in array of matching book
* (book*) book_array: array of book structures to
* popuate
* Outputs:
* (int) result: used with strcmp to determine if there is a
* match
* (int) results: number of matches found
*---------------------------------------------------------*/
void print_book(int j){
    int r;
    printf("\nResult found!\n");
    printf("Title is:          %s\n", book_array[j].title);
    printf("Author is:         %s\n", book_array[j].author_name);
    printf("ISBN is:           %s\n", book_array[j].ISBN);
    printf("Page count is:     %s\n", book_array[j].pages);
    printf("Year published is: ");
         for (r = 0; r < 4; ++r){
    printf("%c", book_array[j].year_published[r]);
        }
    printf("\n");
 results = results + 1;
}

