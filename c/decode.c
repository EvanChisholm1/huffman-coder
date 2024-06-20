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

void deserializeDictionary(FILE *f, hc *dictionary, int *size) {
    fread(size, sizeof(int), 1, f);
    
    for (int i = 0; i < *size; i++) {
        fread(&dictionary[i].symbol, sizeof(char), 1, f);
        int codeLength;
        fread(&codeLength, sizeof(int), 1, f);
        dictionary[i].code = (char *)malloc(sizeof(char) * codeLength);
        fread(dictionary[i].code, sizeof(char), codeLength, f);
    }
}

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

char *getDecompressedStr(char *sourceString, hc *dictionary, int size, int outLength) {
    int resultCapacity = 128;
    char *result = (char *)malloc(sizeof(char) * resultCapacity);
    if(result == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int resultIndex = 0;
    int codedStrLen = strlen(sourceString);
    char tempBuffer[128];
    int tempIndex = 0;

    for(int i = 0; i < codedStrLen; i++) {
        tempBuffer[tempIndex++] = sourceString[i];
        tempBuffer[tempIndex] = '\0';

        for(int j = 0; j < size; j++) {
            if(strcmp(tempBuffer, dictionary[j].code) == 0) {
                if(resultIndex == resultCapacity) {
                    resultCapacity *= 2;
                    result = (char *)realloc(result, sizeof(char) * resultCapacity);
                    if(result == NULL) {
                        perror("Error allocating memory");
                        exit(EXIT_FAILURE);
                    }
                }

                result[resultIndex++] = dictionary[j].symbol;
                tempIndex = 0;
                break;
            }
        }
        if(resultIndex == outLength) break;
    }

    result[resultIndex] = '\0';
    return result;
}


void deserializeCompressed(FILE *f, char **outString) {
    int uncompressedLength;
    int compressedLength;

    fread(&uncompressedLength, sizeof(int), 1, f);
    fread(&compressedLength, sizeof(int), 1, f);

    unsigned char *buffer = (unsigned char *)malloc(compressedLength);
    fread(buffer, sizeof(unsigned char), compressedLength, f);

    char *unpackedStr;
    unpackBits(buffer, compressedLength, &unpackedStr);

    *outString = getDecompressedStr(unpackedStr, huffmanDictionary, dictionarySize, uncompressedLength);
}

int main() {
    FILE *f = fopen("test.bin", "rb");

    deserializeDictionary(f, huffmanDictionary, &dictionarySize);

    for (int i = 0; i < dictionarySize; i++) {
        printf("%c: %s\n", huffmanDictionary[i].symbol, huffmanDictionary[i].code);
    }

    char *outString;
    deserializeCompressed(f, &outString);

    printf("%s\n", outString);

    fclose(f);
}
