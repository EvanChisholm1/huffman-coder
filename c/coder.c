#include<stdio.h>
#include<stdlib.h>
// #include<stdbool.h>
#include<malloc.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>

#define bn struct BinaryNode

struct BinaryNode {
    bn *left;
    bn *right;
    char val;
    float prob;
};

int cmp(bn *a, bn *b) {
    float diff = a->prob - b->prob;

    if (diff == 0) return 0;

    return diff > 0 ? ceil(diff) : floor(diff);
}

bn *constructTree(bn **initial_nodes, int n) {
    printf("%d\n", n);
    printf("made it here!!\n");
    if(n == 1) {
        return initial_nodes[0];
    }

    qsort(initial_nodes, n, sizeof(bn*), cmp);

    int new_size = ceil(((float) n) / 2);
    bn **new_nodes;
    new_nodes = (bn **)malloc(new_size * sizeof(bn *));

    int new_nodes_loc = 0;
    for(int i = 0; i < n; i += 2) {
        if(i == (n - 1)) {
            new_nodes[new_nodes_loc] = initial_nodes[i];
            continue;
        }

        bn *left = initial_nodes[i];
        bn *right = initial_nodes[i + 1];

        bn *parent = (bn *)malloc(sizeof(bn));

        parent->left = left;
        parent->right = right;
        parent->val = 0;
        parent->prob = left->prob + right->prob;

        new_nodes[new_nodes_loc] = parent;

        new_nodes_loc++;
    }

    bn *root = constructTree(new_nodes, new_size);

    free(new_nodes);
    return root;
}

int count_leaf(bn *node, int depth) {
    if(node->left == NULL && node->right == NULL) {
        return 1;
    } else {
        int out = 0;
        if(node->left != NULL) out += count_leaf(node->left, depth + 1);
        if(node->right != NULL) out += count_leaf(node->right, depth + 1);
        return out;
    }
}

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

    float prob_sum = 0;

    for(int i = 0; i < used_chars; i++) {
        printf("%c prob: %f\n", initial_nodes[i]->val, initial_nodes[i]->prob);
        prob_sum += initial_nodes[i]->prob;
    }

    fclose(file);


    bn *root = constructTree(initial_nodes, used_chars);
    printf("made it here!!\n");

    printf("%f\n", root->prob);
    printf("%f\n", prob_sum);
    printf("%d\n", count_leaf(root, 0));

    for(int i = 0; i < used_chars; i++) {
        free(initial_nodes[i]);
    }

    free(initial_nodes);

    return 0;
}


