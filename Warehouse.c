#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

struct node {
    char ID[6];
    char nama[40];
    char ownership[40];
    int qnty;
    char cond[5];
    time_t entryTime;
    struct node *next;
    struct node *prev;
};

struct history {
    char action[30];
    char ID[6];
    char nama[40];
    char ownership[40];
    int qnty;
    char cond[5];
    struct history *next;
};

struct node *head = NULL, *tail = NULL, *ptr, *temp, *temp2, *nnode;

struct history *headHistory = NULL, *ptrHistory;
struct StackNode {
    struct history *data;
    struct StackNode *next;
};
struct StackNode *top = NULL;

void push(struct history *data) {
    struct StackNode *newNode = (struct StackNode *)malloc(sizeof(struct StackNode));
    newNode->data = data;
    newNode->next = top;
    top = newNode;
}

void generateRandomID(char* id, int length) {
    int i;
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (i = 0; i < length; i++) {
        int randomIndex = rand() % (sizeof(charset) - 1);
        id[i] = charset[randomIndex];
    }
    id[length] = '\0';
}

void newNode() {
    char condCheck[100];
    char condGood[] = "GOOD";
    char condBad[] = "BAD";
    int quantity;

    nnode = (struct node *)malloc(sizeof(struct node));
    generateRandomID(nnode->ID, 5);

    printf("\tGenerated Item ID: %s\n", nnode->ID);
    printf("\tInput Item Name: ");
    getchar();
    fgets(nnode->nama, sizeof(nnode->nama), stdin);
    nnode->nama[strcspn(nnode->nama, "\n")] = '\0';

    printf("\tInput Item Ownership: ");
    fgets(nnode->ownership, sizeof(nnode->ownership), stdin);
    nnode->ownership[strcspn(nnode->ownership, "\n")] = '\0';

    while (1) {
        printf("\tInput Item Quantity: ");
        if (scanf("%d", &quantity) != 1 || quantity <= 0) {
            printf("\n\tPlease Input A Number Greater Than 0.\n");
            while (getchar() != '\n');
        } else {
            nnode->qnty = quantity;
            break;
        }
    }

cond:
    printf("\tInput Item Condition [GOOD/BAD]: ");
    scanf("%99s", condCheck);

    if (strcasecmp(condCheck, condGood) == 0) {
        strcpy(nnode->cond, "GOOD");
    } else if (strcasecmp(condCheck, condBad) == 0) {
        strcpy(nnode->cond, "BAD");
    } else {
        printf("\n\tPlease Input A Valid Condition, e.g: GOOD, BAD, Good, Bad.\n\n");
        goto cond;
    }

    nnode->next = NULL;
    nnode->prev = NULL;
}

void addHistory(char* action, char* ID, char* nama, char* owner, int qnty, char* cond) {

//	if(action=="Database Export"){
//    	strcpy(newHistory->action, action);
//    	strcpy(newHistory->ID, ID);
//    	strcpy(newHistory->nama, nama);
//    	strcpy(newHistory->ownership, owner);
//    	newHistory->qnty = qnty;
//    	strcpy(newHistory->cond, cond);
//    	newHistory->next = NULL;
//	} else{
//	}
	struct history *newHistory = (struct history*)malloc(sizeof(struct history));
   	strcpy(newHistory->action, action);
    strcpy(newHistory->ID, ID);
    strcpy(newHistory->nama, nama);
    strcpy(newHistory->ownership, owner);
    newHistory->qnty = qnty;
    strcpy(newHistory->cond, cond);
    newHistory->next = NULL;
    
    if (headHistory == NULL) {
        headHistory = newHistory;
    } else {
    	ptrHistory = headHistory;
    	
    	while(ptrHistory->next!=NULL){
    		ptrHistory = ptrHistory->next;
		}
		
		ptrHistory->next = newHistory;
    }
}

