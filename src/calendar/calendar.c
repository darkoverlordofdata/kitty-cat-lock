#include "calendar.h"

typedef struct VRrule VRrule;
typedef struct ICalNode ICalNode;

struct VRrule 
{
    char* freq;         //  yearly
    char* count;        //  5
    char* byday;        //  byday
    char* bymonth;      //  bymonth
};

struct ICalNode 
{
    char* name;
    char* param;
    char* value;
};

char* rdbuf(char* path);
int index_of(char* values, char* value);
int last_dayofmonth(int m, int y);
int dayofweek(int y, int m, int d);
char** split2(char* str, char sep1, char sep2);
VRrule* vcalendar_parse_rrule(char* str);

/**
 *  vcalendar_parse_rrule
 *
 * @param str
 * @returns VRrule*
 */
VRrule* vcalendar_parse_rrule(char* str) {

    VRrule* rrule = malloc(sizeof(VRrule*));
    char** array = split2(str, '=', ';');
    static char* empty = "";
    rrule->freq = empty;
    rrule->count = empty;
    rrule->byday = empty;
    rrule->bymonth = empty;

    for (int i=0; array[i] != NULL; i+=2) {
        /* FREQ Repeat (Yearly, Monthly, Weekly, Daily, Hourly) */
        if (strcmp("FREQ", array[i]) == 0) {
            rrule->freq = array[i+1];
        }
        /* COUNT/UNTIL End (Never After:n On:date) */
        else if (strcmp("COUNT", array[i]) == 0) {
            rrule->count = array[i+1];
        }
        /* BYDAY+SETPOS+BYMONTH OnThe (1st 2nd 3rd 4th Last;  Mon-Sun / Day/ Weekday/ WeekendDay) of (Jan-Dec) */
        else if (strcmp("BYDAY", array[i]) == 0) {
            rrule->byday = array[i+1];
        }
        /* BYMONTH+BYMONTHDAY On (Jan - Dec; 1-31) */
        else if (strcmp("BYMONTH", array[i]) == 0) {
            rrule->bymonth = array[i+1];
        }
    }
    return rrule;
}


/**
 *  vcalendar_dispose
 *
 * @param this
 */
void vcalendar_dispose(VCalendar* this) {
    for (int i=0; i<this->nevents; i++) 
        free(this->event[i]);
    free(this->event);
    free(this->data);
    free(this);
}

/**
 *  vcalendar_new
 *
 * @param path
 * @returns new VCalendar* object
 */
