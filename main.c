#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void login();
void createAccount();
void bookingSystem(char userFileName[255], char password[255]);
void changeTicket();
void cancelReservation(char userFileName[255], char password[255], char movieName[255], char movieDay[255], int row, int column);
void printMovieSeats(char seats[8][8], int arrayLength);
char movies[][255] = {"movie1", "movie2", "movie3", "movie4"}; 
struct movieTickets{char name[255]; char date[3][255]; char seat[8];}; //double array for the available dates and for the layout of the seats

int main(){
    int input;
    char throwaway;
    do{
    printf("If you would like to create an account, press 1\nIf you would like to log into an account, press 2\nEnter: ");
    scanf("%d", &input);//recieves input
    scanf("%c", &throwaway); //clears input buffer
    if(input == 1){
        printf("\n************************************************************************\nCREATING ACCOUNT\n************************************************************************\n");
        createAccount();
    }
    else if (input == 2){ //login
        printf("\n************************************************************************\nLOGGING INTO ACCOUNT\n************************************************************************\n");
        login();
    }
    else{
        printf("\nInvalid input, please try again\n");
    }
    }while(input != 1 || input != 2); //exits only if a valid input is created
}
void createAccount(){
   while (true){
    char userName[255];
    char buffer[255]; //used as a dummy variable for names
    bool uniqueUsername = true; //used to check if username is unique
    char password[255];   
    printf("What would you like your username to be(lowercase): ");
    fgets(userName, 255, stdin);
    userName[strlen(userName) - 1] = '\0'; //reads name and removes whitespace at end of it
    for(int i = 0; i < strlen(userName); i++){ //makes everything lowercase
        userName[i] = tolower(userName[i]);
    }
    FILE *pF = fopen("usernames.txt", "a+"); //opens 2nd file to read it, and creates it if it doesn't exist
    
    //int length = sizeof(userName)/sizeof(userName[0]);
    while(fgets(buffer, 255, pF)){ //reads file adn checks if the username is on it
        //if(strncmp(buffer, userName, strlen(userName)) == 0){  DIDN'T WORK
        if(strncmp(buffer, userName, strlen(userName)) == 0 && buffer[strlen(userName)] == '\n'){ //checks the amount of characters in the string userName and only compares those characters 
            uniqueUsername = false;
            break;
        }
    }
    if(uniqueUsername == true){
        printf("What is your password: ");
        fgets(password, 255, stdin);
        password[strlen(password)-1] = '\0'; //removes whitespace at end of password cause fgets is just like that
        printf("Your username is %s and your password is %s", userName, password);
        fprintf(pF, "%s\n", userName); //adds username to the list of usernames. 
        //I do know that it would be more efficient to ignore the file with the list of usernames and instead list through the existing profile files of a user but I spent a lot of time figuring out how to check things in a file so I'm gonna keep it in
        char buf[255]; 
        strcpy(buf, userName);
        strcat(buf, ".txt"); //creates a variable which matches the file name
        FILE *pUser = fopen(buf, "w"); //opens a file with the username of the individual
        fprintf(pUser, "%s\nno", password); //creates a user file
        printf("\nYour username has succesfully been registered into the system!\n\n");
        fclose(pF);
        fclose(pUser);
        break;
    }
    else{
        printf("This username is already taken, try again\n");
    }
    fclose(pF);
   }
}
void login(){
    char userName[255];
    char password[255];
    char reservation[255];
    printf("Enter your username: ");
    fgets(userName, 255, stdin);
    userName[strlen(userName) - 1] = '\0'; 
    char buf[255]; 
    char readFromFile[255];
    strcpy(buf, userName);
    strcat(buf, ".txt");
    FILE *pUser1 = fopen(buf, "r"); //opens file with the username
    if (pUser1 == NULL){ //checks if file exists for user
        printf("\nThis user doesn't exist, L Bozo\n");
        exit(0);
    }
    printf("Enter your password: ");
    fgets(password, 255, stdin);
    password[strlen(password)-1] = '\0'; //removes whitespace at end of line
    fgets(readFromFile, 255, pUser1); //reads file
    if(strncmp(readFromFile, password, strlen(password)) == 0 && readFromFile[strlen(password)] == '\n'){ //checks the amount of characters in the string password and only compares those characters  
        printf("You have succesfully logged in!\n");
    }   
    else{
        printf("Haxor!\n");
        exit(0); //breaks function
    }
    fgets(reservation, 255, pUser1); //reads 2nd line
    if(strncmp(reservation, "no", strlen(reservation)) == 0){ //no reservations
        char userInput[255];
        while (true){
            printf("Hello %s, you currently have no reservations. Would you like to make a reservation Y/N: ", userName);
            fgets(userInput, 255, stdin);
            if(strncmp(userInput, "y", 1) == 0 || strncmp(userInput, "Y", 1) == 0){
                printf(".........Booking a ticket......\n");
                break;
            }
            else if (strncmp(userInput, "n", 1) == 0 || strncmp(userInput, "N", 1) == 0){
                printf("Why the flip are you here bro?\n");
                exit(0);
            }
            else{
                printf("That aint a correct input broski\n");
            }
        }
        bookingSystem(buf, password);
    }
    else{
        char movieTime[255];
        char movieSeatRow[255];
        char movieSeatColumn[255];
        fgets(movieTime, 255, pUser1);
        fgets(movieSeatRow, 255, pUser1);
        fgets(movieSeatColumn, 255, pUser1);
        int movieSeatRow1 = atoi(movieSeatRow); //integer conversion
        int movieSeatColumn1 = atoi(movieSeatColumn);
        reservation[strlen(reservation)-1] = '\0'; //removes whitespace at end of line
        movieTime[strlen(movieTime)-1] = '\0'; //removes whitespace at end of line
        printf("You currently have a reservation for %s on %s with the seat %d,%d\n", reservation, movieTime, movieSeatRow1 + 1, movieSeatColumn1 + 1);
        fclose(pUser1);
        int userInput;
        while (true) {
            printf("Would you like to change your reservation(enter 1) or would you like to cancel your reservation(enter 2): ");
            scanf("%d", &userInput);
            if(userInput == 1){
                printf("\nChanging reservation...\n");
                changeTicket();
                break;
            }
            else if(userInput == 2){
                printf("\nCancelling reservation\n");
                cancelReservation(buf, password, reservation, movieTime, movieSeatRow1, movieSeatColumn1);
                break;
            }
            else{
                printf("\nEnter correctly idiot\n");
            }
        }
        
    }
    
}

