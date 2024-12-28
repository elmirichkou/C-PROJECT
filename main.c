#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
    char type[50];
    float value;
    float emission_factor;
} Activity;


void addActivity(Activity **activities, int *count);
void removeActivity(Activity **activities, int *count);
void viewActivities(Activity *activities, int count);
void calculateAverageEmissions(Activity *activities, int count);
void resetActivities(Activity **activities, int *count);
void displayReport(Activity *activities, int count);

int main() {
    Activity *activities = NULL;
    int count = 0;
    int choice;
    loadActivities(&activities, &count);


    do {
        printf("\nCarbon Footprint Calculator\n");
        printf("1. Add Activity\n");
        printf("2. Remove Activity\n");
        printf("3. View Activities\n");
        printf("4. Calculate Average Emissions Per Activity\n");
        printf("5. Reset All Activities\n");
        printf("6. Display Report\n");
        printf("7. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addActivity(&activities, &count);
                loadingAnimation();
                break;
            case 2:
                removeActivity(&activities, &count);
                loadingAnimation();
                break;
            case 3:
                viewActivities(activities, count);
                loadingAnimation();
                break;
            case 4:
                calculateAverageEmissions(activities, count);
                loadingAnimation();
                break;
            case 5:
                resetActivities(&activities, &count);
                loadingAnimation();
                break;
            case 6:
                displayReport(activities, count);
                loadingAnimation();
                break;
            case 7:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    free(activities);
    return 0;
}

void addActivity(Activity **activities, int *count) {
    *activities = realloc(*activities, (*count + 1) * sizeof(Activity));
    if (*activities == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter activity type: ");
    scanf("%s", (*activities)[*count].type);
    printf("Enter activity value: ");
    scanf("%f", &(*activities)[*count].value);
    printf("Enter emission factor: ");
    scanf("%f", &(*activities)[*count].emission_factor);

    (*count)++;

    FILE *file = fopen("activities.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %.2f %.2f\n", (*activities)[*count - 1].type, (*activities)[*count - 1].value, (*activities)[*count - 1].emission_factor);
        fclose(file);
    } else {
        perror("Failed to open file");
    }

    printf("Activity added successfully.\n");
}

void removeActivity(Activity **activities, int *count) {
    if (*count == 0) {
        printf("No activities to remove.\n");
        return;
    }

    int index;
    printf("Enter the index of the activity to remove (1 to %d): ", *count);
    scanf("%d", &index);

    if (index < 1 || index > *count) {
        printf("Invalid index.\n");
        return;
    }

    for (int i = index - 1; i < *count - 1; i++) {
        (*activities)[i] = (*activities)[i + 1];
    }

    *activities = realloc(*activities, (*count - 1) * sizeof(Activity));
    if (*activities == NULL && *count > 1) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    (*count)--;

    FILE *file = fopen("activities.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < *count; i++) {
            fprintf(file, "%s %.2f %.2f\n", (*activities)[i].type, (*activities)[i].value, (*activities)[i].emission_factor);
        }
        fclose(file);
    } else {
        perror("Failed to open file");
    }

    printf("Activity removed successfully.\n");
}

void viewActivities(Activity *activities, int count) {
    if (count == 0) {
        printf("No activities to display.\n");
        return;
    }

    printf("\nList of Activities:\n");
    printf("----------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - Value: %.2f, Emission Factor: %.2f\n", i + 1, activities[i].type, activities[i].value, activities[i].emission_factor);
    }
    printf("----------------------------\n");
}


void calculateAverageEmissions(Activity *activities, int count) {
    if (count == 0) {
        printf("No activities to calculate emissions for.\n");
        return;
    }

    float totalEmissions = 0;
    for (int i = 0; i < count; i++) {
        totalEmissions += activities[i].value * activities[i].emission_factor;
    }

    float averageEmissions = totalEmissions / count;
    printf("\nAverage Emissions Per Activity: %.2f kg CO2\n", averageEmissions);
}

void resetActivities(Activity **activities, int *count) {
    free(*activities);
    *activities = NULL;
    *count = 0;


    FILE *file = fopen("activities.txt", "w");
    if (file != NULL) {
        fclose(file);
    } else {
        perror("Failed to clear file");
    }

    printf("All activities have been reset.\n");
}

void displayReport(Activity *activities, int count) {
    if (count == 0) {
        printf("No activities to display a report for.\n");
        return;
    }

    float totalEmissions = 0;
    printf("\nCarbon Footprint Report:\n");
    printf("----------------------------\n");

    for (int i = 0; i < count; i++) {
        float emissions = activities[i].value * activities[i].emission_factor;
        totalEmissions += emissions;
        printf("%s: %.2f kg CO2\n", activities[i].type, emissions);
    }

    printf("----------------------------\n");
    printf("Total Emissions: %.2f kg CO2\n", totalEmissions);
}
void loadingAnimation() {
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        usleep(600000);
    }
    printf("\n");
}
void loadActivities(Activity **activities, int *count) {
    FILE *file = fopen("activities.txt", "r");
    if (file == NULL) {
        return;
    }

    char type[50];
    float value, emission_factor;

    while (fscanf(file, "%s %f %f", type, &value, &emission_factor) == 3) {
        *activities = realloc(*activities, (*count + 1) * sizeof(Activity));
        if (*activities == NULL) {
            perror("Memory allocation failed");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        strcpy((*activities)[*count].type, type);
        (*activities)[*count].value = value;
        (*activities)[*count].emission_factor = emission_factor;
        (*count)++;
    }

    fclose(file);
}
