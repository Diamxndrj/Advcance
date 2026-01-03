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
        printf("\n>>> System: Table Resized to %d <<<\n", ht->size);
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

void search(HashTable *ht, char *id) {
    unsigned int index = hash_function(id, ht->size);
    Node *curr = ht->buckets[index];
    while (curr != NULL) {
        if (strcmp(curr->id, id) == 0) {
            printf("\nFound: ID : %s| Name : %s| GPA : %.2f\n", curr->id, curr->name, curr->gpa);
            return;
        }
        curr = curr->next;
    }
    printf("\nError : ID %s not found.\n", id);
}

void delete_node(HashTable *ht, char *id) {
    unsigned int index = hash_function(id, ht->size);
    Node *curr = ht->buckets[index];
    Node *prev = NULL;
    while (curr != NULL) {
        if (strcmp(curr->id, id) == 0) {
            if (prev == NULL) ht->buckets[index] = curr->next;
            else prev->next = curr->next;
            free(curr);
            ht->count--;
            printf("\nDeleted : Student ID %s\n", id);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("\nError: Cannot find ID %s to delete.\n", id);
}

void display(HashTable *ht) {
    printf("\n--- Student List (Total : %d) ---\n", ht->count);
    int found = 0, counter = 1;
    for (int i = 0; i < ht->size; i++) {
        Node *curr = ht->buckets[i];
        while (curr) {
            printf("%d. ID : %s | Name : %s | GPA : %.2f\n", counter++, curr->id, curr->name, curr->gpa);
            curr = curr->next;
            found = 1;
        }
    }
    if (!found) printf("No records available.\n");
    printf("------------------------------\n");
}

int main() {
    HashTable *ht = create_table(INITIAL_SIZE);
    int choice;
    char id[20], name[50];
    float gpa;
    while(1) {
        printf("\n=== Student Management System ===\n");
        printf("1. Add Student\n2. Search Student\n3. Delete Student\n4. Show All\n5. Exit Program\nSelect: ");
        if (scanf("%d", &choice) != 1) break;
        if (choice == 1) {
            printf("ID : "); scanf("%s", id);
            printf("Name : "); scanf("%s", name);
            printf("GPA : "); scanf("%f", &gpa);
            insert(ht, id, name, gpa);
        } else if (choice == 2) {
            printf("Enter ID to Search : "); scanf("%s", id);
            search(ht, id);
        } else if (choice == 3) {
            printf("Enter ID to Delete: "); scanf("%s", id);
            delete_node(ht, id);
        } else if (choice == 4) {
            display(ht);
        } else if (choice == 5) {
            break;
        }
    }
    return 0;
}
