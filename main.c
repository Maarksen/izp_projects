#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_ARG 3               //minimaly pocet argumentov ktory staci zadat aby program fungoval
#define MIN_LVL 1               //najmensia hodnota argumentu 'level' je 1
#define MAX_LVL 4               //maximala hodnota argumentu 'level' je 4
#define MIN_PARAM 1             //hodnota argumentu param musi vzdy byt nezaporna a vacsia ako 0
#define PARAM_3 3               //argument 'param' v hodnote 3
#define PARAM_4 4               //argument 'param' v hodnote 4
#define ASCII 126                   //pocet vsetkych znakov v ascii tabulke
#define RIGHT_STATS_LENGTH 7        //jedina spravna dlzka 4. parametru
#define STERTER_MIN 100             //konstanta ktora vyjadruje zaciatocnu hodnotu minimalnej dlzky passwords, od ktorej odpocitavame
#define MAX_PASSWORD_LENGTH 101     //maximalna dlzka ktoru heslo moze mat
#define LEVEL_3 3                   //argument 'level' v hodnote 3
#define LEVEL_4 4                   //argument 'level' v hodnote 4

//prototypy funkcii na kontrolu argumentov
bool enough_arguments(int argc);
bool is_level_correct(unsigned level);
bool is_param_correct(long long param);
bool checking_for_param(char *passwords);
bool param_length_ok(char *argv[]);

//prototypy funkcii na kontrolu volitelneho argumentu --stats
bool is_stats_correct(char *stats, char *correct_stats, unsigned stats_length);
bool are_strings_equal(char *stats, char *correct_stats, unsigned stats_length);
int stringlength(char *stats);
int strlength(char *string);

//prototypy funkcii na kontrolu hesla
bool containslowercase(char *passwords);
bool containsuppercase(char *passwords);
bool containsnumber(char *passwords);
bool containssymbol(char *passwords);
void final_password(char *passwords, long long param, unsigned level, unsigned password_length);

//prototypy funkcii na pocitanie statistik
int num_of_characters_password(char *passwords, unsigned passwords_sum, unsigned passwords_count);
int num_of_unique_chars(char *passwords, char *unique_chars, unsigned unique_chars_count);
void statistics(unsigned unique_chars_count, unsigned min_length, double average_length);

//prototypy funkcii na kontolu argumentu 'level' v hodnote 3 a 4
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
    unsigned min_length = STERTER_MIN;
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

//funkcia kontroluje či je parameter 'level' spravne zadany
bool is_level_correct(unsigned level){
    if(MIN_LVL <= level && level <= MAX_LVL){
        return true;
    }
    else{
        fprintf(stderr,"argument 'level' has to be 1-4");
        return false;
    }
}

//funkcia zistujuca dlzku stringu
int strlength(char *string){
    int length = 0;
    for(int i = 0; string[i] != '\0'; i++){
        length++;
    }
    return length;
}

//funkcia kontroluje či je parameter 'param' spravne zadany
bool is_param_correct(long long param){
    if(param >= MIN_PARAM){
        return true;
    }
    else{
        fprintf(stderr,"argument 'param' has to be >1");
        return false;
    }
}

//funkcia zistuje dlzku zadaneho stringu
int stringlength(char *stats){
    unsigned stats_length = 0;
    for(int i = 0; stats[i] != '\0'; i++){
        stats_length++;
    }
    return stats_length;
}

//funkcia kontroluje ci sa dlzka parametru zhoduje s pozadovanou dlzkou
bool are_strings_equal(char *stats, char *correct_stats, unsigned stats_length){
    for(int i = 0; stats[i] != '\0' || correct_stats[i] != '\0'; i++){
            if(stats_length != RIGHT_STATS_LENGTH || stats[i] != correct_stats[i]){
                return false;
            }
            else{                       //ak zisti ze sa dlzky stringov nerovnaju alebo najde 2 charaktery ktore sa nerovnaju vrati false
                return true;
            }
    }
    return true;
}

//funkcia kontroluje ci je treti, volitelny parameter dobre zadany
bool is_stats_correct(char *stats, char *correct_stats, unsigned stats_length){
    if(are_strings_equal(stats, correct_stats, stats_length) == true){
        return true;
    }
    else{
        fprintf(stderr,"optional argument: wrong input");
        return false;
    }
}

//funkcia kontroluje ci je dostatok argumentov
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

//funkcia pocita pocet znakov kazdeho passwords
int num_of_characters_password(char *passwords, unsigned passwords_sum, unsigned passwords_count){
    int password_length = 0;
    for(int i = 0; passwords[i] != '\0'; i++){              //pocita dlzku jednotliveho hesla a zaroven spocitava znaky vsetkych hesiel
        password_length++;
        passwords_sum++;
    }
    passwords_count++;                      //po kazdom zavolani funkcie 1 k poctu hesiel
    return password_length;
}

//funkcia zistujuca ci heslo obsahuje aspon jedno velke pismeno
bool containsuppercase(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if(passwords[i] >= 'A' && passwords[i] <= 'Z'){
            return true;
        }
    }
    return false;
}

//funkcia zistujuca ci heslo obsahuje aspon jedno male pismeno
bool containslowercase(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if(passwords[i]>= 'a' && passwords[i]<= 'z'){
            return true;
        }
    }
    return false;
}

