#include "calendar.h"

int calendar_main(int argc, char **argv) {
   Holiday* holidays = NULL;

   int count = get_holidays("/home/darko/GitHub/calendar/orage.ics", &holidays);

   while (holidays != NULL) {
      printf("%s: %d\n", holidays->description, holidays->date);
      holidays = holidays->next;
   }

   return 0;
}
