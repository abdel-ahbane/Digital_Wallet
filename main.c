/******************************************************************************
 This project is made as a HomeWork requirement for the course "Programming 1" in the Fall term of 2022 at the Budapest University of Technology and Economics.
 Credits earned from the subject : 7 (ESCS)
 Final grade : Excelent 5/5
 By Abdelhamid Ahbane, Electrical Engineering student at BME, Hungary.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int year, month, day;
}date;

typedef struct{
    date time; 
    int type;// 0 = expenses    ;      1 = income;
    int category; //index of the string in the categories 2d array
    float amount;
}entry_data;


typedef struct{
    int length; // number of entries
    entry_data *pocket; // entries array of sructures
}set;

date enter_date(void);
int enter_type(void);
float enter_amount(void);
int choose_category(char ***strings, int total);

//add new entry
entry_data* add_entry(char ***strings, int total);
void add(set *a, entry_data *input);

//print menu
void print_menu( int* option, char*** p_categories_list, int* cat_len, set* wallet1, entry_data *input);

//add a new category to the list
void add_new_category(char ***strings, int *p_total);

//view statistics
void view_statistics(char*** p_categories_list, int* cat_len, set* wallet1 );


int main()
{   
    printf("-----------------------------------------------------------------------------------\n\n");
    fflush(stdin);


    /***Load the list of categories from the file***/

    //Load the number of categories
    int cat_len=0;  //default number of available categories
    int *p_cat_len = &cat_len;
    FILE *fptr_cat_len = NULL;
    fptr_cat_len = fopen("number_of_categories.txt", "r");
    if( fptr_cat_len == NULL){
        printf("Couldn't load number of categories file!!");
        exit(1);
    }
    fscanf(fptr_cat_len, "%d", &cat_len);
    fclose(fptr_cat_len);

    //Load the list of categories strings
    char **categories_list=(char**)malloc( (*p_cat_len)*sizeof(char*) );
    char*** p_categories_list = &categories_list;
    for(int i=0; i<(*p_cat_len); i=i+1)
        categories_list[i]=(char*)malloc(100*sizeof(char));
    
    FILE *fptr_categories_list = NULL;
    fptr_categories_list = fopen("categories.txt", "r");
    if( fptr_categories_list == NULL ){
        printf("Couldn't open list of categories file!!!");
        exit(1);
    }

    for(int i=0; i<cat_len; i++){
        fgets(categories_list[i], 100, fptr_categories_list);
    }
    fclose(fptr_categories_list);
    
    

    /*   Loading the wallet pockets from the file   */

    set *wallet1 = (set*)malloc( sizeof(set) );
    //loading the length of pockets in the wallet
    wallet1->length = 0;  //empty wallet
    wallet1->pocket = NULL;
    FILE *fptr_wallet_length = NULL;
    fptr_wallet_length = fopen("number_of_pockets.txt", "r");
    if( fptr_wallet_length == NULL){
        printf("Couldn't open number of pockets file!!");
        exit(1);
    }
    fscanf(fptr_wallet_length, "%d", &(wallet1->length));
    fclose(fptr_wallet_length);
    //load the wallet pockets
    FILE *fptr_pockets = NULL;
    fptr_pockets = fopen("pockets.txt", "r");
    if( fptr_pockets == NULL){
        printf("Couldn't open pocketss file!!");
        exit(1);
    }
    int len = (wallet1->length);
    wallet1->pocket = (entry_data*)malloc(len*sizeof(entry_data));
    for(int i=0; i<len; i++){
        fscanf(fptr_pockets, "%d,%d,%d,%d,%d,%f", &(wallet1->pocket[i]).time.year, &(wallet1->pocket[i]).time.month, &(wallet1->pocket[i]).time.day, &(wallet1->pocket[i]).type, &(wallet1->pocket[i]).category, &(wallet1->pocket[i]).amount);
    }
    fclose(fptr_pockets);



    
    entry_data* input = (entry_data*)malloc(sizeof(entry_data));
    
    /*   new input   */
    int option=0;
    int *p_option = &option;

    while((*p_option)!=4){
        fflush(stdin);
        print_menu( p_option, p_categories_list, p_cat_len, wallet1, input);

    }

    

    /****Save categories into a file****/
    //save number of catrgories
    fptr_cat_len = fopen("number_of_categories.txt", "w");
    if( fptr_cat_len == NULL){
        printf("Couldn't open number of categories file!!");
        exit(1);
    }
    fprintf(fptr_cat_len, "%d", cat_len);
    fclose(fptr_cat_len);
    //save the list of categories strings
    fptr_categories_list = fopen("categories.txt", "w");
    if( fptr_categories_list == NULL ){
        printf("Couldn't open list of categories file!!!");
        exit(1);
    }

    for(int i=0; i<cat_len; i++){
        fputs(categories_list[i], fptr_categories_list);
    }
    fclose(fptr_categories_list);


    /*   save the wallet pockets into the file   */

    //save the length of wallet
    fptr_wallet_length = fopen("number_of_pockets.txt", "w");
    if( fptr_wallet_length == NULL){
        printf("Couldn't open number of pockets file!!");
        exit(1);
    }
    fprintf(fptr_wallet_length, "%d", wallet1->length );
    fclose(fptr_wallet_length);

    //save the wallet pockets
    fptr_pockets = fopen("pockets.txt", "w");
    if( fptr_pockets == NULL){
        printf("Couldn't open pockets file!!");
        exit(1);
    }
    for(int m=0; m < (wallet1->length) ; m++){
        fprintf(fptr_pockets, "%d,%d,%d,%d,%d,%f\n", (wallet1->pocket[m]).time.year, (wallet1->pocket[m]).time.month, (wallet1->pocket[m]).time.day, (wallet1->pocket[m]).type, (wallet1->pocket[m]).category, (wallet1->pocket[m]).amount);
    }
    fclose(fptr_pockets);
    
    
    

    for(int i=0; i<(*p_cat_len); i=i+1)
        free(categories_list[i]);
    free(categories_list);

    free(input);

    free( wallet1->pocket );
    free(wallet1);
    return 0;
}







