#include<stdio.h>
#include<stdlib.h>
// #include<stdbool.h>
#include<malloc.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>

#define bn struct BinaryNode
#define pq struct PriorityQueue

struct BinaryNode {
    bn *left;
    bn *right;
    char val;
    float prob;
};

struct PriorityQueue {
    int size;
    int capacity;
    bn **array;
};

pq *createPriorityQueue(int capacity) {
    pq *priorityQueue = (pq*)malloc(sizeof(pq));
    priorityQueue->size = 0;
    priorityQueue->capacity = capacity;
    priorityQueue->array = (bn**)malloc(priorityQueue->capacity * sizeof(bn*));

    return priorityQueue;
}

void swapNodes(bn **a, bn **b) {
    bn *temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(pq *priorityQueue, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if(left < priorityQueue->size && priorityQueue->array[left]->prob < priorityQueue->array[smallest]->prob) {
        smallest = left;
    }

    if(right < priorityQueue->size && priorityQueue->array[right]->prob < priorityQueue->array[smallest]->prob) {
        smallest = right;
    }

    if(smallest != idx) {
        swapNodes(&priorityQueue->array[smallest], &priorityQueue->array[idx]);
        heapify(priorityQueue, smallest);
    }
}

bn *extractMin(pq *priorityQueue) {
    bn *temp = priorityQueue->array[0];
    priorityQueue->array[0] = priorityQueue->array[priorityQueue->size - 1];
    --priorityQueue->size;
    heapify(priorityQueue, 0);
    return temp;
}

void insertPriortyQueue(pq *priorityQueue, bn *node) {
    ++priorityQueue->size;

    int i = priorityQueue->size - 1;

    while(i && node->prob < priorityQueue->array[(i - 1) / 2]->prob) {
        priorityQueue->array[i] = priorityQueue->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    priorityQueue->array[i] = node;
}

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

    int size = n;

    while(size > 1) {

    }

    return root;
}

int countLeaf(bn *node, int depth) {
    if(node->left == NULL && node->right == NULL) {
        return 1;
    } else {
        int out = 0;
        if(node->left != NULL) out += count_leaf(node->left, depth + 1);
        if(node->right != NULL) out += count_leaf(node->right, depth + 1);
        return out;
    }
}

// int getCodes(int)

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


