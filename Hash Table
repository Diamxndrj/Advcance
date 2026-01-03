#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 7
#define LOAD_FACTOR_THRESHOLD 0.7

typedef struct Node {
    char id[20];
    char name[50];
    float gpa;
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    int size;
    int count;
} HashTable;

unsigned int hash_function(const char *key, int size) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) hash = ((hash << 5) + hash) + c;
    return hash % size;
}

HashTable* create_table(int size) {
    HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->buckets = (Node**)calloc(size, sizeof(Node*));
    return ht;
}

void insert_student(HashTable *ht, char *id, char *name, float gpa);

void rehash(HashTable *ht) {
    int old_size = ht->size;
    int new_size = old_size * 2 + 1;
    Node **old_buckets = ht->buckets;

    printf("\n[System] Expanding table to size %d...\n", new_size);

    ht->buckets = (Node**)calloc(new_size, sizeof(Node*));
    ht->size = new_size;
    ht->count = 0;

    for (int i = 0; i < old_size; i++) {
        Node *curr = old_buckets[i];
        while (curr != NULL) {
            Node *temp = curr;
            insert_student(ht, temp->id, temp->name, temp->gpa);
            curr = curr->next;
            free(temp);
        }
    }
    free(old_buckets);
}

void insert_student(HashTable *ht, char *id, char *name, float gpa) {
    if ((float)(ht->count + 1) / ht->size > LOAD_FACTOR_THRESHOLD) {
        rehash(ht);
    }

    unsigned int index = hash_function(id, ht->size);
    Node *curr = ht->buckets[index];

    while (curr != NULL) {
        if (strcmp(curr->id, id) == 0) {
            curr->gpa = gpa;
            strcpy(curr->name, name);
            return;
        }
        curr = curr->next;
    }

    Node *new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->id, id);
    strcpy(new_node->name, name);
    new_node->gpa = gpa;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    ht->count++;
}

void display_all(HashTable *ht) {
    printf("\n--- Current Records (Size: %d, Count: %d) ---\n", ht->size, ht->count);
    for (int i = 0; i < ht->size; i++) {
        printf("[%d]: ", i);
        Node *curr = ht->buckets[i];
        while (curr) {
            printf("(%s, %.2f) -> ", curr->id, curr->gpa);
            curr = curr->next;
        }
        printf("NULL\n");
    }
}

int main() {
    HashTable *ht = create_table(INITIAL_SIZE);
    int choice;
    char id[20], name[50];
    float gpa;

    while(1) {
        printf("\n1. Add/Update\n2. Show All\n3. Exit\nSelect: ");
        if (scanf("%d", &choice) != 1) break;

        if(choice == 1) {
            printf("ID: "); scanf("%s", id);
            printf("Name: "); scanf("%s", name);
            printf("GPA: "); scanf("%f", &gpa);
            insert_student(ht, id, name, gpa);
        } else if(choice == 2) {
            display_all(ht);
        } else if(choice == 3) {
            break;
        }
    }
    return 0;
}