/********              add date function              ********/
// function to add year month and day of the entry
date enter_date(void){
    date t;
    printf("Select date (format : year month day ) :\n");
    scanf("%d %d %d", &t.year, &t.month, &t.day);
    return t;
}



/********              add type function              ********/
// function to add type of the entry (income = 1 , expenses = 0)
int enter_type(void){
    int n=0;
    printf("Select type :\n\t1 : income --\t0 : expenses  : \n");
    scanf("%d",&n);
    if (n == 1)
        return 1;
    else if(n == 0)
        return 0;
    else{
        printf("invalid type\n");
        return -1;
    }
}


/********              add amount of money function              ********/
// function to add value of money in euros to the entry
float enter_amount(void){
    float f;
    printf("Enter the amount of money in Euros :\n");
    scanf("%f",&f);
    return f;
}


/********              add category of input function              ********/
// function to add the category string to the entry
int choose_category(char ***strings, int total){
    printf ("select a category from the list below :\n");
    int i=0;
    for(int i=0; i<total; i++)
        printf("%d = %s\n",i , (*strings)[i] );
    
    scanf("%d",&i);
    return i;
}


/********              add one new entry to the wallet1 ()new pocket            ********/
void add(set *a, entry_data *input){ //pointer to the new entry is used !!
    int l = (a->length);
    entry_data* new=(entry_data*)malloc( (l+1)*sizeof(entry_data) );
    for(int i=0; i<l; i++){
        new[i].time.year = (a->pocket[i]).time.year;
        new[i].time.month = (a->pocket[i]).time.month;
        new[i].time.day = (a->pocket[i]).time.day;
        new[i].type = (a->pocket[i]).type;
        new[i].category = (a->pocket[i]).category;
        new[i].amount = (a->pocket[i]).amount;
    }   
    
    new[l].time.year = (*input).time.year;
    new[l].time.month = (*input).time.month;
    new[l].time.day = (*input).time.day;
    new[l].type = (*input).type;
    new[l].category = (*input).category ;
    new[l].amount = (*input).amount ;
    
    
    free( a->pocket );
    
    (a->length) = l+1;
    (a->pocket) = new;
}



/********              print menu function              ********/
// function to dislay the menu options and the general income and expenses information
void print_menu( int* option, char*** p_categories_list , int* cat_len, set* wallet1, entry_data *input) {
    printf("                                    <----------------------------------------------\n ");
    printf("                                   <\n  ");
    printf("                                  <  WELCOME TO YOUR DIGITAL WALLET      \n ");
    printf("                                   <\n  ");
    printf("                                  <_______________________________________________\n\n\n");
    printf("1.Add Entry (income or expenses) \n");    // User can add a category later here as well
    printf("2.Add a New Category \n");
    printf("3.View statistics / Check the most expensive category \n");
    printf("4.EXIT\n");
    printf("\n\n Choose an option from the menu above:  ");

    scanf("%d", option);

    //to handle errors relating to non valid option selection
    fflush(stdin);
    if( (*option != 1) && (*option != 2) && (*option != 3) && (*option != 4) ){
        *option = 5;
    };


    switch (*option) {
        case 1: //Add Entry (Income or expenses)
            input = add_entry(p_categories_list , *cat_len);
            add(wallet1, input);
            printf("\n New entry saved successfully !\n");
            break;
        case 2: //Add a New Category
            add_new_category( p_categories_list, cat_len);
            break;
        case 3: //View statistics for each Category in a selected Time Period
            view_statistics(p_categories_list, cat_len, wallet1 );
            break;
        case 4: //Exit
            break;
        case 5: 
            printf("\n !!!!!\t Please, choose a valid option from the menu ( an integer from 1 to 4)\t !!!!!!!\n");
            break;
    }

}