void insertData() {
    int confirm;
    struct node *temp;

insert:
    newNode();

    if (head != NULL) {
        tail->next = nnode;
        nnode->prev = tail;
        tail = nnode;
    } else {
        head = tail = nnode;
    }

    do {
        printf("\n\tInput Data [%s] to Database?\n", nnode->nama);
        printf("\t1) Confirm\n");
        printf("\t2) Cancel (Undo)\n");
        printf("\t===> "); 
        scanf("%d", &confirm);

        if (confirm == 1) {
            break;
        } else if (confirm == 2) {
            printf("\n\tRe-Enter Item Details:\n");

            if (tail == head) { 
                free(nnode);
                head = tail = NULL;
            } else {
                temp = tail;
                tail = tail->prev;
                tail->next = NULL;
                free(temp);
            }

            printf("\n\n");

            goto insert;
        } else {
            printf("Invalid Option. Please Try Again.\n");
        }
    } while (confirm < 1 || confirm > 2);

    nnode->entryTime = time(NULL);
    addHistory("Data Added", nnode->ID, nnode->nama, nnode->ownership, nnode->qnty, nnode->cond);
}

void displayItems() {
    if (head == NULL) {
        printf("\n\tDATA IS EMPTY");
        return;
    }

    int choice;
    printf("\nChoose An Option:\n");
    printf("1) Not Sorted\n");
    printf("2) Sort Items\n");
    printf("===> ");
    scanf("%d", &choice);
    printf("\n");

    if (choice == 1) {
        ptr = head;
        if (head != NULL) {
            printf("\n============================================================================================================================\n");
        	printf("%-15s %-40s %-30s %-20s %-10s\n", "ITEM ID", "ITEM NAME", "ITEM OWNERSHIP", "ITEM QUANTITY", "ITEM CONDITION");
        	printf("============================================================================================================================\n");


            while (ptr != NULL) {
                printf("%-15s %-40s %-30s %-20d %-10s\n\n", ptr->ID, ptr->nama, ptr->ownership, ptr->qnty, ptr->cond);
                ptr = ptr->next;
            }
        } else {
            printf("\n\tDATA IS EMPTY");
        }
    } else if (choice == 2) {
        int opsi, sort;

        do {
            printf("\nSort Items By :\n");
            printf("1) Item Name\n");
            printf("2) Item Quantity\n");
            printf("3) Item Condition\n");
            printf("===> "); scanf("%d", &opsi);
            printf("\n");

            if (opsi == 3) {
                printf("Choose Condition:\n");
                printf("1) BAD\n");
                printf("2) GOOD\n");
                printf("===> "); scanf("%d", &sort);
                printf("\n");
            } else if(opsi>0 && opsi<4){              printf("ASCENDING/DESCENDING?\n");
                printf("1) ASC\n");
                printf("2) DESC\n");
                printf("===> "); scanf("%d", &sort);
                printf("\n");
            }

            if (opsi < 1 || opsi > 3 || sort < 1 || sort > 2) {
                printf("Please Input A Valid Option.\n");
                getchar();
            }
        } while (opsi < 1 || opsi > 3 || sort < 1 || sort > 2);

        int swapped;
        struct node *current;
        struct node *lptr = NULL;

        do {
            swapped = 0;
            current = head;

            while (current->next != lptr) {
                int compareResult = 0;
                switch (opsi) {
                    case 1:
                        compareResult = strcmp(current->nama, current->next->nama);
                        break;
                    case 2:
                        compareResult = current->qnty - current->next->qnty;
                        break;
                    case 3:
                        compareResult = strcmp(current->cond, current->next->cond);
                        break;
                }

                if ((sort == 1 && compareResult > 0) || (sort == 2 && compareResult < 0)) {
                    char tempID[6], tempNama[40], tempCond[5];
                    int tempQnty;

                    strcpy(tempID, current->ID);
                    strcpy(tempNama, current->nama);
                    tempQnty = current->qnty;
                    strcpy(tempCond, current->cond);

                    strcpy(current->ID, current->next->ID);
                    strcpy(current->nama, current->next->nama);
                    current->qnty = current->next->qnty;
                    strcpy(current->cond, current->next->cond);

                    strcpy(current->next->ID, tempID);
                    strcpy(current->next->nama, tempNama);
                    current->next->qnty = tempQnty;
                    strcpy(current->next->cond, tempCond);

                    swapped = 1;
                }
                current = current->next;
            }
            lptr = current;
        } while (swapped);

        printf("\n============================================================================================================================\n");
        printf("%-15s %-40s %-30s %-20s %-10s\n", "ITEM ID", "ITEM NAME", "ITEM OWNERSHIP", "ITEM QUANTITY", "ITEM CONDITION");
        printf("============================================================================================================================\n");

        ptr = head;
        while (ptr != NULL) {
            printf("%-15s %-40s %-30s %-20d %-10s\n\n", ptr->ID, ptr->nama, ptr->ownership, ptr->qnty, ptr->cond);
            ptr = ptr->next;
        }
    } else {
        printf("Invalid Choice. Please Input A Valid Option.\n");
    }
}

