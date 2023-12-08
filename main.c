#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void login();
void createAccount();
void bookingSystem();
void changeTicket();
void cancelReservation(char userFileName[255], char password[255]);
enum movies{movie1, movie2}; //placeholder
struct movieTickets{char name[255]; char date[255]; char seat[2];};

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
    }
    else{
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
        bookingSystem();
    }
    else{
        char movieName[255];
        char movieTime[255];
        char movieSeat[255];
        fgets(movieName, 255, pUser1);
        fgets(movieTime, 255, pUser1);
        fgets(movieSeat, 255, pUser1);
        printf("You currently have a reservation for %s at %s with the seat %s", movieName, movieTime, movieSeat);
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
                cancelReservation(buf, password);
                break;
            }
            else{
                printf("\nEnter correctly idiot\n");
            }
        }
        
    }
    
}

void bookingSystem(){
    //use the enum
    //use the struct
    //add new line for movie name
    //add new line for movie time
    //add new line for movie seat
}
void changeTicket(){

}
void cancelReservation(char userFileName[255], char password[255]){
    FILE *pCan = fopen(userFileName, "w"); //overrides old user file
    if (pCan == NULL){
        printf("File could not be opened");
    }
    fprintf(pCan, "%s\nno", password); //cancels old reservations
    fclose(pCan);
    printf("You have no more reservations!\n");
}