VCalendar* vcalendar_new(char* path) {
    char* str = rdbuf(path);
    int count = 0;
    int len = strlen(str);
    int i = 0;
    char* s1;

    for (i = 0; i<len; i++) {
        if (str[i] == '\r' && str[i+1] == '\n') {
            if (str[i+2] == ' ') {
                // line continuation?
                str[i] = ' ';
                str[i+1] = ' ';
            } else {
                count += 1;
            }
        }
    }

    ICalNode* nodes = calloc(count+1, sizeof(ICalNode));
    int index = 0;
    nodes[index] = (ICalNode){ str, NULL, NULL};
    for (i = 1; i<len; i++) {
        if (str[i] == '\r' && str[i+1] == '\n') {
            str[i] = '\0';
            str[i+1] = '\0';
            for (s1 = nodes[index].name; *s1 != '\0'; s1++) {
                if (*s1 == ':') {
                    *s1 = '\0';
                    nodes[index].value = ++s1;
                }
            }
            for (s1 = nodes[index].name; *s1 != '\0'; s1++) {
                if (*s1 == ';') {
                    *s1 = '\0';
                    nodes[index].param = ++s1;
                }
            }
            index += 1;
            nodes[index] = (ICalNode){ &str[i+2], NULL, NULL };
            i += 1;
        }
    }
    index += 1;
    nodes[index] = (ICalNode){ NULL, NULL, NULL };
    count = 0;
    for (i=0; nodes[i].value != NULL; i++) {
        if (strcmp("VEVENT", nodes[i].value) == 0) 
            count++;
    }

    VCalendar* calendar = malloc(sizeof(VCalendar));
    VEvent* event;
    calendar->event = malloc(sizeof(VEvent*)*count);
    calendar->data = str;
    count = 0;
    
    /**
     * Load VCalendar tree 
     */
    int level = 0;
    for (int i=0; nodes[i].value != NULL; i++) {
        // "BEGIN",   //:VCALENDAR, VEVENT
        if (strcmp("BEGIN", nodes[i].name) == 0) {
            level++;
            switch (level) {
            case 1: //  VCALENDAR
                if (strcmp("VCALENDAR", nodes[i].value) == 0) {
                } else exit(99);
                break;
            case 2: //  VEVENT
                if (strcmp("VEVENT", nodes[i].value) == 0) {
                    event = calendar->event[count++] = malloc(sizeof(VEvent));
                } else exit(99);
                break;
            }
            continue;
        }
        // "END",              //:VCALENDAR, VEVENT
        else if (strcmp("END", nodes[i].name) == 0) {
            switch (level) {
            case 1: //  VCALENDAR
                if (strcmp("VCALENDAR", nodes[i].value) == 0) {
                } else exit(99);
                break;
            case 2: //  VEVENT
                if (strcmp("VEVENT", nodes[i].value) == 0) {
                } else exit(99);
                break;
            }
            level--;
            continue;
        }

        switch (level) {
        
        case 1:
            // "VERSION",          //:2.0
            if (strcmp("VERSION", nodes[i].name) == 0) {
                calendar->version = nodes[i].value;
            }
            // "PRODID",           //:icalendar-ruby
            else if (strcmp("PRODID", nodes[i].name) == 0) {
                calendar->prodid = nodes[i].value;
            }
            // "CALSCALE",         //:GREGORIAN
            else if (strcmp("CALSCALE", nodes[i].name) == 0) {
                calendar->calscale = nodes[i].value;
            }
            // "X-WR-CALNAME",     //:US Holidays
            else if (strcmp("X-WR-CALNAME", nodes[i].name) == 0) {
                calendar->calname = nodes[i].value;
            }
            // "X-APPLE-LANGUAGE", //:en-US
            else if (strcmp("X-APPLE-LANGUAGE", nodes[i].name) == 0) {
                calendar->language = nodes[i].value;
            }
            // "X-APPLE-REGION",   //:US
            else if (strcmp("X-APPLE-REGION", nodes[i].name) == 0) {
                calendar->region = nodes[i].value;
            }
            break;

        case 2:
            // "DTSTAMP",          //:20190531T224632Z
            if (strcmp("DTSTAMP", nodes[i].name) == 0) {
                event->dtstamp = nodes[i].value;
            }
            // "UID",              //:c5bbb4ae-8dfd-33db-b3e0-4862c55e3ce7
            else if (strcmp("UID", nodes[i].name) == 0) {
                event->uid = nodes[i].value;
            }
            // "DTSTART",          //;VALUE=DATE:20180704
            else if (strcmp("DTSTART", nodes[i].name) == 0) {
                event->dtstart = nodes[i].value;
            }
            // "CLASS",            //:PUBLIC
            else if (strcmp("CLASS", nodes[i].name) == 0) {
                event->klass = nodes[i].value;
            }
            // "SUMMARY",          //;LANGUAGE=en-US:Independence Day
            else if (strcmp("SUMMARY", nodes[i].name) == 0) {
                event->summary = nodes[i].value;
            }
            // "TRANSP",           //:TRANSPARENT
            else if (strcmp("TRANSP", nodes[i].name) == 0) {
                event->transp = nodes[i].value;
            }
            // "RRULE",            //:FREQ=YEARLY;COUNT=5
            else if (strcmp("RRULE", nodes[i].name) == 0) {
                event->rrule = nodes[i].value;
            }
            // "CATEGORIES",       //:Holidays
            else if (strcmp("CATEGORIES", nodes[i].name) == 0) {
                event->categories = nodes[i].value;
            }
            // "X-APPLE-UNIVERSAL-ID", //:f42c6443-488d-9756-925b-bf42f63a3348
            else if (strcmp("X-APPLE-UNIVERSAL-ID", nodes[i].name) == 0) {
                event->id = nodes[i].value;
            }
            break;
        }
    }
    free(nodes);
    calendar->nevents = count;
    return calendar;
}

/**
 *  holidays_new
 *
 struct Holidays 
{
   Holiday* head;       //  next => in list
   int today;           //  today's date
   int count;           //  count of holidays
};

 * @param path
 * @param holidays list
 * @returns list size
 */