void search(){
    if (head == NULL) {
        printf("\n\tDATA IS EMPTY");
        return;
    }
    
    int opsi;
    
    do {
        printf("\nSearch Item By?\n");
        printf("1) ID\n");
        printf("2) Name\n");
        printf("3) Ownership\n"); // New option for ownership search
        printf("===> "); 
        scanf("%d", &opsi);
        
        if (opsi < 1 || opsi > 3) {
            printf("\nPlease Input A Valid Option.\n");
            getchar();
        }
    } while (opsi < 1 || opsi > 3);
    
    printf("\n");
    
    if (head == NULL) {
        printf("\tDATA IS EMPTY");
        return;
    }
    
    char searchValue[40];
    
    switch (opsi) {
        case 1:
            printf("Enter Item ID: \n"); printf("===> ");
            scanf("%s", searchValue);
            break;
        case 2:
            printf("Enter Item Name: \n"); getchar(); printf("===> ");
            fgets(searchValue, sizeof(searchValue), stdin);
            searchValue[strcspn(searchValue, "\n")] = '\0';
            break;
        case 3:
            printf("Enter Item Ownership: \n"); getchar(); printf("===> ");
            fgets(searchValue, sizeof(searchValue), stdin);
            searchValue[strcspn(searchValue, "\n")] = '\0';
            break;
    }
    
    int found = 0;
    
    printf("\n");
    printf("Search Result:\n");
    printf("============================================================================================================================\n");
    printf("%-15s %-40s %-30s %-20s %-10s\n", "ITEM ID", "ITEM NAME", "ITEM OWNERSHIP", "ITEM QUANTITY", "ITEM CONDITION");
    printf("============================================================================================================================\n");
    
    ptr = head;
    while (ptr != NULL) {
        int match = 0;
        switch (opsi) {
            case 1:
                if (strcmp(ptr->ID, searchValue) == 0) {
                    match = 1;
                }
                break;
            case 2:
                if (strcmp(ptr->nama, searchValue) == 0) {
                    match = 1;
                }
                break;
            case 3:
                if (strcmp(ptr->ownership, searchValue) == 0) {
                    match = 1;
                }
                break;
        }
        
        if (match) {
            printf("%-15s %-40s %-30s %-20d %-10s\n", ptr->ID, ptr->nama, ptr->ownership, ptr->qnty, ptr->cond);
            found = 1;
        }
        
        ptr = ptr->next;
    }
    
    if (!found) {
        printf("\n\tNO MATCHING ITEMS FOUND.\n");
    }
}