void bookingSystem(char userFileName[255], char password[255]){
    printf("Which movie would you like to book a ticket for?\n");
    int userInput;
    while(true){
        for(int i = 0; i < sizeof(movies)/sizeof(movies[0]); i++){ //prints all movies
            printf("%s(Enter %d)\n", movies[i], i+1);
        }
        scanf("%d", &userInput);
        if(userInput == 1){
            printf("Booking a ticket for movie 1");
            struct movieTickets movie1 = {"movie1", {"monday", "wednesday", "saturday"}};
            char date[255];
            char buf;
            scanf("%c", &buf); //clears input buffer
            while (true){
                printf("\nWhat date would you like to book in? The available dates are %s, %s, %s: ", movie1.date[0], movie1.date[1], movie1.date[2]);
                fgets(date, 255, stdin);
                for(int i = 0; i < strlen(date); i++){
                    date[i] = tolower(date[i]); //converts lowercase
                }
                if(strncmp(date, movie1.date[0], strlen(movie1.date[0])) == 0){ //compares strings
                    printf("\nBooking the ticket for %s\n", movie1.date[0]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie1.name);
                    strcat(movieFileName, movie1.date[0]);
                    strcat(movieFileName, ".txt");
                    
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    // if(pMovie = NULL){ //idk this caused a segmentation error in my code bro ask gandalf or smth
                    //      printf("error\n");
                    //      exit(0);
                    // }
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie1.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie1.name, movie1.date[0], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED\n");
                    exit(0);
                }
                else if(strncmp(date, movie1.date[1], strlen(movie1.date[1])) == 0){
                    printf("\nBooking the ticket for %s\n", movie1.date[1]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie1.name);
                    strcat(movieFileName, movie1.date[1]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie1.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie1.name, movie1.date[1], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED\n");
                    exit(0);
                }
                else if(strncmp(date, movie1.date[2], strlen(movie1.date[2])) == 0){
                     printf("\nBooking the ticket for %s\n", movie1.date[2]);
                     char movieFileName[255];
                    strcpy(movieFileName, movie1.name);
                    strcat(movieFileName, movie1.date[2]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie1.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie1.name, movie1.date[2], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED\n");
                    exit(0);
                }
                else{
                    printf("Not a correct date broski\n");
                }
            }

        }
        else if(userInput == 2){
            printf("\nBooking a ticket for movie 2");
            struct movieTickets movie2 = {"movie2", {"tuesday", "thursday", "saturday"}};
            char date[255];
            char buf;
            scanf("%c", &buf); //clears input buffer
            while (true){
                printf("\nWhat date would you like to book in? The available dates are %s, %s, %s: ", movie2.date[0], movie2.date[1], movie2.date[2]);
                fgets(date, 255, stdin);
                for(int i = 0; i < strlen(date); i++){
                    date[i] = tolower(date[i]); //converts lowercase
                }
                if(strncmp(date, movie2.date[0], strlen(movie2.date[0])) == 0){ //compares strings
                    printf("\nBooking the ticket for %s\n", movie2.date[0]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie2.name);
                    strcat(movieFileName, movie2.date[0]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie2.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie2.name, movie2.date[0], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED\n");
                    exit(0);

                }
                else if(strncmp(date, movie2.date[1], strlen(movie2.date[1])) == 0){
                    printf("\nBooking the ticket for %s\n", movie2.date[1]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie2.name);
                    strcat(movieFileName, movie2.date[1]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie2.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie2.name, movie2.date[1], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED\n");
                    exit(0);
                }
                else if(strncmp(date, movie2.date[2], strlen(movie2.date[2])) == 0){
                     printf("\nBooking the ticket for %s\n", movie2.date[2]);
                     char movieFileName[255];
                    strcpy(movieFileName, movie2.name);
                    strcat(movieFileName, movie2.date[2]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie2.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie2.name, movie2.date[2], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED\n");
                    exit(0);
                }
                else{
                    printf("Not a correct date broski\n");
                }
            }
        }
        else if(userInput == 3){
            printf("\nBooking a ticket for movie 3");
            struct movieTickets movie3 = {"movie3", {"wednesday", "thursday", "sunday"}};
            char date[255];
            char buf;
            scanf("%c", &buf); //clears input buffer
            while (true){
                printf("\nWhat date would you like to book in? The available dates are %s, %s, %s: ", movie3.date[0], movie3.date[1], movie3.date[2]);
                fgets(date, 255, stdin);
                for(int i = 0; i < strlen(date); i++){
                    date[i] = tolower(date[i]); //converts lowercase
                }
                if(strncmp(date, movie3.date[0], strlen(movie3.date[0])) == 0){ //compares strings
                    printf("\nBooking the ticket for %s\n", movie3.date[0]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie3.name);
                    strcat(movieFileName, movie3.date[0]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie3.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie3.name, movie3.date[0], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED!\n");
                    exit(0);

                }
                else if(strncmp(date, movie3.date[1], strlen(movie3.date[1])) == 0){
                    printf("\nBooking the ticket for %s\n", movie3.date[1]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie3.name);
                    strcat(movieFileName, movie3.date[1]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie3.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie3.name, movie3.date[1], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED!\n");
                    exit(0);
                }
                else if(strncmp(date, movie3.date[2], strlen(movie3.date[2])) == 0){
                     printf("\nBooking the ticket for %s\n", movie3.date[2]);
                     char movieFileName[255];
                    strcpy(movieFileName, movie3.name);
                    strcat(movieFileName, movie3.date[2]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie3.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie3.name, movie3.date[2], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED!\n");
                    exit(0);
                }
                else{
                    printf("Not a correct date broski\n");
                }
            }
        }
        else if(userInput == 4){
            printf("\nBooking a ticket for movie 4");
            struct movieTickets movie4 = {"movie4", {"tuesday", "thursday", "saturday"}};
            char date[255];
            char buf;
            scanf("%c", &buf); //clears input buffer
            while (true){
                printf("\nWhat date would you like to book in? The available dates are %s, %s, %s: ", movie4.date[0], movie4.date[1], movie4.date[2]);
                fgets(date, 255, stdin);
                for(int i = 0; i < strlen(date); i++){
                    date[i] = tolower(date[i]); //converts lowercase
                }
                if(strncmp(date, movie4.date[0], strlen(movie4.date[0])) == 0){ //compares strings
                    printf("\nBooking the ticket for %s\n", movie4.date[0]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie4.name);
                    strcat(movieFileName, movie4.date[0]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie4.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie4.name, movie4.date[0], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED!\n");
                    exit(0);

                }
                else if(strncmp(date, movie4.date[1], strlen(movie4.date[1])) == 0){
                    printf("\nBooking the ticket for %s\n", movie4.date[1]);
                    char movieFileName[255];
                    strcpy(movieFileName, movie4.name);
                    strcat(movieFileName, movie4.date[1]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie4.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie4.name, movie4.date[1], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED!\n");
                    exit(0);
                }
                else if(strncmp(date, movie4.date[2], strlen(movie4.date[2])) == 0){
                     printf("\nBooking the ticket for %s\n", movie4.date[2]);
                     char movieFileName[255];
                    strcpy(movieFileName, movie4.name);
                    strcat(movieFileName, movie4.date[2]);
                    strcat(movieFileName, ".txt");
                    FILE *pMovie = fopen(movieFileName, "a+"); //creates file if doesn't exit or else opens it
                    char seats[8][8]; //seats for the movie
                    for (int i = 0; i < sizeof(seats)/sizeof(seats[0]); i++)
                    {
                        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
                        {
                            seats[i][j] = 'O'; //fills array with empty seats
                        }
                    }
                    char row[255];
                    char column[255];
                    int x;
                    int y;
                    while(fgets(row, 255, pMovie)){ //reads file adn checks if the username is on it
                        fgets(column, 255, pMovie); //reads 2nd line
                        x = atoi(row); //x is the row
                        y = atoi(column); //y is the column
                        seats[y][x] = 'X';
                    }
                    int arrayLength = sizeof(seats)/sizeof(seats[0]); //have to do this cause C is stupid and identifies the array as a pointer when passed to the function
                    printMovieSeats(seats, arrayLength);
                    int userRow;
                    int userColumn;
                    while(true){
                        printf("Above are the seats for the movie, with O for empty and X for taken\nWhich row would you like (1-8): ");
                        scanf("%d", &userRow);
                        printf("\nWhich column would you like (1-8): ");
                        scanf("%d", &userColumn);
                        if(seats[userRow-1][userColumn-1] == 'O'){
                            printf("Booking seat.....\n");
                            break;
                        }
                        else{
                            printf("This seat is taken, try again\n");
                        }
                    }
                    userRow -= 1;
                    userColumn -= 1;
                    scanf("%c", &buf); //clears input buffer
                    seats[userRow][userColumn] = 'X';
                    fprintf(pMovie, "%d\n%d\n", userRow, userColumn);
                    movie4.seat[0] = (userRow, userColumn); //not necessary really, but I wanted to practice using structs
                    printMovieSeats(seats, arrayLength);
                    fclose(pMovie);
                    FILE *pUser = fopen(userFileName, "w"); //to append to the file
                    if(pUser == NULL){
                        printf("Failure\n");
                        exit(0);
                    }
                    fprintf(pUser, "%s\n%s\n%s\n%d\n%d", password, movie4.name, movie4.date[2], userRow, userColumn);
                    fclose(pUser);
                    printf("BOOKED!\n");
                    exit(0);
                }
                else{
                    printf("Not a correct date broski\n");
                }
            }
        }
        else{
            printf("\nEnter a correct input bozo");
        }
        
    }
}
void printMovieSeats(char seats[8][8], int arrayLength){
    printf("\t 1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < arrayLength; i++){ //prints the seats
        printf("%d\t", i+1);
        printf("|");
        for (int j = 0; j < sizeof(seats[0])/sizeof(seats[0][0]); j++)
        {
            printf("%c|", seats[i][j]);
        }
        printf("\n\t-----------------\n");
    }
}
void changeTicket(){

}
void cancelReservation(char userFileName[255], char password[255], char movieName[255], char movieDay[255], int row, int column){
    FILE *pCan = fopen(userFileName, "w"); //overrides old user file
    if (pCan == NULL){
        printf("File could not be opened");
    }
    fprintf(pCan, "%s\nno", password); //cancels old reservations
    fclose(pCan);
    strcat(movieName, movieDay);
    strcat(movieName, ".txt"); //makes movieName the filename of the movie
    char seatRow[255];
    char seatColumn[255]; //to store the row and columns on the file
    FILE *pMoviee = fopen(movieName, "r+"); //append
    if(pMoviee == NULL){
        printf("Error!");
        exit(0);
    }
    int remainingSeats[128];
    short int i = 0;
    while (fgets(seatRow, 255, pMoviee)){ //ends when there is nothing to read
        fgets(seatColumn, 255, pMoviee); //reads the movie files
        int seatRoww = atoi(seatRow);
        int seatColumnn = atoi(seatColumn); //converts to integers
        if(seatRoww != row && seatColumnn != column){
            remainingSeats[i] = seatRoww;
            remainingSeats[i+1] = seatColumnn;
            i += 2;
        }
    }
    fclose(pMoviee);
    FILE *pMovi = fopen(movieName, "w"); //overrides old file
    // if(pMovi == NULL){
    //     printf("Error!");
    //     exit(0);
    // }
   
    for(short int j = 0; j < i; j++){ //creates a new movie file for that date without these user seats
        fprintf(pMovi, "%d\n", remainingSeats[j]);
    } 
    fclose(pMovi);
    printf("You have no more reservations!\n");
    exit(0);
    //NEED TO PARSE THROUGH MOVIE FILE AND REMOVE THE SEAT ALLOCATIONS WHICH ARE FOUND FROM THE USER FILE
}