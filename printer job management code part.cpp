#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5

typedef struct {
    int job_id;
    char doc_name[50];
    int pages;
    char user[30];
} PrintJob;

typedef struct {
    PrintJob jobs[MAX];
    int front;
    int rear;
    int count;
} PrintQueue;

void initQueue(PrintQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isFull(PrintQueue *q) {
    return q->count == MAX;
}

int isEmpty(PrintQueue *q) {
    return q->count == 0;
}

// Add job to the spooler (Enqueue)
void addJob(PrintQueue *q, int id, const char *name, int pages, const char *user) {
    if (isFull(q)) {
        printf("\n[Error] Printer spooler full! Cannot accept Job ID: %d\n", id);
        return;
    }
    q->rear = (q->rear + 1) % MAX;
    q->jobs[q->rear].job_id = id;
    strncpy(q->jobs[q->rear].doc_name, name, sizeof(q->jobs[q->rear].doc_name) - 1);
    q->jobs[q->rear].pages = pages;
    strncpy(q->jobs[q->rear].user, user, sizeof(q->jobs[q->rear].user) - 1);
    q->count++;
    printf("[Success] Job %d (\"%s\") added by %s.\n", id, name, user);
}

// Print and remove the front job (Dequeue)
void processJob(PrintQueue *q) {
    if (isEmpty(q)) {
        printf("\n[Info] No pending print jobs. Printer is idle.\n");
        return;
    }
    PrintJob current = q->jobs[q->front];
    q->front = (q->front + 1) % MAX;
    q->count--;

    printf("\n--- Processing Job ---\n");
    printf("Job ID: %d\nDocument: %s\nPages: %d\nSubmitted by: %s\n", 
           current.job_id, current.doc_name, current.pages, current.user);
    printf("Status: Printed successfully.\n----------------------\n");
}

// Display all pending jobs
void displayQueue(PrintQueue *q) {
    if (isEmpty(q)) {
        printf("\nPrint queue is empty.\n");
        return;
    }
    printf("\n=== Active Print Queue (%d/%d Jobs) ===\n", q->count, MAX);
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % MAX;
        printf("[%d] Job #%d | '%s' | %d pages | User: %s\n",
               i + 1, q->jobs[idx].job_id, q->jobs[idx].doc_name, 
               q->jobs[idx].pages, q->jobs[idx].user);
    }
    printf("========================================\n");
}

int main() {
    PrintQueue spooler;
    initQueue(&spooler);
    int choice, id = 101, pages;
    char doc[50], user[30];

    while (1) {
        printf("\n--- Printer Job Manager ---");
        printf("\n1. Submit Print Job\n2. Process Next Job\n3. View Queue Status\n4. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("Enter document name: ");
                scanf("%49s", doc);
                printf("Enter number of pages: ");
                scanf("%d", &pages);
                printf("Enter user name: ");
                scanf("%29s", user);
                addJob(&spooler, id++, doc, pages, user);
                break;
            case 2:
                processJob(&spooler);
                break;
            case 3:
                displayQueue(&spooler);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid selection.\n");
        }
    }
    return 0;
}