Holidays* holidays_new(char* path)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    Holidays* this = malloc(sizeof(Holidays*));
    this->today = (1900+t->tm_year)*10000 + (t->tm_mon+1)*100 + t->tm_mday;
    Holiday** holidays = &this->head;

   VCalendar* cal = vcalendar_new(path);
   VRrule* rr;

   int count = 0;
   for (int i=0; i<cal->nevents; i++) {
      /**
       * only consider entries with a YEARLY recurance rule
       */
      if (cal->event[i]->rrule) {
         if (strstr(cal->event[i]->rrule, "FREQ=YEARLY;") > 0)
         count += 1;
      }
   }

   for (int i=0; i<cal->nevents; i++) {
      /**
       * only consider entries with a YEARLY recurance rule
       */
      if (cal->event[i]->rrule) {
        rr = vcalendar_parse_rrule(cal->event[i]->rrule);
        if (strcmp("YEARLY", rr->freq) != 0) continue;

        int dtstart = atoi(cal->event[i]->dtstart);

        if (strlen(rr->byday)>0 && strlen(rr->bymonth)>0) 
        {
        /**
            * Holiday such as Memorial Day or MLK Birthday
            * occur on the Nth Weekday of the month.
            */
        bool searching = true;
        bool last;
        char byday;
        int ordinal;
        char* dayname;

        if (rr->byday[0] == '-') {
            last = true;
            byday = rr->byday[1];
            ordinal = (int)byday-48;
            dayname = &rr->byday[2];
        } else {
            last = false;
            byday = rr->byday[0];
            ordinal = (int)byday-48;
            dayname = &rr->byday[1];
        }
            
        int yyyy = (this->today/10000) * 10000;
        int mm = atoi(rr->bymonth);

        if (last) {
            /**
            * Look for Last Monday
            */
            while (searching) {

                int last_day = last_dayofmonth(yyyy, mm);
                int offset = index_of("SU.MO.TU.WE.TH.FR.SA.", dayname);
                if (offset < 0) break;
                int day = last_day - 7 * ordinal + offset;
                dtstart = yyyy + mm * 100 + day;
                if (dtstart < this->today) yyyy += 10000;
                else searching = false;
            }
        } else {
            /**
            * Look for Nth Monday
            */
            while (searching) {

                int start_day = dayofweek(yyyy/10000, mm, 1);
                int offset = index_of("SU.MO.TU.WE.TH.FR.SA.", dayname);
                if (offset < 0) break;
                int weeks;
                if (start_day == 0) 
                    weeks = (7 * (ordinal-1)) + 1;
                else
                    weeks = (7 * (ordinal)) + 1;
                int day = weeks - start_day + offset;
                dtstart = yyyy + mm * 100 + day;

                if (dtstart < this->today) yyyy += 10000;
                else searching = false;
            }
        }

        } else {
            /**
             * This is easy - holidays like New Years Day
             * or 4th of July are always on the same date.
             */
            while (dtstart < this->today) {
               dtstart += 10000; //add 1 year
            }
        }

         /**
          * push holiday node onto list 
          */
         Holiday* new_node = malloc(sizeof(Holiday)); 
         new_node->description  = strdup(cal->event[i]->summary);
         new_node->date = dtstart;
         new_node->next = (*holidays); 
         (*holidays) = new_node;          
      }
   }
   vcalendar_dispose(cal);
   this->count = count;
   return this;
}

// typedef void (*HolidayIter) (Holiday* this);

void holidays_filter(Holidays* this, int for_date, HolidayIter iterate) 
{
    Holiday* holiday = this->head;
    
    while (holiday != NULL) {
        if (for_date == 0 || for_date == holiday->date) iterate(holiday);
        holiday = holiday->next;
    }
}


void holidays_dispose(Holidays* this)
{
    Holiday* holidays = this->head;
    Holiday* next = NULL;

    while (holidays != NULL) {
        next = holidays->next;
        free(holidays->description);
        free(holidays);
        holidays = next;
    }
    free(this);

}

/**
 * ReadTextFile
 * 
 * @param path path to file
 * @returns string with file contents
 */
char* rdbuf(char* path)
{
    FILE* f = fopen(path, "r");

    fseek(f, 0L, SEEK_END);
    long s = ftell(f);
    rewind(f);
    char* buf = (char*)calloc(s+1, sizeof(char));
    buf[s] = '\0';

    if (buf != NULL)
    {
        fread(buf, s, 1, f);
        return buf;
    }
    return buf;
}

/**
 * index_of
 * 
 * @param values string list of values
 * @param value string to find
 * @returns ordinal position
 */
int index_of(char* values, char* value) {
   int len = strlen(value)+1;
   return (((int)(strstr(values, value) - values)+len)/len)-1;
}

/**
 * last_dayofmonth
 * 
 * @param year
 * @param month
 * @returns day number (0-6)
 */
int last_dayofmonth(int y, int m) {
   //                     jan feb mar apr may jun jul aug sep oct nov dec
   static int d[] = { 0,  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   d[2] = (y%100 ? y%4 ? false : true : y%400 ? false : true) ? 29 : 28;
   return d[m];
}

/**
 * dayofweek
 * 
 * @param year
 * @param month
 * @param day
 * @returns day number (0-6)
 */
int dayofweek(int y, int m, int d) 
{ 
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 }; 
    y -= m < 3; 
    return ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7; 
} 

/**
 * split2
 *
 * split string into array with 2 seperators
 * 
 * @param str
 * @param sep1
 * @param sep2
 * @returns array
 */
char** split2(char* str, char sep1, char sep2) {
    int count = 0;
    int len = strlen(str+1);
    int i;

    for (i = 0; i<len; i++) 
        if (str[i] == sep1 || str[i] == sep2) count++;

    char** array = malloc(count*sizeof(char*));
    int index = 0;
    array[0] = str;
    for (i = 0; i<len; i++) {
        if (str[i] == sep1 || str[i] == sep2) {
            str[i] = '\0';
            index += 1;
            array[index] = &str[i+1];
        }
    }
    index += 1;
    array[index] = NULL;
    return array;
}


char** split(char* str, char sep) {
    int count = 0;
    int len = strlen(str+1);
    int i;

    for (i = 0; i<len; i++) 
        if (str[i] == sep) count++;

    char** array = malloc(count*sizeof(char*));
    int index = 0;
    array[0] = str;
    for (i = 0; i<len; i++) {
        if (str[i] == sep) {
            str[i] = '\0';
            index += 1;
            array[index] = &str[i+1];
        }
    }
    index += 1;
    array[index] = NULL;
    return array;
}

