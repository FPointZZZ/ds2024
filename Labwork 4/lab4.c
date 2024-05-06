#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100


struct KeyValue {
    char key[MAX_WORD_LENGTH];
    int value;
};

void map(char *line, struct KeyValue *intermediate) {
    char *word;
    char delimiters[] = " .,;:!?\n\t"; 

    word = strtok(line, delimiters);

    while (word != NULL) {
        strcpy(intermediate->key, word);
        intermediate->value = 1;
        word = strtok(NULL, delimiters);
        printf("%s\t%d\n", intermediate->key, intermediate->value);
    }
}

void reduce(struct KeyValue *intermediate, int count) {
    int i, j;
    struct KeyValue result[count]; 
    int result_count = 0;

    for (i = 0; i < count; i++) {
        int found = 0;
        // Check if key already exists in result array
        for (j = 0; j < result_count; j++) {
            if (strcmp(intermediate[i].key, result[j].key) == 0) {
                result[j].value++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(result[result_count].key, intermediate[i].key);
            result[result_count].value = 1;
            result_count++;
        }
    }

    for (i = 0; i < result_count; i++) {
        printf("%s\t%d\n", result[i].key, result[i].value);
    }
}

int main() {
    char line[1000];
    struct KeyValue intermediate[1000]; 
    int count = 0;

    FILE *file = fopen("lab4.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file.\n");
        return 1;
    }


    while (fgets(line, sizeof(line), file)) {
        map(line, &intermediate[count]);
        count++;
    }

    fclose(file);

    reduce(intermediate, count);

    return 0;
}
