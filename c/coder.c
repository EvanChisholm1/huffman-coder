#include<stdio.h>
#include<stdlib.h>
// #include<stdbool.h>
#include<malloc.h>
#include<string.h>

#define bn struct BinaryNode

struct BinaryNode {
    bn *left;
    bn *right;
    char val;
    float prob;
};

int main() {
    FILE *file;
    char *filename = "../book.txt";
    char ch;

    file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }


    int counts[256];

    for(int i = 0; i < 256; i++) {
        counts[i] = 0;
    }

    int length = 0;

    while((ch = fgetc(file)) != EOF) {
        counts[ch] += 1;
        length += 1;
    }

    int used_chars = 0;

    for(int i = 0; i < 256; i++) {
        if(counts[i] == 0) continue;

        // printf("%c: %d\n", i, counts[i]);
        used_chars += 1;
    }



    printf("%d\n", used_chars);

    bn **initial_nodes;
    initial_nodes = (bn **)malloc(used_chars * sizeof(bn *));

    int current_loc = 0;
    for (int i = 0; i < 256; i++) {
        if(counts[i] == 0) continue;
        
        initial_nodes[current_loc] = (bn *)malloc(sizeof(bn));
        if(initial_nodes[current_loc] == NULL) {
            perror("failed to allocated memory for node struct");

            for(int j = 0; j < i; j++) {
                free(initial_nodes[j]);
            }
            free(initial_nodes);
            exit(EXIT_FAILURE);
        }

        initial_nodes[current_loc]->left = NULL;
        initial_nodes[current_loc]->right = NULL;
        initial_nodes[current_loc]->val = (char)i;
        initial_nodes[current_loc]->prob = ((float)counts[i]) / ((float) length);

        current_loc++;
    }


    for(int i = 0; i < used_chars; i++) {
        printf("%c prob: %f\n", initial_nodes[i]->val, initial_nodes[i]->prob);
    }

    fclose(file);

    for(int i = 0; i < used_chars; i++) {
        free(initial_nodes[i]);
    }

    free(initial_nodes);

    return 0;
}


