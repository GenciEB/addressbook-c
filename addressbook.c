#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_PHONE 20
#define MAX_EMAIL 100
#define FILENAME "contacts.txt"
#define CSVFILE "contacts.csv"

struct Contact {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
};

void load_contacts(struct Contact **contacts, int *count, int *capacity) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return;

    struct Contact temp;
    while (fscanf(file, " %99[^|]|%19[^|]|%99[^\n]\n", temp.name, temp.phone, temp.email) == 3) {
        if (*count >= *capacity) {
            *capacity *= 2;
            *contacts = realloc(*contacts, (*capacity) * sizeof(struct Contact));
            if (!*contacts) {
                perror("Memory error");
                exit(1);
            }
        }
        (*contacts)[(*count)++] = temp;
    }

    fclose(file);
}

void save_contacts(struct Contact *contacts, int count) {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("Failed to save contacts.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
    }

    fclose(file);
}

void add_contact(struct Contact *contacts, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity *= 2;
        contacts = realloc(contacts, (*capacity) * sizeof(struct Contact));
        if (!contacts) {
            perror("Memory error");
            exit(1);
        }
    }

    printf("Enter name: ");
    fgets(contacts[*count].name, MAX_NAME, stdin);
    contacts[*count].name[strcspn(contacts[*count].name, "\n")] = 0;

    printf("Enter phone: ");
    fgets(contacts[*count].phone, MAX_PHONE, stdin);
    contacts[*count].phone[strcspn(contacts[*count].phone, "\n")] = 0;

    printf("Enter email: ");
    fgets(contacts[*count].email, MAX_EMAIL, stdin);
    contacts[*count].email[strcspn(contacts[*count].email, "\n")] = 0;

    (*count)++;
    printf("Contact added!\n");
}

void view_contacts(struct Contact *contacts, int count) {
    if (count == 0) {
        printf("No contacts found.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\n[%d]\n", i + 1);
        printf("Name : %s\n", contacts[i].name);
        printf("Phone: %s\n", contacts[i].phone);
        printf("Email: %s\n", contacts[i].email);
    }
}

void search_contacts(struct Contact *contacts, int count) {
    char query[MAX_NAME];
    printf("Enter name to search: ");
    fgets(query, MAX_NAME, stdin);
    query[strcspn(query, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(contacts[i].name, query)) {
            printf("\n[%d]\n", i + 1);
            printf("Name : %s\n", contacts[i].name);
            printf("Phone: %s\n", contacts[i].phone);
            printf("Email: %s\n", contacts[i].email);
            found = 1;
        }
    }

    if (!found) {
        printf("No contact found with that name.\n");
    }
}

void delete_contact(struct Contact *contacts, int *count) {
    int index;
    printf("Enter contact number to delete: ");
    scanf("%d", &index);
    getchar(); // clear newline

    if (index < 1 || index > *count) {
        printf("Invalid contact number.\n");
        return;
    }

    for (int i = index - 1; i < *count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    (*count)--;

    printf("Contact deleted.\n");
}

void export_csv(struct Contact *contacts, int count) {
    FILE *file = fopen(CSVFILE, "w");
    if (!file) {
        printf("Failed to export CSV.\n");
        return;
    }

    fprintf(file, "Name,Phone,Email\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "\"%s\",\"%s\",\"%s\"\n",
                contacts[i].name,
                contacts[i].phone,
                contacts[i].email);
    }

    fclose(file);
    printf("Contacts exported to %s\n", CSVFILE);
}

int main() {
    struct Contact *contacts = malloc(10 * sizeof(struct Contact));
    int count = 0, capacity = 10;

    load_contacts(&contacts, &count, &capacity);

    while (1) {
        printf("\n--- Address Book ---\n");
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Search Contacts\n");
        printf("4. Delete Contact\n");
        printf("5. Save Contacts\n");
        printf("6. Export to CSV\n");
        printf("0. Exit\n");
        printf("Choose: ");

        int choice;
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1: add_contact(contacts, &count, &capacity); break;
            case 2: view_contacts(contacts, count); break;
            case 3: search_contacts(contacts, count); break;
            case 4: delete_contact(contacts, &count); break;
            case 5: save_contacts(contacts, count); break;
            case 6: export_csv(contacts, count); break;
            case 0: 
                save_contacts(contacts, count);
                free(contacts);
                printf("Goodbye!\n");
                return 0;
            default: printf("Invalid option.\n");
        }
    }
}
