#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILES 128
#define MAX_PATH 260

typedef struct {
    int row;
    int id;
    bool gender;
    bool customer_type;
    int age;
    bool type_of_travel;
    char class[9];
    int flight_distance;
    int inflight_wifi_service;
    int departure_arrival_time_convenient;
    int ease_of_online_booking;
    int gate_location;
    int food_and_drink;
    int online_boarding;
    int seat_comfort;
    int inflight_entertainment;
    int onboard_service;
    int leg_room_service;
    int baggage_handling;
    int checkin_service;
    int inflight_service;
    int cleanliness;
    int departure_delay_in_minutes;
    float arrival_delay_in_minutes;
    bool satisfaction;
} Passenger_Satisfaction;

extern char file_names[MAX_FILES][MAX_PATH];
extern int file_count;
extern Passenger_Satisfaction *data;

#ifdef __cplusplus
extern "C" {
#endif

void list_dir_posix(char *dirpath);
void list_dir_win(char *dirpath);

#ifdef _WIN32
#define list_dir list_dir_win
#else
#define list_dir list_dir_posix
#endif

void open_file(int choice, char* delim);

#ifdef __cplusplus
}
#endif

#endif