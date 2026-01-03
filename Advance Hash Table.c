#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 5

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
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 31) + key[i];
    }
    return hash % size;
}

HashTable* create_table(int size) {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->buckets = calloc(size, sizeof(Node*));
    return ht;
}

void insert(HashTable *ht, char *id, char *name, float gpa) {
    // ระบบ Advance แบบง่าย: ถ้าข้อมูลเริ่มเยอะ (Count == Size) ให้ขยายตาราง
    if (ht->count >= ht->size) {
        int old_size = ht->size;
        Node **old_buckets = ht->buckets;

        ht->size = old_size * 2;
        ht->buckets = calloc(ht->size, sizeof(Node*));
        ht->count = 0;

        for (int i = 0; i < old_size; i++) {
            Node *curr = old_buckets[i];
            while (curr != NULL) {
                insert(ht, curr->id, curr->name, curr->gpa);
                Node *temp = curr;
                curr = curr->next;
                free(temp);
            }
        }
        free(old_buckets);
        printf("\n>>> Table Resized to %d <<<\n", ht->size);
    }

    unsigned int index = hash_function(id, ht->size);
    Node *new_node = malloc(sizeof(Node));
    strcpy(new_node->id, id);
    strcpy(new_node->name, name);
    new_node->gpa = gpa;
    
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    ht->count++;
}

void display(HashTable *ht) {
    printf("\n--- Student List ---\n");
    for (int i = 0; i < ht->size; i++) {
        Node *curr = ht->buckets[i];
        printf("Bucket %d: ", i);
        while (curr) {
            printf("[%s: %.2f] -> ", curr->id, curr->gpa);
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
        printf("\n1. Add Student\n2. Show Table\n3. Exit\nSelect: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("ID: "); scanf("%s", id);
            printf("Name: "); scanf("%s", name);
            printf("GPA: "); scanf("%f", &gpa);
            insert(ht, id, name, gpa);
        } else if (choice == 2) {
            display(ht);
        } else if (choice == 3) {
            break;
        }
    }
    return 0;
}