void updateItem() {
    if (head == NULL) {
        printf("\n\tDATA IS EMPTY");
        return;
    }

    int opsi;
    char cariID[6];
    char newName[40];
    char newOwner[40];
    int newQuantity;
    char newCondition[5];
    char condGood[] = "GOOD";
    char condBad[] = "BAD";
    char condCheck[100];

    printf("Input The ID Of The Item To Update: \n");
    printf("===> "); 
    scanf("%5s", cariID); 
    printf("\n");

    ptr = head;
    while (ptr != NULL) {
        if (strcmp(cariID, ptr->ID) == 0) {
            printf("Current Item Name: %s\n", ptr->nama);
            printf("Input New Item Name: ");
            getchar();
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            
            printf("\nCurrent Ownership: %s\n", ptr->ownership);
            printf("Input New Ownership: ");
            fgets(newOwner, sizeof(newOwner), stdin);
            newOwner[strcspn(newOwner, "\n")] = '\0';

            while (1) {
                printf("\nCurrent Item Quantity: %d\n", ptr->qnty);
                printf("Input New Item Quantity: ");
                if (scanf("%d", &newQuantity) != 1 || newQuantity <= 0) {
                    printf("\n\tPlease Input A Number & Greater Than 0.\n");
                    while (getchar() != '\n');
                } else {
                    ptr->qnty = newQuantity;
                    break;
                }
            }

            cond:
            printf("\nCurrent Item Condition: %s\n", ptr->cond);
            printf("Input New Item Condition [GOOD/BAD]: ");
            scanf("%99s", condCheck);

            if (strcasecmp(condCheck, condGood) == 0) {
                strcpy(newCondition, "GOOD");
            } else if (strcasecmp(condCheck, condBad) == 0) {
                strcpy(newCondition, "BAD");
            } else {
                printf("\n\tPlease input a proper condition, e.g: GOOD, BAD, Good, Bad.\n\n");
                goto cond;
            }

            strcpy(ptr->nama, newName);
            strcpy(ptr->ownership, newOwner);
            strcpy(ptr->cond, newCondition);

            printf("\nItem Updated Successfully!\n");
            addHistory("Data Updated", ptr->ID, ptr->nama, ptr->ownership, ptr->qnty, ptr->cond);
            return;
        }
        ptr = ptr->next;
    }

    printf("Item ID Not Found.");
}

void hapus(){
    if (head == NULL) {
        printf("\n\tDATA IS EMPTY");
        return;
    }
    
	int opsi;

    do{
        printf("Delete Data By? \n");
        printf("1) Item ID\n");
        printf("2) Item Name\n");
        printf("===> "); scanf("%d", &opsi);

        if(opsi==0 || opsi>2){
            printf("\nPlease Input A Valid Option.\n");
            getchar();
        }
    } while(opsi==0 || opsi>2);

    printf("\n");

    if(head==NULL){
        printf("\n\tDATA IS EMPTY");
        return;
    }

    ptr=head;
    temp=NULL;

    if(opsi==1){ //hapus berdasarkan ID
        char cariID[6];

        printf("Input The ID Of The Item To Delete: \n");
        printf("===> "); scanf("%5s", cariID); printf("\n");

        while(ptr!=NULL && temp!=ptr){
            if(strcmp(cariID, ptr->ID) == 0){
                temp=ptr;
            } else{
                ptr=ptr->next;
            }
        }

        if(temp==NULL){ //ID nya ga ketemu
            printf("Item ID Not Found.");
            return;
        }

        //kalo ID nya ketemu
        printf("Item %s With Item ID %s Has Been Deleted.", temp->nama, temp->ID);
        addHistory("Data Deleted", temp->ID, temp->nama, temp->ownership, temp->qnty, temp->cond);

        if(temp==head && temp==tail){ //kalo data nya cmn 1 jadinya head tail sama, trs kalo datanya diapus
            head=NULL;
            tail=NULL;
            free(temp);
        } else if(temp==head){ //kalo data yang dihapus itu data yg pertama
            head=head->next;
            if(head==tail){ // kalo cmn ada 2 data
                head->prev=NULL;
                tail=head;
            } else{ // total data > 2
                head->prev=NULL;
            }
            free(temp);
        } else if(temp==tail){ //kalo data yang dihapus itu data yg terakhir
            tail=tail->prev;
            tail->next=NULL;
            free(temp);
        } else{ // data ditengah
            ptr=temp;
            ptr=ptr->prev;
            ptr->next=temp->next;
            ptr=temp->next;
            ptr->prev=temp->prev;
            free(temp);
        }

    } else if(opsi==2){ //hapus berdasarkan nama
        // overall sama kea hapus by ID, bedanya ini berdasarkan nama aja, ganti variabel.
        char cariNama[40];

        printf("Input The Name Of The Item To Delete: \n");
        printf("===> ");
        getchar();
        fgets(cariNama, sizeof(cariNama), stdin);
        cariNama[strcspn(cariNama, "\n")] = '\0';
        printf("\n");

        while(ptr!=NULL && temp!=ptr){
            if(strcmp(cariNama, ptr->nama) == 0){
                temp=ptr;
            } else{
                ptr=ptr->next;
            }
        }

        if(temp==NULL){ //barang nya ga ketemu
            printf("Item Name Not Found.");
            return;
        }

        //kalo nama barang nya ketemu
        printf("Item %s Has Been Deleted.", temp->nama, temp->ID);
        addHistory("Data Deleted", temp->ID, temp->nama, temp->ownership, temp->qnty, temp->cond);

        if(temp==head && temp==tail){ //kalo data nya cmn 1 jadinya head tail sama, trs kalo datanya diapus
            head=NULL;
            tail=NULL;
            free(temp);
        } else if(temp==head){ //kalo data yang dihapus itu data yg pertama
            head=head->next;
            if(head==tail){ // kalo cmn ada 2 data
                head->prev=NULL;
                tail=head;
            } else{ // total data > 2
                head->prev=NULL;
            }
            free(temp);
        } else if(temp==tail){ //kalo data yang dihapus itu data yg terakhir
            tail=tail->prev;
            tail->next=NULL;
            free(temp);
        } else{ // data ditengah
            ptr=temp;
            ptr=ptr->prev;
            ptr->next=temp->next;
            ptr=temp->next;
            ptr->prev=temp->prev;
            free(temp);
        }
    }
}

