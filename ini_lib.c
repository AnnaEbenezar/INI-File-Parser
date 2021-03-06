#include <ctype.h>
#include <stdlib.h>
#include "ini_lib.h"
#include <string.h>

typedef struct 
{
    char *key[20];
    char *value[20];
} Properties;

typedef struct 
{
    char *section;
    Properties *properties;
} Section;

typedef struct 
{
    Section *sections[20];
} IniData;

int parse_ini(char *file)
{

    IniData iniData;

    FILE *fptr;
    if ((fptr = fopen(file, "r")) == NULL)
    {
        printf("Error opening file %s", file);
        exit(-1);
    }

    char chunk[MAX_LINE];
    size_t len = sizeof(chunk);
    char *line = malloc(len);
    
    if (line == NULL) 
    {
        perror("Memory allocation failed for line");
        exit(-1);
    }

    line[0] = '\0';
    int secCnt = -1;
    int keyCnt = 0;
    int valCnt = 0;
    
    while(fgets(chunk, sizeof(chunk), fptr) != NULL) 
    {
        size_t len_used = strlen(line);
        size_t chunk_used = strlen(chunk);

        if (len - len_used < chunk_used) 
        {
            len *= 2;
            if ((line = realloc(line, len)) == NULL) 
            {
                perror("Memory re-allocation failed for line");
                free(line);
                exit(-1);
            }
        }


        strncpy(line + len_used, chunk, len - len_used);
        len_used += chunk_used;
        if (line[len_used -1] == '\n') 
        {
           // fputs(line, stdout);
           if ((len_used - 1) == 0) 
           {
               memset(line, 0, len);
               continue;
           }
            if (strchr(line, '[') != NULL) 
            {
                secCnt++;
                //Parse section name and store in next available iniData array
                char *startStr = strchr(line, '[');
                int startIndex = (int)(startStr - line);
                char *endStr = strchr(line, ']');
                int endIndex = (int)(endStr - line);

                iniData.sections[secCnt] = malloc(sizeof(Section));
                iniData.sections[secCnt]->section = malloc(MAX_SECTION);

                memset(iniData.sections[secCnt]->section, 0, MAX_SECTION);
                strncpy(iniData.sections[secCnt]->section, line + 1, endIndex - startIndex);
                char *section = iniData.sections[secCnt]->section;
                *(section + endIndex - startIndex -1) = '\0';
                fputs(section, stdout);
                fputs("\n", stdout);
                iniData.sections[secCnt]->properties = malloc(sizeof(Properties));
                keyCnt = 0;
                valCnt = 0;
                line[0] = '\0';
                continue;
            }
            //Parse key value pairs under a section and store sequentially
            char *strIndex = strchr(line, '=');
            if (strIndex == NULL)
                continue;
            int index = (int)(strIndex - line);
            iniData.sections[secCnt]->properties->key[keyCnt] = malloc(MAX_NAME);
            iniData.sections[secCnt]->properties->value[valCnt] = malloc(MAX_VAL);
            char *key = iniData.sections[secCnt]->properties->key[keyCnt];
            char *val = iniData.sections[secCnt]->properties->value[valCnt];
            strncpy(key, line, index);
            strncpy(val, line + index + 1, len - index);
            key[index] = 0;
            //val[index] = 0;
            //printf("%d\n", (int)key[4]);
            printf("Key is %s\n", key);
            printf("Value is %s\n", val);
            keyCnt++;
            valCnt++;
            line[0] = '\0';
        }
    }
    fclose(fptr);
    free(line);

    //printf("\n\n Max line size: %zd\n", len);
}


