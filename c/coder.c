#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<assert.h>

#define bn struct BinaryNode
#define pq struct PriorityQueue
#define hc struct HuffmanCode
#define MAX_SYMBOLS 256

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

struct HuffmanCode {
    char symbol;
    char *code;
};

hc huffmanDictionary[MAX_SYMBOLS];
int dictionarySize = 0;

void insertHuffmanCode(hc *dictionary, int *size, char symbol, char *code) {
    dictionary[*size].symbol = symbol;
    dictionary[*size].code = strdup(code);
    (*size)++;
}

// linear scan of table, should be fine since max of 256 values
char *getHuffmanCode(hc *dictionary, int size, char symbol) {
    for(int i = 0; i < size; i++) {
        if(dictionary[i].symbol == symbol) {
            return dictionary[i].code;
        }
    }

    return NULL;
}

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
        left = extractMin(priorityQueue);
        right = extractMin(priorityQueue);

        top = merge(left, right);
        insertPriortyQueue(priorityQueue, top);
    }

    return extractMin(priorityQueue);
}

void printCodes(bn *root, int arr[], int top) {
    // printf("%c\n", root->val);
    if(root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if(root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if(root->left == NULL && root->right == NULL) {
        printf("%d\n", top);
        printf("%c: ", root->val);
        for (int i = 0; i < top; i++) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

void buildDict(bn *root, int arr[], int top, hc *dictionary, int *size) {
    if(root->left) {
        arr[top] = 0;
        buildDict(root->left, arr, top + 1, dictionary, size);
    }

    if(root->right) {
        arr[top] = 1;
        buildDict(root->right, arr, top + 1, dictionary, size);
    }

    if(root->left == NULL && root->right == NULL) {

        char *code = (char*)malloc(sizeof(char) * (top + 1));

        for (int i = 0; i < top; i++) {
            if(arr[i] == 1) {
                code[i] = '1';
            } else {
                code[i] = '0';
            }
        }

        code[top] = '\0';

        insertHuffmanCode(dictionary, size, root->val, code);
    }
}

float calcAvgCodeLen(bn *root, int top) {
    float out = 0.0;
    if(root->left) {
        out += calcAvgCodeLen(root->left, top + 1);
    }

    if(root->right) {
        out += calcAvgCodeLen(root->right, top + 1);
    }

    if(root->left == NULL && root->right == NULL) {
        return root->prob * top;
    }

    return out;
}

int calcCompressedLength(char *sourceString) {
    int length = 0;
    for(int i = 0; i < strlen(sourceString); i++) {
        char *charCode = getHuffmanCode(huffmanDictionary, dictionarySize, sourceString[i]);
        length += strlen(charCode);
    }

    return length;
}

char *getCompresedStr(char *sourceString) {
    int outLength = calcCompressedLength(sourceString);
    char *outStr = (char *)malloc(sizeof(char) * (outLength + 1));

    int currentLocation = 0;
    for(int i = 0; i < strlen(sourceString); i++) {
        char *code = getHuffmanCode(huffmanDictionary, dictionarySize, sourceString[i]);
        for(int j = 0; j < strlen(code); j++) {
            outStr[currentLocation] = code[j];
            currentLocation++;
        }
    }

    outStr[outLength] = '\0';

    return outStr;
}

void packBits(char *sourceString, unsigned char **outBuffer, size_t *byteCount) {
    size_t length = strlen(sourceString);
    size_t numBytes = (length + 7) / 8;

    *byteCount = numBytes;

    *outBuffer = (unsigned char *)malloc(numBytes);
    if(*outBuffer == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i < numBytes; i++) {
        (*outBuffer)[i] = 0;
        for(size_t j = 0; j < 8; j++) {
            size_t bitIndex = i * 8 + j;
            if(bitIndex < length && sourceString[bitIndex] == '1') {
                (*outBuffer)[i] |= 1 << (7 - j);
            }
        }
    }
}

// need to add a way to see where the huffman code terminates
void unpackBits(unsigned char *buffer, size_t byteCount, char **outString) {
    size_t length = byteCount * 8;
    *outString = (char *)malloc(length + 1);
    if(*outString == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i < byteCount; i++) {
        for(size_t j = 0; j < 8; j++) {
            size_t bitIndex = i * 8 + j;
            if(bitIndex < length) {
                (*outString)[bitIndex] = (buffer[i] & (1 << (7 - j))) ? '1' : '0';
            }
        }
    }

    (*outString)[length] = '\0';

}

char *readFile(char *filename) {
    FILE *f = fopen(filename, "rt");
    assert(f);
    fseek(f, 0, SEEK_END);
    int l = ftell(f);
    printf("%d", l);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *)malloc(l + 1);
    buffer[l] = '\0';

    fread(buffer, 1, l, f);

    fclose(f);
    return buffer;
}

int main() {
    // when this code runs it causes a segfault way down the line no idea why
    // char *foobar = readFile("../test.txt");
    // printf("%s\n", foobar);
    // free(foobar);

    FILE *file;
    char *filename = "../book.txt";
    // char *filename = "../shake.txt";
    char ch;

    file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int counts[256];

    for(int i = 0; i < 256; i++) {
        counts[i] = 1;
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

    int arr[200];

    printf("made it here\n");
    printCodes(tree, arr, 0);
    float avg_len = calcAvgCodeLen(tree, 0);
    printf("made it here\n");

    printf("%f\n", avg_len);

    buildDict(tree, arr, 0, huffmanDictionary, &dictionarySize);

    char *code = getHuffmanCode(huffmanDictionary, dictionarySize, 'A');

    printf("%s\n", code);


    char *testString = "hello world!";
    printf("here!!\n");
    int compressedLength = calcCompressedLength(testString);
    printf("here!!\n");
    printf("%d -> %d\n", (int)strlen(testString) * 8, compressedLength);
    char *compressedStr = getCompresedStr(testString);
    printf("%s\n", compressedStr);

    printf("here\n");

    unsigned char *buffer;
    size_t byteCount;
    packBits(compressedStr, &buffer, &byteCount);
    printf("%ld\n", byteCount);



    // TODO: add clean up
    return 0;
}