void checkOut(){
    if (head == NULL) {
        printf("\n\tDATA IS EMPTY.");
        return;
    }
    
	int conf;
    
    printf("\n============ CHECKOUT ============\n\n");
    
    char cariID[6];
        
    printf("Input Item ID to Check Out: \n");
    printf("===> ");
    scanf("%5s", cariID); 
    printf("\n");
    
    ptr = head;
    struct node *prevNode = NULL;
    
    while (ptr != NULL) {
        if (strcmp(cariID, ptr->ID) == 0) {
            printf("Item ID: %s\n", ptr->ID);
            printf("Item Name: %s\n", ptr->nama);
            printf("Item Ownership: %s\n", ptr->ownership);
            printf("Item Quantity: %d\n", ptr->qnty);
            printf("Item Condition: %s\n\n", ptr->cond);
            printf("Check Out Successful!\n");
            addHistory("Data Checked Out", ptr->ID, ptr->nama, ptr->ownership, ptr->qnty, ptr->cond);
            
			time_t currentTime = time(NULL);
    		time_t timeSpent = difftime(currentTime, ptr->entryTime);
    		printf("Total Time Inside Warehouse: %ld Seconds\n", (long)timeSpent);
            
            long paymentAmount = timeSpent * 1000;
    		printf("Total Payment:Rp %ld \n", paymentAmount);

			if (ptr == head) {
                head = ptr->next;
                if (head != NULL) {
                    head->prev = NULL;
                }
                if (ptr == tail) {
                    tail = NULL;
                }
            } else if (ptr == tail) {
                tail = ptr->prev;
                tail->next = NULL;
            } else {
                prevNode->next = ptr->next;
                if (ptr->next != NULL) {
                    ptr->next->prev = prevNode;
                }
            }
            free(ptr); getch();
            return;
        }
        prevNode = ptr;
        ptr = ptr->next;
    }

    printf("Item ID Not Found.");
}


