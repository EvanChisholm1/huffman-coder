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

bn *createNode(char val, float prob) {
    bn *node = (bn*)malloc(sizeof(bn));
    node->val = val;
    node->prob = prob;
    node->left = node->left = NULL;

    return node;
}

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

bool isSizeOne(pq *priorityQueue) {
    return (priorityQueue->size == 1);
}

void buildMinHeap(pq *priorityQueue) {
    int n = priorityQueue->size - 1;
    for (int i = (n - 1) / 2; i >= 0; i--) {
        heapify(priorityQueue, i);
    }
}

bn *merge(bn *left, bn *right) {
    bn *top = createNode('$', left->prob + right->prob);
    top->left = left;
    top->right = right;
    return top;
}

bn *buildHuffmanTree(char characters[], float probabilities[], int size) {
    bn *left, *right, *top;
    pq *priorityQueue = createPriorityQueue(size);

    for(int i = 0; i < size; i++) {
        priorityQueue->array[i] = createNode(characters[i], probabilities[i]);
    }

    priorityQueue->size = size;
    buildMinHeap(priorityQueue);

    while(!isSizeOne(priorityQueue)) {
        bn *left = extractMin(priorityQueue);
        bn *right = extractMin(priorityQueue);


        top = merge(left, right);
        insertPriortyQueue(priorityQueue, top);
    }

    return extractMin(priorityQueue);
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

        used_chars += 1;
    }

    printf("%d\n", used_chars);

    char *characters = (char *)malloc(sizeof(char) * used_chars);
    float *probabilities = (float *)malloc(sizeof(float) * used_chars);

    int current_loc = 0;
    for(int i = 0; i < 256; i++) {
        if(counts[i]) {
            characters[current_loc] = i;
            probabilities[current_loc] = ((float)(counts[i])) / ((float)length);

            current_loc++;
        }
    }

    float prob_sum = 0.0;
    for(int i = 0; i < used_chars; i++) prob_sum += probabilities[i];
    printf("%f\n", prob_sum);

    bn *tree = buildHuffmanTree(characters, probabilities, used_chars);

    printf("%f\n", tree->prob);

    return 0;
}


