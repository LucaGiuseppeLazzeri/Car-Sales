#include <stdio.h>
#include <string.h>     // allows us to use strings within my code
#include <time.h>       // used for outputting the time data was outputted to a file
#include <errno.h>      // adds error codes for specific file errors
#include <ctype.h>      // allows for use of tolower functions


// Constants
// Used to make code clean

#define MENU_OPTION_BUY 'a'
#define MENU_OPTION_VIEW_SALES 'b'
#define MENU_OPTION_VIEW_STOCK 'c'
#define MENU_OPTION_VIEW_PREVIOUS_SALES 'd'

#define MENU_OPTION_EXIT 'x'
#define TRUE 1
#define FALSE 0
#define clear() printf("\033[H\033[J")


// Discounts amount for discount 0 & 1
#define INSURANCE_NCB 0.2f // DISCOUNT 1
#define MAX_SALES 10

// Establishing variables
char customerName[MAX_SALES][200], NCB_qual[MAX_SALES];
char carModel[][10] = {"Nissan", "Ford", "Toyota"};
unsigned int carsAvaliable[] = {10, 9, 8};
float carPrices[] = {49999.00f, 55000.00f, 17800.00f};
int carInBasket;
int userChoice, carQuant;
int subTotal, numberOfSales = 0;
int customerAge[MAX_SALES];
int discountTotal[MAX_SALES];
int finalPrice[MAX_SALES];
int records = 0;

//used for input validation. CICheck = CorrectInputCheck
int correctInput = 0;
int CICheck;

unsigned short carQuantPerSale[MAX_SALES];
unsigned short carModelPerSale[MAX_SALES];

typedef struct {
    int salesIndex;
    int saleAmount;
    int age;
    char customerName[50];
    int quantity;
    int carModelID;
    int totalDiscount;
    char NCB[3];
}SalesData;

SalesData data [50];


void newLine(){
    //if you don't know what this does why did you open this file
    printf("\n");
}

void astriskLine(){
    //creates neat border
    for (int i = 0; i < 40; i++) {
        printf("*");
    } // end of for
}
/*
void ID2Name(){
    strcpy(data[numberOfSales].carModelName,carModel[carInBasket]);
    printf("%s", data[numberOfSales].carModelName);
}
*/
void menu_previousSales(){

    clear();

    astriskLine();
    printf("\n");

    FILE *filePointer = fopen("Sales_data_previous.csv", "r");

    //returns an error
    if (filePointer == NULL){
        printf("Error\n");
    } // end of if
    // reads the data from a file and stores that infomation within the charArra
    char charArray[1024];
    while (fgets(charArray, 1024, filePointer)!= NULL){
        printf("%s", charArray);
        fflush(stdout);
    }   // end of while


    printf("\n");
    astriskLine();
    printf("\n");
}

//it works, but it doesn't really work. maybe i was wrong it might?
void bubbleSort(){
    //calculates size of carPrices to tell us total ammount of cars in arrays
    int numberOfCars = sizeof(carsAvaliable) / sizeof(unsigned int);

    for (int i = 0; i < numberOfCars - 1; i++){
        for (int j = i+1; j < numberOfCars; j++){
            if (carsAvaliable[i] > carsAvaliable[j]){
                //sorting out the order of carsAvaliable
                unsigned int temp = carsAvaliable[i];
                carsAvaliable[i] = carsAvaliable[j];
                carsAvaliable[j] = temp;
                //sorting out thr car models array to be in line with cars avalaiable 
                char temp2[20];
                strcpy(temp2, carModel[i]);
                strcpy(carModel[i], carModel[j]);
                strcpy(carModel[j], temp2);
                //staying in line with carsAvaliable
                float temp3 = carPrices[i];
                carPrices[i] = carPrices[j];
                temp3 = carPrices[j];

                
            } // end of if
        } // end of for 2
    } // end of for 1

}