void viewHistory() {
    if (headHistory == NULL) {
        printf("\n\tHISTORY IS EMPTY.");
        return;
    }
    
    char act[40]="Database Export";
    
    printf("\n============ HISTORY ============\n\n");
    
    struct history *ptr = headHistory;
    int count = 1;
    while (ptr != NULL) {
        if(strcasecmp(ptr->action, act) == 0) {
            printf("[%d] Action: Exported %d Data to Database\n", count, ptr->qnty);
            printf("\n");
            ptr = ptr->next;
            count++;
        } else {
            push(ptr);
            ptr = ptr->next;
        }
    }
    
    while (top != NULL) {
        struct history *data = top->data;
        printf("[%d] Action: %s\n", count, data->action);
        printf("    Item ID: %s\n", data->ID);
        printf("    Item Name: %s\n", data->nama);
        printf("    Item Ownership: %s\n", data->ownership);
        printf("    Item Quantity: %d\n", data->qnty);
        printf("    Item Condition: %s\n", data->cond);
        printf("\n");
        top = top->next;
        count++;
    }
}

void importData() {
    char fileName[100];
    printf("\nEnter File Name To import: \n");
    printf("===> ");
	scanf("%s", fileName);

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("ERROR WRONG FILE NAME OR FILE NOT FOUND.\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        struct node *newNode = (struct node *)malloc(sizeof(struct node));

        char *token;
        token = strtok(line, ",");
        if (token != NULL) {
            strcpy(newNode->ID, token);
        } else {
            printf("\n");
            continue;
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(newNode->nama, token);
        } else {
            printf("\n");
            continue;
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(newNode->ownership, token);
        } else {
            printf("\n");
            continue;
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            newNode->qnty = atoi(token);
        } else {
            printf("\n");
            continue;
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(newNode->cond, token);
        } else {
            printf("\n");
            continue;
        }

        newNode->entryTime = time(NULL);
        newNode->next = NULL;
        newNode->prev = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        addHistory("Import From Database", newNode->ID, newNode->nama, newNode->ownership, newNode->qnty, newNode->cond);

        printf("Data '%s' Imported Successfully.\n", newNode->nama);
    }

    fclose(file);
}

void exportData() {
    char fileName[100];
    int count=0;
    printf("\nEnter File Name To Export: \n");
    printf("===> ");
	scanf("%s", fileName);

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("ERROR\n");
        return;
    }

    struct node *current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%d,%s\n", current->ID, current->nama, current->ownership, current->qnty, current->cond);
        current = current->next;
        count++;
    }

    fclose(file);
	addHistory("Database Export", "-", "-", "-", count, "-");
    printf("Data Exported Successfully.\n");
}

void exportLog() {
    char fileName[100] = "log.txt";
//    int count=0;
//    printf("\nEnter File Name To Export: \n");
//    printf("===> ");
//	scanf("%s", fileName);

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("ERROR\n");
        return;
    }

    struct history *current = headHistory;
    
    //header
    fprintf(file, "ACTION, ITEM ID, ITEM NAME, ITEM OWNER, QUANTITY, CONDITION\n");
    
	while (current != NULL) {	
        fprintf(file, "%s,%s,%s,%s,%d,%s\n", current->action, current->ID, current->nama, current->ownership, current->qnty, current->cond);
        current = current->next;
//        count++;
    }

    fclose(file);
//	addHistory("thisisexport", "", "-", "-", count, "-");
    printf("Log has been Exported Successfully.\n"); 
}

void printWarehouseTitle() {
    printf("    ___  ________   ________      \n");
    printf("   |\\  \\|\\   ___  \\|\\   ____\\     \n");
    printf("   \\ \\  \\ \\  \\\\ \\  \\ \\  \\___|_    \n");
    printf(" __ \\ \\  \\ \\  \\\\ \\  \\ \\_____  \\   \n");
    printf("|\\  \\\\_\\  \\ \\  \\\\ \\  \\|____|\\  \\  \n");
    printf("\\ \\________\\ \\__\\\\ \\__\\____\\_\\  \\ \n");
    printf(" \\|________|\\|__| \\|__|\\_________\\ \n");
    printf("                       \\|_________| \n");
    printf("\n");
}