/********              add income function              ********/
// function to add income money to the wallet
entry_data* add_entry(char ***strings, int total) {
    
    entry_data *new_entry = (entry_data*)malloc(sizeof(entry_data));
    //maybe allocate time as well !
    new_entry->time = enter_date();

    new_entry->type = enter_type();

    new_entry->category = choose_category(strings, total);

    new_entry->amount = enter_amount();
    
    return new_entry;   //should we free new_entry here and then change adress of recipient?
}




/********              add a new category function              ********/
// function to add a new category to the categories array
void add_new_category(char ***strings, int *p_total){
    
    
    char** n_strings = (char**)malloc(((*p_total)+1)*sizeof(char*));
    for(int i=0; i <= (*p_total); i++)
        n_strings[i] = (char*)malloc(100*sizeof(char));
    
    for(int j=0; j<(*p_total); j++)
        strcpy( n_strings[j], (*strings)[j] );
        
    printf("Enter name of the new category to add :\n");
    
    fflush(stdin);
    
    fgets( n_strings[ (*p_total) ], 100, stdin );
    
    //free old strings and its string elements
    for(int k=0; k < (*p_total); k++)
        free( (*strings)[k] );
    free(*strings);
    
    (*p_total) = (*p_total) + 1;
    *strings = n_strings;

    printf("\nNew category has been added successfully\n");
}



/********              show statistics function              ********/
// function to ashow statistics for each category

void view_statistics(char*** p_categories_list, int* cat_len, set* wallet1 ){
    /*
        for a user-selected time period (the user may enter the start and end date of
        the period) display statistics for each category (total spent, total income,
        largest expense).
    */
    printf("Starting from :\n");
    date starting = enter_date();
    printf("Ending at :\n");
    date ending = enter_date();

    int i, j;
    float total_income, total_expenses;
    float *most_expensive = (float*)malloc( (*cat_len)*sizeof(float) );

    for(j=0; j< (*cat_len) ; j++){
        total_income = 0;
        total_expenses = 0;
        i = 0;
        for(i; i < (wallet1->length) ; i++ ){

            if( (wallet1->pocket[i].category == j ) ){
                //between years
                if( (wallet1->pocket[i].time.year > starting.year)  && (wallet1->pocket[i].time.year < ending.year) ){
                    if(wallet1->pocket[i].type == 1){
                        total_income += wallet1->pocket[i].amount;
                    }
                    else if(wallet1->pocket[i].type == 0){
                        total_expenses += wallet1->pocket[i].amount;
                    }
                }
                //same year (the starting year)
                else if( (wallet1->pocket[i].time.year == starting.year) ){
                    //starting year...between months
                    if( (wallet1->pocket[i].time.month > starting.month) ){
                        if(wallet1->pocket[i].type == 1){
                            total_income += wallet1->pocket[i].amount;
                        }
                        else if(wallet1->pocket[i].type == 0){
                            total_expenses += wallet1->pocket[i].amount;
                        }
                     }
                    //starting year & same month
                    else if( (wallet1->pocket[i].time.month == starting.month) ){
                        // starting year, month, between days
                        if( (wallet1->pocket[i].time.day >= starting.day) ){
                                if(wallet1->pocket[i].type == 1){
                                    total_income += wallet1->pocket[i].amount;
                                }  
                                else if(wallet1->pocket[i].type == 0){
                                    total_expenses += wallet1->pocket[i].amount;
                                }
                         }
                        
                    }
                }

                //same year (the ending year)
                else if( (wallet1->pocket[i].time.year == ending.year) ){
                    //ending year...between months
                    if( (wallet1->pocket[i].time.month < ending.month) ){
                        if(wallet1->pocket[i].type == 1){
                            total_income += wallet1->pocket[i].amount;
                        }
                        else if(wallet1->pocket[i].type == 0){
                            total_expenses += wallet1->pocket[i].amount;
                        }
                     }
                    //ending year & same month
                    else if( (wallet1->pocket[i].time.month == ending.month) ){
                        // ending year, month, between days
                        if( (wallet1->pocket[i].time.day <= ending.day) ){
                                if(wallet1->pocket[i].type == 1){
                                    total_income += wallet1->pocket[i].amount;
                                }  
                                else if(wallet1->pocket[i].type == 0){
                                    total_expenses += wallet1->pocket[i].amount;
                                }
                         }
                        
                    }
                }
                        
            }

        }

        most_expensive[j] = total_expenses;
        printf("Category : %s\n", (*p_categories_list)[j]);
        printf("Total income = %f\n", total_income);
        printf("Total expenses = %f\n", total_expenses);
        printf("---------\n");
    }

    int max_index = 0;
    int e = 0;
    for (e; e < (*cat_len); e++)
    {
        if( most_expensive[e] >= most_expensive[max_index] ){
            max_index = e;
        }
    }
    
    //to avoid trivial case
    if( most_expensive[max_index] != 0 ){
        printf("\n--The most Expensive category is : \t%s\n", (*p_categories_list)[max_index]);
        printf("-|-->it's expenses are :%f\n",most_expensive[max_index] );
    }
    

    free(most_expensive);
}