//funkcia kontroluje obsah pozadovanych parametrov pri zadanom parametri 3
bool checking_for_param(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if((passwords[i] >= 32 && passwords[i] <= 47) || (passwords[i] >= 58 && passwords[i] <= 64) || (passwords[i] >= 91 && passwords[i] <= 96) || (passwords[i] >= 123 && passwords[i] <= 126)
           || (passwords[i] >= '0' && passwords[i] <= '9')){
            return true;
        }
    }
    return false;
}

//funkcia zistujuca ci heslo obsahuje aspon jednu cislicu
bool containsnumber(char *passwords){
    for(int i = 0; passwords[i] != '\0'; i++){
        if(passwords[i] >= '0' && passwords[i] <= '9'){
            return true;
        }
    }
    return false;
}

//funkcia zistujuca ci heslo obsahuje aspon jedno znak
bool containssymbol(char *passwords){
    for(int i = 0; passwords[i] != '\0';i++){
        if((passwords[i] >= 32 && passwords[i] <= 47) || (passwords[i] >= 58 && passwords[i] <= 64) || (passwords[i] >= 91 && passwords[i] <= 96) || (passwords[i] >= 123 && passwords[i] <= 126)){
            return true;
        }
    }
    return false;
}

//funkcia, ktora zisti pocet roznych charakterov
int num_of_unique_chars(char *passwords, char *unique_chars, unsigned unique_chars_count){
    bool found;
    for(int i = 0; passwords[i] != '\0'; i++){      // "heslo" "ahoj"
            found = false;
        for(int j = 0; unique_chars[j] != '\0'; j++){
            if(passwords[i] == unique_chars[j]){            //ak sa znaky ktore porovnavame rovnaju breakne funkciu a ide na dalsi znak
                found = true;
                break;
            }
        }
            if (found == false) {
                unique_chars_count++;                               //pripocita +1 k roznym charakterom
                unique_chars[unique_chars_count] = passwords[i];    //pripise znak do pola s charaktermi ktore sme uz presli 
            }
        }

    return unique_chars_count;
}

//funkcia ktora kontroluje ci sa opakuje X znakov po sebe
bool sequence_of_chars(char *passwords, long long param){
    for(int i = 0; passwords[i] != '\0'; i++){
        if((passwords[i] == passwords[i+1]) && param == 2){         //pre zadany parameter 2 nemusi ani prechadzat cyklom ak najde 2 rovnake charaktery po sebe
            return false;                                          
        }
        else if(passwords[i] == passwords[i+1]){                    //hlada rovnake znaky, ktore nasleduju hned za sebou
            for(int j = 0; j < param; j++){
                if(passwords[i] != passwords[i+j]){             //pokial sa rovnaju nic sa nedeje len sa pripocitava j
                    break;
                }
                else if((j+1) == param){                        //ak sa j+1 dostane na param vtedy je podmienka splnena a v hesle sa nechadza taky retazec 
                    return false;
                }
            }
        }
    }
    return true;
}

//funkcia ktora hlada dva rovnake podretazce aspon dlzky X
bool substring_of_chars(char *passwords, long long param, unsigned password_length){
    for(unsigned int i = 0; passwords[i] != '\0'; i++){
        for(unsigned int j = 0; j < password_length; j++){              //pre kazdy znak hesla prechadza kazdym znakom toho isteho hesla
            if((passwords[i] == passwords[j]) && (i != j)){             //porovnava ich a pokracuje len ked sa rovnaju a zaroven niesu an rovnakom indexe
                for(int k = 1; k < param; k++){
                    if(passwords[i+k] != passwords[j+k]){               //ked sa nasli 2 rovnake charaktery tak potrovnava index prveho znaku +1
                        break;                                          //a index drhueho znaku co sa rovnaju +1 
                    }
                    else if((k) == param-1){            //porovnava az dokym index nepride po zadany param-1 a vtedy je podmienka splnena
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

//funkcia ktora zavola funkcie na kontrolu hesiel
void final_password(char *passwords, long long param, unsigned level, unsigned password_length){
    if(containsuppercase(passwords) == false){
        }
    else if(containslowercase(passwords) == false){
        }
    else if(level >= 2 && param == PARAM_3 && checking_for_param(passwords) == false){
        }
    else if(level >= 2 && param == PARAM_4 && containsnumber(passwords) == false){
        }
    else if(level >= 2 && param == PARAM_4 && containssymbol(passwords) == false){
        }
    else if(param > PARAM_4){
        }
    else if((level == LEVEL_3 || level == LEVEL_4) && sequence_of_chars(passwords, param) == false){
        }
    else if(level > 2 && param == 1){
        }
    else if(level == LEVEL_4 && substring_of_chars(passwords, param, password_length) == false){
        }
    else{
        printf("%s", passwords);            //ak sa ziadna s tych podmienok nestane vypise heslo
        }
}

//funkcia ktora vypisuje statistiky
void statistics(unsigned unique_chars_count, unsigned min_length, double average_length){
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", unique_chars_count-1);
    printf("Minimalni delka: %d\n", min_length);
    printf("Prumerna delka: %.1f\n", average_length);
}

//funkcia ktora kontoluje dlzu parametru
bool param_length_ok(char *argv[]){
    if(strlength(argv[2]) != 1){
        for(int i = 0; argv[2][i] != '\0'; i++){                //kontroluje ci su vsetky charaktery v 'param' cisla
            if(!(argv[2][i] >= '0' && argv[2][i] <= '9')){
                fprintf(stderr,"Argument 'param': wrong input");
                return false;
            }
        }
    }
    return true;
}