void menu(){
	printWarehouseTitle();
	printf("\n========================================\n");
    printf("|            Welcome To JNS!           | \n");
    printf("========================================\n");
    printf("|  1) Add Item                         |\n");
    printf("|  2) Display Item                     |\n");
    printf("|  3) Search Item                      |\n");
    printf("|  4) Update Item                      |\n");
    printf("|  5) Delete Item                      |\n");
    printf("|  6) Check Out Item                   |\n");
    printf("|  7) Import Database                  |\n");
    printf("|  8) Export Database                  |\n");
    printf("|  9) View Log                         |\n");
    printf("|  10) Export Log                      |\n");
    printf("|  0) EXIT                             |\n");
    printf("========================================\n");
}

void printASCIIArtThankYou() {
    printf(" _________    ___  ___      ________      ________       ___  __             ___    ___  ________      ___  ___      ___       \n");
    printf("|\\___   ___\\ |\\  \\|\\  \\    |\\   __  \\    |\\   ___  \\    |\\  \\|\\  \\          |\\  \\  /  /||\\   __  \\    |\\  \\|\\  \\    |\\  \\      \n");
    printf("\\|___ \\  \\_| \\ \\  \\\\\\  \\   \\ \\  \\|\\  \\   \\ \\  \\\\ \\  \\   \\ \\  \\/  /|_        \\ \\  \\/  / /\\ \\  \\|\\  \\   \\ \\  \\\\\\  \\   \\ \\  \\     \n");
    printf("     \\ \\  \\   \\ \\   __  \\   \\ \\   __  \\   \\ \\  \\\\ \\  \\   \\ \\   ___  \\        \\ \\    / /  \\ \\  \\\\\\  \\   \\ \\  \\\\\\  \\   \\ \\  \\    \n");
    printf("      \\ \\  \\   \\ \\  \\ \\  \\   \\ \\  \\ \\  \\   \\ \\  \\\\ \\  \\   \\ \\  \\\\ \\  \\        \\/  /  /    \\ \\  \\\\\\  \\   \\ \\  \\\\\\  \\   \\ \\__\\   \n");
    printf("       \\ \\__\\   \\ \\__\\ \\__\\   \\ \\__\\ \\__\\   \\ \\__\\\\ \\__\\   \\ \\__\\\\ \\__\\     __/  / /       \\ \\_______\\   \\ \\_______\\   \\|__|   \n");
    printf("        \\|__|    \\|__|\\|__|    \\|__|\\|__|    \\|__| \\|__|    \\|__| \\|__|    |\\___/ /         \\|_______|    \\|_______|       ___ \n");
    printf("                                                                           \\|___|/                                        |\\__\\\n");
    printf("                                                                                                                          \\|__| \n");
}

void exitingMessage() {
    int i;
    
    system("cls");
	printf("\n\tExiting the Warehouse Management System");
    fflush(stdout);
    for (i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n\n");
    printASCIIArtThankYou();
    printf("\n");
}

int main(){
	srand(time(0));
	int opt;
	
	do {
        menu();
        printf("===> ");
        scanf("%d", &opt);

        switch(opt) {
            case 0:
                exitingMessage();
                break;
            case 1:
                insertData();
                system("cls");
                break;
            case 2:
                displayItems(); getch();
                system("cls");
                break;
            case 3:
                search(); getch();
                system("cls");
                break;
            case 4:
                updateItem(); getch();
                system("cls");
                break;
            case 5:
                hapus(); getch();
                system("cls");
                break;
            case 6:
                checkOut(); getch();
                system("cls");
                break;
            case 7:
                importData(); getch();
                system("cls");
                break;
            case 8:
                exportData(); getch();
                system("cls");
                break;
            case 9:
                viewHistory(); getch();
                system("cls");
                break;
            case 10:
            	exportLog(); getch();
            	system("cls");
            	break;
            default:
                printf("\n\tInvalid Option. Please Input A Valid Option.\n");               
                break;
        }
    } while(opt != 0);

	return 0;
}