void menu_viewStock(){
    clear();

    bubbleSort();
    int numberOfCars = sizeof(carsAvaliable) / sizeof(unsigned int);

    printf("Models Remaining\n");
    //compares the size of carsAvaliable to the size of a float to determine how many diffrent modles there are
    for (int i = 0; i < numberOfCars; i++){
        printf("%s Left in Stock: %d\n", carModel[i], carsAvaliable[i]);
    } // end of for

    printf("\n");
}

void menu_viewSales(){
    // View Sales also acts as a save button in my car sales program allowing for saving to an external file
    FILE* file;
    char fileName[] = "Sales_data_previous.csv";

    file = fopen(fileName, "a");

    if (file == NULL){
        printf("Opperation failed. ERROR NO: %d\n", errno);
    } // end of if

    if (file != NULL){
        printf("Read Succsessful\n");
    } // end of if

    //creates neat border
    astriskLine();

    printf("\n");
    //  prints out all sale realated infomation to the console
    for (int i = 0; i < numberOfSales; i++) {

        printf("|index|\t|saleAmount|\t|customerName|\t|age|\t|quanity|\t|carmodel|\t|NCB y/n|\t|totalDiscount|\n");

        printf("%d\t%d\t\t%s\t\t%d\t%d\t\t%d\t\t%s\t\t%d\n",
               data[i].salesIndex,
               data[i].saleAmount,
               data[i].customerName,
               data[i].age,
               data[i].quantity,
               data[i].carModelID,
               data[i].NCB,
               data[i].totalDiscount);

        // code reused from https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program (Adam Rosenfield)
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(file,"\nRecorded at: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        //  prints the same infomation within a csv file
        fprintf(file,
                "%d,%2.d,%s,%d,%d,%d,%s,%d\n\n",
                data[i].salesIndex,
                data[i].saleAmount,
                data[i].customerName,
                data[i].age,
                data[i].quantity,
                data[i].carModelID,
                data[i].NCB,
                data[i].totalDiscount);

        //"\n sales index, total price, name, age, Quantity, Model\n%d %2.d, %d, %s, %s, %d", i, totalSales[i], customerName[i], customerAge[i], carQuantPerSale[i], carModelPerSale[i]);
        //fprintf(file, "\n Sales index, Sale Amount, customerName, customer age, quantity, car Model, Total Discount, \n %d, %2.d, %s, %d, %d, %s, %d" i, totalSales[i], customerName[i], customerAge[i], carQuantPerSale[i], carModelPerSale[i]);

    } // end of for
    
    fflush(stdout);

    astriskLine();
    printf("\n");

    fclose(file);
}

//needs more specific name
void validation(){
    while(1){
        printf("Please input Y or N\n");
        scanf("%s", &NCB_qual[numberOfSales]);
        NCB_qual[numberOfSales] = tolower(NCB_qual[numberOfSales]);
        strcpy(data[numberOfSales].NCB, &NCB_qual[numberOfSales]);
        if (NCB_qual[numberOfSales] == 'y'){
            break;
        }// end of if

        if (NCB_qual[numberOfSales] == 'n'){
            break;
        } // end of if
    } // end of while
}

void getInteger(){
    do{
        CICheck = scanf("%d", &customerAge[numberOfSales]);

        if (CICheck == 0){
            printf("Please enter a whole number value\n");
        } // end of if

        else{
            correctInput = 1;
        } // end of else

        //if user types decimal value, line below will discard decmial portion
        while (getchar() != '\n');

    }while (CICheck == 0); // end of do while
}


void discountMaths(){
    //checks for customer input and discounts price accordingly
    if (NCB_qual[numberOfSales] == 'y'){
        float temp;

        temp = 1 - INSURANCE_NCB;
        finalPrice[numberOfSales] =  subTotal * temp ;

        discountTotal[numberOfSales] = subTotal - finalPrice[numberOfSales];
        printf("Total saved: %d\n", discountTotal[numberOfSales]);

        strcpy(data[numberOfSales].NCB , "Yes");
        data[numberOfSales].totalDiscount = discountTotal[numberOfSales];

    }// end of if


    if (NCB_qual[numberOfSales] == 'n'){
        strcpy(data[numberOfSales].NCB , "no");
        finalPrice[numberOfSales] = subTotal;
        data[numberOfSales].totalDiscount = discountTotal[numberOfSales];

    } //end of if

}

void menu_buyCar(){
    clear();

    //calculates the sie of array
    int numberOfCars = sizeof(carPrices) / sizeof(float);
    printf("There are %d types of cars avaliable for purchase\n\n", numberOfCars);

    printf("What is your name?\n");
    scanf("%[^\n]s", &*customerName[numberOfSales]);
    strcpy(data[numberOfSales].customerName, customerName[numberOfSales]);

    newLine();

    clear();
    printf("How old are you?\n");
    getInteger();
    data[numberOfSales].age = customerAge[numberOfSales];

    clear();
    printf("Hello %s, Here are the cars we have avaliable\n\n", customerName[numberOfSales]);

    //uses line 179 to set the length of the loop to print out all values in the array once
    while(1){
        bubbleSort();

        for (int i = 0; i < numberOfCars; i++){
            printf("%d - %s Left in Stock: %d\n", i, carModel[i], carsAvaliable[i]);

        }// end of for

        scanf("%d", &carInBasket);

        if (carInBasket == 0 || carInBasket == 1 || carInBasket == 2){
            break;

        }// end of if

        else{
            printf("Please input 0, 1 or 2\n");

        }// end of else

    } // end of while


    carModelPerSale[numberOfSales] = carInBasket;
    data[numberOfSales].carModelID = carInBasket;


    clear();

    while(1){
        printf("How many %s's do you need?\n", carModel[carInBasket]);
        scanf("%d", &carQuant);


        if (carQuant > carsAvaliable[carInBasket]){
            printf("Sorry we do not have that many cars avaliable\n");
        } // end of if

        if (carQuant <= carsAvaliable[carInBasket]){
            carsAvaliable[carInBasket] -= carQuant;
            data[numberOfSales].quantity = carQuant;
            break;
        } // end of if
    } // end of while



    subTotal = carQuant * (int)carPrices[carInBasket];
    carQuantPerSale[numberOfSales] = carQuant;

    clear();
    printf("Your current subTotal is %2.d\n", subTotal);

    printf("Do you have a NCB? Please enter Y or N.\n");
    scanf("%s", &NCB_qual[numberOfSales]);


    NCB_qual[numberOfSales] = tolower(NCB_qual[numberOfSales]);

    strcpy(data[numberOfSales].NCB, &NCB_qual[numberOfSales]);

    validation();

    //checks for customer input and discounts price accordingly
    discountMaths();

    printf("Your total is: %2.d\nPress 'ENTER' to continue" , finalPrice[numberOfSales]);
    data[numberOfSales].saleAmount = finalPrice[numberOfSales];
    userChoice = getchar(), getchar();


    numberOfSales++;
}

void landingPage(){
    printf("Welcome to the Car Sales office!\n\n");
    printf("Menu:\n");
    printf("a. Buy a car\n");
    printf("b. View Sales \n");
    printf("c. View Stock Levels\n");
    printf("d. View Menu Previous Sales\n");
    printf("x. Exit\n\n");

    printf("Please choose one option from the menu: \n");
    userChoice = getchar(), getchar();

    userChoice = tolower(userChoice);
}


int main(){
    do{
        landingPage();
        switch(userChoice){
            case MENU_OPTION_BUY:
                menu_buyCar();
                fflush(stdout);
                break;

            case MENU_OPTION_VIEW_SALES:
                menu_viewSales();
                fflush(stdout);

                break;

            case MENU_OPTION_VIEW_STOCK:
                menu_viewStock();
                fflush(stdout);
                break;

            case MENU_OPTION_VIEW_PREVIOUS_SALES:
                menu_previousSales();
                fflush(stdout);

                break;


            case MENU_OPTION_EXIT:
                printf("Thank you for using car sales!\n");
                break;


            default:
                printf("Press Enter to return to landingPage");
                fflush(stdout);

                break;

        } // end of switch
    }while(userChoice != MENU_OPTION_EXIT); // end of do
}

