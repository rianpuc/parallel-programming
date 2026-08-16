#include "reader.h"

Passenger_Satisfaction *data;

void build_passenger_satisfaction(Passenger_Satisfaction *ps, char *line, char *delim) {
    char *token = strtok(line, delim);
    int index = 0;
    while (token != NULL) {
        switch (index) {
            case 0: { ps->row = atoi(token); break; }
            case 1: { ps->id = atoi(token); break; }
            case 2: { ps->gender = (strcmp(token, "Male") == 0); break; }
            case 3: { ps->customer_type = (strcmp(token, "Loyal Customer") == 0); break; }
            case 4: { ps->age = atoi(token); break; }
            case 5: { ps->type_of_travel = (strcmp(token, "Business travel") == 0); break; }
            case 6: { strncpy(ps->class, token, strlen(token)); ps->class[strlen(token)] = '\0'; break; }
            case 7: { ps->flight_distance = atoi(token); break; }
            case 8: { ps->inflight_wifi_service = atoi(token); break; }
            case 9: { ps->departure_arrival_time_convenient = atoi(token); break; }
            case 10: { ps->ease_of_online_booking = atoi(token); break; }
            case 11: { ps->gate_location = atoi(token); break; }
            case 12: { ps->food_and_drink = atoi(token); break; }
            case 13: { ps->online_boarding = atoi(token); break; }
            case 14: { ps->seat_comfort = atoi(token); break; }
            case 15: { ps->inflight_entertainment = atoi(token); break; }
            case 16: { ps->onboard_service = atoi(token); break; }
            case 17: { ps->leg_room_service = atoi(token); break; }
            case 18: { ps->baggage_handling = atoi(token); break; }
            case 19: { ps->checkin_service = atoi(token); break; }
            case 20: { ps->inflight_service = atoi(token); break; }
            case 21: { ps->cleanliness = atoi(token); break; }
            case 22: { ps->departure_delay_in_minutes = atoi(token); break; }
            case 23: { ps->arrival_delay_in_minutes = atof(token); break; }
            case 24: { ps->satisfaction = (strcmp(token, "satisfied") == 0); break; }
            default: break; 
        }
        token = strtok(NULL, delim);
        index++;
    }
}

void open_file(int choice, char* delim) {
    int stop = 0;
    FILE *file = fopen(file_names[choice], "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    int count = 0;
    int ch;
    char line[512];
    fgets(line, sizeof(line), file); // pulando header

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    fclose(file);

    data = malloc(count * sizeof(Passenger_Satisfaction));

    file = fopen(file_names[choice], "r");
    fgets(line, sizeof(line), file); // pulando header
    while (fgets(line, sizeof(line), file) != NULL && stop < 10) {
        build_passenger_satisfaction(&data[stop++], line, delim);
    }

    fclose(file);
}