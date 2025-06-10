#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define MAX_COUNTRIES 100


struct Country {
    char name[50];
    int army_stars;
    int navy_stars;
    int air_force_stars;
    float production;
    int treasury;
    int is_colonized;
};

int coins = 100;
float production = 1;

void display_rules() {  
    printf("\t\t\t\tPREGAME INFO\t\t\t\t\n");
    printf("1) You are the ruler of India and you need to conquer the world by colonizing different countries one by one.\n");
    printf("2) You will have to form an army to colonize different countries, which requires coins.\n");
    printf("3) You will keep getting coins from your country and the countries you have colonized.\n");
    printf("4) To colonize a country, the strength of your army, navy, and air force should each be greater than that of the country you want to colonize.\n");
    printf("5) Once you colonize all the countries, you will be the dictator of the world, and the game will end.\n\n");
    printf("\t\t\tDO YOU WANT TO START THE GAME?\n IF YES PRESS 1\t\t\t\n");
}

int set_cost(int troop){
    int costs[5] = {25, 100, 500, 1500, 3000};
    return costs[troop];
}



void upgrade(int *army, int *navy, int *air_force) {  
    int choice;
    printf("Choose what to upgrade:\n");
    int cost;
    cost = set_cost(*army); 
    printf("1) Army Strength, star-%d, cost-%d coins\n", *army, cost);
    cost = set_cost(*navy); 
    printf("2) Navy Strength, star-%d, cost-%d coins\n", *navy, cost);
    cost = set_cost(*air_force);
    printf("3) Air Force Strength, star-%d, cost-%d coins\n", *air_force, cost);
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            if (coins >= set_cost(*army)) {
                coins -= set_cost(*army);
                printf("Upgrade successful! Remaining coins: %d\n", coins);
                (*army)++;
            }
            else {
                printf("Not enough coins to upgrade!\n");
            }
            break;
        case 2:
            if (coins >= set_cost(*navy)) {
                coins -= set_cost(*navy);
                printf("Upgrade successful! Remaining coins: %d\n", coins);
                (*navy)++;
            }
            else {
                printf("Not enough coins to upgrade!\n");
            }
            break;
        case 3:
            if (coins >= set_cost(*air_force)) {
                coins -= set_cost(*air_force);
                printf("Upgrade successful! Remaining coins: %d\n", coins);
                (*air_force)++;
            }
            else {
                printf("Not enough coins to upgrade!\n");
            }
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
    sleep(2);
}

void colonize(struct Country *country, int *army, int *navy, int *air_force, int *colonized) { 
    if (country->is_colonized) {
        printf("%s is already colonized.\n", country->name);
        return;
    }

    if (*army >= country->army_stars && *navy >= country->navy_stars && *air_force >= country->air_force_stars) {
        country->is_colonized = 1;
        coins += 10; 
        printf("You have successfully colonized %s! Coins: %d\n", country->name, coins);
        *army -= country->army_stars;
        *navy -= country->navy_stars;
        *air_force -= country->air_force_stars;
        (*colonized)++;
    }
    else {
        printf("Your forces are not strong enough to colonize %s.\n", country->name);
    }
    sleep(3);
}
//farhankhan pathan
int load_countries_from_csv(struct Country countries[], const char *filename) {  
    FILE *file = fopen("countries.csv", "r");
    if (!file) {
        perror("Error opening CSV file");
        return 0;
    }

    char line[256];
    int count = 0;

   
    fgets(line, sizeof(line), file);  

    while (fgets(line, sizeof(line), file) && count < MAX_COUNTRIES) { 
        char *token = strtok(line, ",");
        if (token == NULL) continue;
        strncpy(countries[count].name, token, sizeof(countries[count].name));
        countries[count].name[sizeof(countries[count].name) - 1] = '\0'; 

        token = strtok(NULL, ",");
        countries[count].army_stars = atoi(token);

        token = strtok(NULL, ",");
        countries[count].navy_stars = atoi(token);

        token = strtok(NULL, ",");
        countries[count].air_force_stars = atoi(token);

        token = strtok(NULL, ",");
        countries[count].production = atof(token);

        countries[count].treasury = 0;
        countries[count].is_colonized = 0;

        count++;
    }

    fclose(file);
    return count;
}

int main() { //this is the main functional code 
    int army_stars = 0;
    int navy_stars = 0;
    int air_force_stars = 0;
    int colonized = 0;
    int start;
    display_rules();

    scanf("%d", &start);
    if (start != 1) {
        printf("Exiting the game.\n");
        return 0;
    }

    struct Country countries[MAX_COUNTRIES];
    int num_countries = load_countries_from_csv(countries, "countries.csv");

    if (num_countries == 0) {
        printf("No countries loaded. Exiting.\n");
        return 1;
    }

    time_t start_time = time(NULL);
    while (1) {
        printf("===================================\n");
        printf("COINS: %d\n", coins);
        printf("Army: %d | Navy: %d | Air Force: %d\n", army_stars, navy_stars, air_force_stars);
        printf("===================================\n\n");

        
        printf("1) Upgrade Strength\n");
        printf("2) Attempt Colonization\n");
        printf("3) Refresh\n");
        printf("4) Exit Game\n\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                upgrade(&army_stars, &navy_stars, &air_force_stars);
                break;
            case 2: {
                printf("Choose a country to colonize:\n");
                for (int i = 0; i < num_countries; i++) {
                    if (!countries[i].is_colonized) {
                        printf("\n%d) %s (Army: %d, Navy: %d, Air Force: %d)\n", i + 1, countries[i].name, countries[i].army_stars, countries[i].navy_stars, countries[i].air_force_stars);
                    }
                }
                int country_choice;
                scanf("%d", &country_choice);

                if (country_choice >= 1 && country_choice <= num_countries) {
                    colonize(&countries[country_choice - 1], &army_stars, &navy_stars, &air_force_stars, &colonized);
                }
                else {
                    printf("Invalid choice.\n");
                }
                break;
            }
            case 3:
                break;
            case 4:
                printf("Exiting game. See you next time!\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }

        if (time(NULL) - start_time > 1) {
            coins = coins + floor(time(NULL) - start_time) * production;
            int colonized_money = 0;
            for (int i = 0; i < num_countries; i++) {
                if (countries[i].is_colonized) {
                    colonized_money += floor(time(NULL) - start_time) * countries[i].production;
                }
                else {
                    countries[i].treasury += floor(time(NULL) - start_time) * countries[i].production;
                }
            }
            printf("All colonized countries gave %d coins\n", colonized_money);
            coins += colonized_money;
            start_time = time(NULL);
        }

        printf("Countries colonized: %d\n", colonized);
        if (colonized > 49) {
            printf("Congratulations, you are the dictator of the world!!!\n");
            return 0;
        }
    }
}
