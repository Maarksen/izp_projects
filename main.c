#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_ARG 3               //minimal number of arguments for  the program to work
#define MIN_LVL 1               //the lowest value of argument 'level' is 1
#define MAX_LVL 4               //the highest value of argument 'level' is 4
#define MIN_PARAM 1             //the value of argument 'param' has to be positive and over 0 at all times
#define PARAM_3 3               //argument 'param' value 3
#define PARAM_4 4               //argument 'param' value 4
#define LEVEL_2 2                   //argument 'level' value 2
#define LEVEL_3 3                   //argument 'level' value 3
#define LEVEL_4 4                   //argument 'level' value 4
#define RIGHT_STATS_LENGTH 7        //the only right length of the 4th parameter
#define ASCII 126                   //number of all the characters in the ASCII table
#define STARTER_MIN 100             //constant which holds the starting value of the minimal length of password
#define MAX_PASSWORD_LENGTH 101     //the max length a password can have


//prototypes of functions for checking the arguments
bool enough_arguments(int argc);
bool is_level_correct(unsigned level);
bool is_param_correct(long long param);
bool checking_for_param(char *passwords);
bool param_length_ok(char *argv[]);

//prototypes of functions for validation of the optional argument --stats
bool is_stats_correct(char *stats, char *correct_stats, unsigned stats_length);
bool are_strings_equal(char *stats, char *correct_stats, unsigned stats_length);
int stringlength(char *stats);
int strlength(char *string);

//prototypes of functions for checking the passwords
bool containslowercase(char *passwords);
bool containsuppercase(char *passwords);
bool containsnumber(char *passwords);
bool containssymbol(char *passwords);
void final_password(char *passwords, long long param, unsigned level, unsigned password_length);

//prototypes of functions for calculating statistics
int num_of_characters_password(char *passwords, unsigned passwords_sum, unsigned passwords_count);
int num_of_unique_chars(char *passwords, char *unique_chars, unsigned unique_chars_count);
void statistics(unsigned unique_chars_count, unsigned min_length, double average_length);

//prototypes of functions for validation of the argument 'level' in value 3 and 4
bool sequence_of_chars(char *passwords, long long param);
bool substring_of_chars(char *passwords, long long param, unsigned password_length);


int main(int argc, char *argv[]){

    if(enough_arguments(argc) == false){
        return 1;
    }
    if(strlength(argv[1]) != 1){
        fprintf(stderr,"Argument 'level': wrong input");
        return 1;
    }
    if(param_length_ok(argv) == false){
        return 1;
    }

    unsigned stats_length;
    unsigned level = atoi(argv[1]);
    unsigned password_length=0;
    unsigned passwords_sum = 0;
    unsigned unique_chars_count = 0;
    double passwords_count = 0;
    double average_length = 0;
    unsigned min_length = STARTER_MIN;
    long long param = atoi(argv[2]);
    char *stats = argv[3];
    char correct_stats[7] = "--stats";
    char unique_chars[ASCII];
    char passwords[103];

    if(is_level_correct(level) == false){
        return 1;
    }
    if(is_param_correct(param) == false){
        return 1;
    }
    if(argc > MIN_ARG){
        stats_length = stringlength(stats);
        if(is_stats_correct(stats, correct_stats, stats_length) == false){
            return 1;
        }
    }

    while (fgets(passwords, 103, stdin)) {

        passwords_count++;
        password_length = num_of_characters_password(passwords, passwords_sum, passwords_count);
        if(password_length > MAX_PASSWORD_LENGTH){
            fprintf(stderr,"password too long");
            return 1;
        }

        if(argc > MIN_ARG){

            if(password_length < min_length){
                min_length = password_length-1;
            }
            for(int i = 0; passwords[i] != '\0'; i++){
                if(passwords[i] == '\n' || passwords[i] == EOF){

                }
            passwords_sum++;
            }

            passwords_sum  = passwords_sum-1;
            average_length = passwords_sum / passwords_count;

            unique_chars_count = num_of_unique_chars(passwords, unique_chars, unique_chars_count);
        }

        final_password(passwords, param, level, password_length);
    }

    if(argc > MIN_ARG){
        statistics(unique_chars_count, min_length, average_length);
    }

    return 0;
}

