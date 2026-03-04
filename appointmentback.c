#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_APPOINTMENTS 100

struct Appointment {
    char name[100];
    char phone[20];
    char date[11];  // YYYY-MM-DD
    char time[10];  // Enough space for safety // HH:MM
    char reason[200];
};

struct Appointment heap[MAX_APPOINTMENTS];
int heapSize = 0;

int compare(struct Appointment *a, struct Appointment *b) {
    char da[20], db[20];
    sprintf(da, "%sT%s", a->date, a->time);
    sprintf(db, "%sT%s", b->date, b->time);
    return strcmp(da, db);
}

void swap(int i, int j) {
    struct Appointment temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void heapify_up(int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (compare(&heap[i], &heap[p]) < 0) {
            swap(i, p);
            i = p;
        } else break;
    }
}

void heapify_down(int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize && compare(&heap[left], &heap[smallest]) < 0)
        smallest = left;
    if (right < heapSize && compare(&heap[right], &heap[smallest]) < 0)
        smallest = right;

    if (smallest != i) {
        swap(i, smallest);
        heapify_down(smallest);
    }
}

void insert_appointment(struct Appointment a) {
    if (heapSize >= MAX_APPOINTMENTS) {
        printf("Max appointments reached.\n");
        return;
    }
    heap[heapSize] = a;
    heapify_up(heapSize++);
}

void delete_earliest() {
    if (heapSize == 0) {
        printf("No appointments to delete.\n");
        return;
    }

    struct Appointment earliest = heap[0];
    printf("Deleting earliest appointment: %s | %s | %s %s | %s\n",
           earliest.name, earliest.phone, earliest.date, earliest.time, earliest.reason);

    heap[0] = heap[--heapSize];
    heapify_down(0);
}


void print_appointments() {
    if (heapSize == 0) {
        printf("\nNo appointments to show.\n");
        return;
    }

    printf("\nAll Appointments (Earliest First):\n");

    // Make a copy of the heap to preserve original
    struct Appointment tempHeap[MAX_APPOINTMENTS];
    int tempSize = heapSize;
    memcpy(tempHeap, heap, sizeof(heap));

    while (tempSize > 0) {
        struct Appointment top = tempHeap[0];
        printf("%s | %s | %s %s | %s\n", top.name, top.phone, top.date, top.time, top.reason);

        tempHeap[0] = tempHeap[--tempSize];
        
        // Re-heapify tempHeap manually
        int i = 0;
        while (1) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < tempSize && compare(&tempHeap[left], &tempHeap[smallest]) < 0)
                smallest = left;
            if (right < tempSize && compare(&tempHeap[right], &tempHeap[smallest]) < 0)
                smallest = right;

            if (smallest != i) {
                struct Appointment tmp = tempHeap[i];
                tempHeap[i] = tempHeap[smallest];
                tempHeap[smallest] = tmp;
                i = smallest;
            } else break;
        }
    }
}

int main() {
    int choice;

    while (1) {
        printf("\nAppointment Scheduler\n");
        printf("1. Add appointment\n");
        printf("2. Delete earliest appointment\n");
        printf("3. Print all appointments\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        char line[10];
        fgets(line, sizeof(line), stdin);
        sscanf(line, "%d", &choice);

        if (choice == 1) {
            struct Appointment a;
            printf("\nEnter appointment details:\n");

            printf("Name: ");
            fgets(a.name, sizeof(a.name), stdin);
            a.name[strcspn(a.name, "\n")] = 0;

            printf("Phone: ");
            fgets(a.phone, sizeof(a.phone), stdin);
            a.phone[strcspn(a.phone, "\n")] = 0;

            printf("Date (YYYY-MM-DD): ");
            fgets(a.date, sizeof(a.date), stdin);
            a.date[strcspn(a.date, "\n")] = 0;

            printf("Time (HH:MM): ");
            fgets(a.time, sizeof(a.time), stdin);
            a.time[strcspn(a.time, "\n")] = 0;

            printf("Reason: ");
            fgets(a.reason, sizeof(a.reason), stdin);
            a.reason[strcspn(a.reason, "\n")] = 0;

            insert_appointment(a);
        } else if (choice == 2) {
            delete_earliest();
        } else if (choice == 3) {
            print_appointments();
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}



