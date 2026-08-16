#include <stdio.h>
#include "reader.h"

void printPassengerSatisfaction(Passenger_Satisfaction *ps) {
    printf("Row: %d\n", ps->row);
    printf("ID: %d\n", ps->id);
    printf("Gender: %s\n", ps->gender ? "Male" : "Female");
    printf("Customer Type: %s\n", ps->customer_type ? "Loyal Customer" : "disloyal Customer");
    printf("Age: %d\n", ps->age);
    printf("Type of Travel: %s\n", ps->type_of_travel ? "Business travel" : "Personal Travel");
    printf("Class: %s\n", ps->class);
    printf("Flight Distance: %d\n", ps->flight_distance);
    printf("Inflight Wifi Service: %d\n", ps->inflight_wifi_service);
    printf("Departure Arrival Time Convenient: %d\n", ps->departure_arrival_time_convenient);
    printf("Ease of Online Booking: %d\n", ps->ease_of_online_booking);
    printf("Gate Location: %d\n", ps->gate_location);
    printf("Food and Drink: %d\n", ps->food_and_drink);
    printf("Online Boarding: %d\n", ps->online_boarding);
    printf("Seat Comfort: %d\n", ps->seat_comfort);
    printf("Inflight Entertainment: %d\n", ps->inflight_entertainment);
    printf("Onboard Service: %d\n", ps->onboard_service);
    printf("Leg Room Service: %d\n", ps->leg_room_service);
    printf("Baggage Handling: %d\n", ps->baggage_handling);
    printf("Checkin Service: %d\n", ps->checkin_service);
    printf("Inflight Service: %d\n", ps->inflight_service);
    printf("Cleanliness: %d\n", ps->cleanliness);
    printf("Departure Delay in Minutes: %d\n", ps->departure_delay_in_minutes);
    printf("Arrival Delay in Minutes: %.2f\n", ps->arrival_delay_in_minutes);
    printf("Satisfaction: %s\n", ps->satisfaction ? "Satisfied" : "Neutral or Dissatisfied");
}

int main(void){
    int choice;
    list_dir("./datasets");
    scanf("%d", &choice);
    open_file(choice, ",");
    // printPassengerSatisfaction(&data[0]);
    return 0;
}