//function checks whether the argument 'level' is correct
bool is_level_correct(unsigned level){
    if(MIN_LVL <= level && level <= MAX_LVL){
        return true;
    }
    else{
        fprintf(stderr,"argument 'level' has to be 1-4");
        return false;
    }
}

//function gets the length of a given string
int strlength(char *string){
    int length = 0;
    for(int i = 0; string[i] != '\0'; i++){
        length++;
    }
    return length;
}

//function checks whether the argument 'param' is correct
bool is_param_correct(long long param){
    if(param >= MIN_PARAM){                 //argument 'param' has to be higher of equal to 1
        return true;
    }
    else{
        fprintf(stderr,"argument 'param' has to be >= 1");
        return false;
    }
}

//function gets the length of a given string
int stringlength(char *stats){
    unsigned stats_length = 0;
    for(int i = 0; stats[i] != '\0'; i++){
        stats_length++;
    }
    return stats_length;
}

//function checks whether the length of a parameter is equal to the desired length
bool are_strings_equal(char *stats, char *correct_stats, unsigned stats_length){
    for(int i = 0; stats[i] != '\0' || correct_stats[i] != '\0'; i++){
            if(stats_length != RIGHT_STATS_LENGTH || stats[i] != correct_stats[i]){
                return false;
            }
            else{                       //if it finds that the lengths dont match of finds two non matching characters returns false
                return true;
            }
    }
    return true;
}

//function checks whether the third, optional character is correct
bool is_stats_correct(char *stats, char *correct_stats, unsigned stats_length){
    if(are_strings_equal(stats, correct_stats, stats_length) == true){
        return true;
    }
    else{
        fprintf(stderr,"optional argument: wrong input");
        return false;
    }
}

//function checks wheter there is enough arguments
bool enough_arguments(int argc){
    if(argc < MIN_ARG){
        fprintf(stderr,"not enough arguments");
        return false;
    }
    else if(argc > MIN_ARG+1){
        fprintf(stderr,"too many arguments");
        return false;
    }
    return true;
}

//function that counts the number of characters of each password
int num_of_characters_password(char *passwords, unsigned passwords_sum, unsigned passwords_count){
    int password_length = 0;
    for(int i = 0; passwords[i] != '\0'; i++){              //counts the length of each individual password
        password_length++;                                  //and at the same time counts all the characters of each password
        passwords_sum++;
    }
    passwords_count++;                      //after every call of the function adds +1 to number of passwords
    return password_length;
}

//function check whether the password contains an uppercase letter
bool containsuppercase(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if(passwords[i] >= 'A' && passwords[i] <= 'Z'){
            return true;
        }
    }
    return false;
}

//function check whether the password contains an lowercase letter
bool containslowercase(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if(passwords[i]>= 'a' && passwords[i]<= 'z'){
            return true;
        }
    }
    return false;
}

//function check whether the password contains a nunmber
bool containsnumber(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if(passwords[i] >= '0' && passwords[i] <= '9'){
            return true;
        }
    }
    return false;
}

//function check whether the password contains a special character
bool containssymbol(char *passwords){
    for(int i = 0; passwords[i] != '\0';i++){
        if((passwords[i] >= 32 && passwords[i] <= 47) || (passwords[i] >= 58 && passwords[i] <= 64) || (passwords[i] >= 91 && passwords[i] <= 96) || (passwords[i] >= 123 && passwords[i] <= ASCII)){
            return true;
        }
    }
    return false;
}

//funkcia kontroluje obsah pozadovanych parametrov pri zadanom parametri 3
bool checking_for_param(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if((passwords[i] >= 32 && passwords[i] <= 47) || (passwords[i] >= 58 && passwords[i] <= 64) || (passwords[i] >= 91 && passwords[i] <= 96) || (passwords[i] >= 123 && passwords[i] <= ASCII)
           || (passwords[i] >= '0' && passwords[i] <= '9')){
            return true;
        }
    }
    return false;
}

//function which gets the number of unique characters
int num_of_unique_chars(char *passwords, char *unique_chars, unsigned unique_chars_count){
    bool found;
    for(int i = 0; passwords[i] != '\0'; i++){      // "heslo" "ahoj"
            found = false;
        for(int j = 0; unique_chars[j] != '\0'; j++){
            if(passwords[i] == unique_chars[j]){            //if the characters that we are comparing are equal breaks function and goes to the next character
                found = true;
                break;
            }
        }
        if(found == false) {
            unique_chars[unique_chars_count] = passwords[i];    //adds the characer to the array of characters which we already checked 
            unique_chars_count++;                               //adds +1 to unique characters
        }
    }

    return unique_chars_count;
}

//function that checks whether there are are X repeating characters
bool sequence_of_chars(char *passwords, long long param){
    for(int i = 0; passwords[i] != '\0'; i++){
        if((passwords[i] == passwords[i+1]) && param == 2){         //for param 2 doesnt even have to go thru the cycle if it finds 2 of the same repeating characters
            return false;                                          
        }
        else if(passwords[i] == passwords[i+1]){                    //looks for repeating characters one right after the other
            for(int j = 0; j < param; j++){
                if(passwords[i] != passwords[i+j]){             //if they are equal nothing happens we only do ++j
                    break;
                }
                else if((j+1) == param){                        //if j+1 gets to the value of param then and only then the condtion is met 
                    return false;                               //and there is a substring of the same characters at least of the length of X
                }
            }
        }
    }
    return true;
}

//function which is looking for two of the same substrings of length X
bool substring_of_chars(char *passwords, long long param, unsigned password_length){
    for(unsigned int i = 0; passwords[i] != '\0'; i++){
        for(unsigned int j = 0; j < password_length; j++){              //for every characters of the passwords, goes thru every character of the same password
            if((passwords[i] == passwords[j]) && (i != j)){             //compares them and cotinues only if they are equal and  arent at the same index at the same time
                for(int k = 1; k < param; k++){
                    if(passwords[i+k] != passwords[j+k]){               //if we found two of the same characters we compare index of the first character +1
                        break;                                          //and the index of the second of the same characters +1 
                    }
                    else if((k) == param-1){            //compares until the index is equal to param -1 and then the condition is met
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

//function which calls the functions for validation of passwords
void final_password(char *passwords, long long param, unsigned level, unsigned password_length){

    if(level == LEVEL_2 && param > PARAM_4){
        param = PARAM_4;
    }

    if(containsuppercase(passwords) == false){
        }
    else if(containslowercase(passwords) == false){
        }
    else if(level >= LEVEL_2 && param == PARAM_3 && checking_for_param(passwords) == false){
        }
    else if(level >= LEVEL_2 && param == PARAM_4 && containsnumber(passwords) == false){
        }
    else if(level >= LEVEL_2 && param == PARAM_4 && containssymbol(passwords) == false){
        }
    else if((level == LEVEL_3 || level == LEVEL_4) && sequence_of_chars(passwords, param) == false){
        }
    else if(level > LEVEL_2 && param == 1){
        }
    else if(level == LEVEL_4 && substring_of_chars(passwords, param, password_length) == false){
        }
    else{
        printf("%s", passwords);            //if all of the upper conditions are not met than it prints the password
        }
}

//function that prints the statistics
void statistics(unsigned unique_chars_count, unsigned min_length, double average_length){
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", unique_chars_count-1);
    printf("Minimalni delka: %d\n", min_length);
    printf("Prumerna delka: %.1f\n", average_length);
}

//function that checks the length of the argument
bool param_length_ok(char *argv[]){
    if(strlength(argv[2]) != 1){
        for(int i = 0; argv[2][i] != '\0'; i++){                //checks whether all of the characters of 'param' are numbers
            if(!(argv[2][i] >= '0' && argv[2][i] <= '9')){
                fprintf(stderr,"Argument 'param': wrong input");
                return false;
            }
        }
    }
    return true;
}
