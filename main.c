#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_TABLE_SIZE 100
//Lana Ayed Sayes 1220785 section Dr Radi
//input file towns.txt , output districts.txt
//structure of AVL tree
typedef struct AVLnode *AVLNode;

struct AVLnode {
    char district[50];        //district name (string)
    char town[50];            //town name (string)
    int population;           //population (integer)
    int elevation;            //elevation (integer)
    char hasMunicipality[4];  //"yes" or "no" (string)
    AVLNode Left;
    AVLNode Right;
    int Height;               //height for balancing
};




//hash Table Structure
typedef struct {
    char district[50];
    char town[50];
    int population;
    char status;
    int elevation;
    char hasMunicipality[4];
} HashItem;

typedef struct {
    HashItem* table;
    int size;
    int maxSize;
} HashTable;



//////////////////////////////////////////////////////////////////////

//function prototypes
int isNumber(char *str);
int isAlpha(char *str);
int validChoice();
int Height(AVLNode P);
int Max(int Lhs, int Rhs);
AVLNode MakeEmpty(AVLNode T);
AVLNode Find(char *town, AVLNode T);
AVLNode SingleRotateWithLeft(AVLNode K2);
AVLNode SingleRotateWithRight(AVLNode K1);
AVLNode DoubleRotateWithRight(AVLNode T1);
AVLNode DoubleRotateWithLeft(AVLNode T2);
AVLNode Insert(char *district, char *town, int population, int elevation, char *hasMunicipality, AVLNode T);
void PrintInOrder(AVLNode T);
AVLNode FindMin(AVLNode T);
AVLNode Delete(char *town, AVLNode T);
void readFromFile(AVLNode *tree);
void writeToFile(AVLNode T, FILE* outputFile);
void updateTownInfo(AVLNode T, char *district, char *town);
void listTownsByPopulation(AVLNode T, int population);
void listTownsByMunicipality(AVLNode T, char *status);
unsigned int hash(char* str);
void toLowerCase(char *str);
int isPrime(int n);
int nextPrime(int n);
int findNumLines();
HashTable* createHashTable();
void freeHashTable(HashTable* ht);
void rehash(HashTable* oldTable);
void insertToHashTable(char* town, int population, int elevation, char* hasMunicipality, char* district, HashTable* b);
void printHashTable(HashTable* ht);
void printTableSizeAndLoadFactor(HashTable* ht);
void searchTown(HashTable* ht, char* town);
void deleteRecord(HashTable* ht, char* town);
void readHashTableFromFile(HashTable* ht);
void writeHashTableToFile(HashTable* ht);
void clearInputBuffer();
void trimNewLine(char *str);
int readLineFields(char *line, char *district, char *town, int *population, int *elevation, char *hasMunicipality);

//utility function to check if a string is a valid number
int isNumber(char *str) {
    if (str == NULL || str[0] == '\0')
        return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }
    return 1;
}

int isAlpha(char *str) {
    if (str == NULL || str[0] == '\0')
        return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i]))
            return 0;
    }
    return 1;
}

int validChoice() {
    char input[20];

    while (1) { //infinite loop until valid input is provided
        scanf("%19s", input);
        if (isNumber(input)) {
            return atoi(input); //convert valid numeric string to integer
        } else {
            printf("Invalid input! Please enter a valid number: ");
        }
    }
}

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void trimNewLine(char *str) {
    str[strcspn(str, "\r\n")] = '\0';
}

int readLineFields(char *line, char *district, char *town, int *population, int *elevation, char *hasMunicipality) {
    trimNewLine(line);
    return sscanf(line, " %49[^:]:%49[^:]:%d:%d:%3s", district, town, population, elevation, hasMunicipality) == 5;
}

//create an empty AVL tree
AVLNode MakeEmpty(AVLNode T) {
    if (T != NULL) {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

AVLNode Find(char *town, AVLNode T) {
    if (T == NULL)
        return NULL;

    int cmp = strcmp(town, T->town);
    if (cmp < 0) {
        return Find(town, T->Left);
    } else if (cmp > 0) {
        return Find(town, T->Right);
    } else {
        return T;
    }
}

int Height(AVLNode P) {
    if (P == NULL)
        return -1;
    else
        return P->Height;
}

int Max(int Lhs, int Rhs) {
    return Lhs > Rhs ? Lhs : Rhs;
}

AVLNode SingleRotateWithLeft(AVLNode K2) {
    AVLNode K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;
    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), K2->Height) + 1;
    return K1;  //new root
}

AVLNode SingleRotateWithRight(AVLNode K1) {
    AVLNode K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;
    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Right), K1->Height) + 1;
    return K2;  //new root
}

AVLNode DoubleRotateWithRight(AVLNode T1) {
T1->Right=SingleRotateWithLeft(T1->Right);
return SingleRotateWithRight(T1);



}
AVLNode DoubleRotateWithLeft(AVLNode T2) {
T2->Left=SingleRotateWithRight(T2->Left);
return SingleRotateWithLeft(T2);



}

//function to insert an element into the AVL tree
AVLNode Insert(char *district, char *town, int population, int elevation, char *hasMunicipality, AVLNode T) {
    if (T == NULL) {
        // Create a new node
        T = malloc(sizeof(struct AVLnode));
        if (T == NULL) {
            printf("Out of space!!!\n");
            return NULL;
        }
        strcpy(T->district, district);
        strcpy(T->town, town);
        T->population = population;
        T->elevation = elevation;
        strcpy(T->hasMunicipality, hasMunicipality);
        T->Height = 0;
        T->Left = T->Right = NULL;





    } else if (strcmp(town, T->town) < 0) {
        //insert into the left subtree
        T->Left = Insert(district, town, population, elevation, hasMunicipality, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2) {
            if (strcmp(town, T->Left->town) < 0)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
        }
    } else if (strcmp(town, T->town) > 0) {
        //insert into the right subtree
        T->Right = Insert(district, town, population, elevation, hasMunicipality, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2) {
            if (strcmp(town, T->Right->town) > 0)
                T = SingleRotateWithRight(T);
            else
                T = DoubleRotateWithRight(T);
        }
    } else {
        //if the town already exists, update its information
        strcpy(T->district, district);
        T->population = population;
        T->elevation = elevation;
        strcpy(T->hasMunicipality, hasMunicipality);
    }




    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

void PrintInOrder(AVLNode T) {
    if (T != NULL) {
        PrintInOrder(T->Left);
        printf(" %s:%s:%d:%d:%s \n", T->district, T->town, T->population, T->elevation, T->hasMunicipality);
        PrintInOrder(T->Right);
    }

}
/*
void PrintPreOrder(AVLNode T) {
    if (T != NULL) {
        // Print the current node (root)
        printf("District: %s, Town: %s, Population: %d, Elevation: %d, Municipality: %s\n",
               T->district, T->town, T->population, T->elevation, T->hasMunicipality);
        // Traverse the left subtree
        PrintPreOrder(T->Left);
        // Traverse the right subtree
        PrintPreOrder(T->Right);
    }
}

*/
// Function to delete a node from the AVL tree
AVLNode FindMin(AVLNode T) {
    if (T == NULL)
        return NULL;
    while (T->Left != NULL) {
        T = T->Left;
    }
    return T;
}

AVLNode Delete(char *town, AVLNode T) {
    if (T == NULL) {
        return NULL;
    }

    int cmp = strcmp(town, T->town);
    if (cmp < 0) {
        T->Left = Delete(town, T->Left);
    } else if (cmp > 0) {
        T->Right = Delete(town, T->Right);
    } else {
        // Node to be deleted found
        if (T->Left == NULL || T->Right == NULL) {
            AVLNode temp = T->Left ? T->Left : T->Right;
            free(T);
            return temp;
        } else {
            AVLNode temp = FindMin(T->Right);
            strcpy(T->district, temp->district);
            strcpy(T->town, temp->town);

            T->population = temp->population;
            T->elevation = temp->elevation;

            strcpy(T->hasMunicipality, temp->hasMunicipality);
            T->Right = Delete(temp->town, T->Right);
        }
    }

    if (T == NULL)
        return NULL;

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    int balance=Height(T->Left)-Height(T->Right);
    if (balance>1)
    {
    if (Height(T->Left->Left )>=Height(T->Left->Right))
    {
    return SingleRotateWithLeft(T);

    }
else
    return DoubleRotateWithLeft(T);}


else if(balance<-1){


    if (Height(T->Right->Right )>=Height(T->Right->Left))
    {
    return SingleRotateWithRight(T);

    }
 else return DoubleRotateWithRight(T);
}
    return T;
    }

//read data from the file and insert it into the AVL tree
void readFromFile(AVLNode *tree) {
    FILE *input = fopen("districts.txt", "r");
    if (input == NULL) {
        printf("Error opening input file. Starting with empty AVL tree.\n");
        return;
    }
    char district[50], town[50], hasMunicipality[4];
    int population, elevation;
    char line[256];

    //read each line and insert into the AVL tree
    while (fgets(line, sizeof(line), input)) {
        if (readLineFields(line, district, town, &population, &elevation, hasMunicipality)) {
            *tree = Insert(district, town, population, elevation, hasMunicipality, *tree);
        }
    }

    fclose(input);
}

void writeToFile(AVLNode T,FILE* outputFile) {//using recursion to print all tree information till reaching null
    if (T != NULL) {
        writeToFile(T->Left,outputFile);  //traverse left subtree
        //write the town information to the file
        fprintf(outputFile, "%s:%s:%d:%d:%s\n", T->district, T->town, T->population, T->elevation, T->hasMunicipality);
        writeToFile(T->Right,outputFile);  // right subtree
    }


}

void updateTownInfo(AVLNode T, char *district, char *town) {
    AVLNode node = Find(town, T);
    if (node != NULL) {
        printf("Found town: %s, %s\n", node->district, node->town);
        strcpy(node->district, district);
        printf("Enter new population: ");
        scanf("%d", &node->population);
        printf("Enter new elevation: ");
        scanf("%d", &node->elevation);
        printf("Enter new municipality status (yes/no): ");
        scanf("%3s", node->hasMunicipality);
    } else {
        printf("Town not found.\n");
    }
}

//list all towns with population greater than a given number
void listTownsByPopulation(AVLNode T, int population) {
    if (T != NULL) {
        listTownsByPopulation(T->Left, population);
        if (T->population > population) {
            printf("District: %s, Town: %s, Population: %d, Elevation: %d, Municipality: %s\n",
                   T->district, T->town, T->population, T->elevation, T->hasMunicipality);
        }
        listTownsByPopulation(T->Right, population);
    }
}

//function to list all towns with a specific municipality status
void listTownsByMunicipality(AVLNode T, char *status) {
    if (T != NULL) {
        listTownsByMunicipality(T->Left, status);
        if (strcmp(T->hasMunicipality, status) == 0) {
            printf("District: %s, Town: %s, Population: %d, Elevation: %d, Municipality: %s\n",
                   T->district, T->town, T->population, T->elevation, T->hasMunicipality);
        }
        listTownsByMunicipality(T->Right, status);
    }
}

unsigned int hash(char* str) {
    unsigned int hashVal = 0;
    for (int i = 0; i < 5 && str[i] != '\0'; i++) {
        hashVal = (hashVal << 5) + (unsigned char)str[i];
    }
    return hashVal % HASH_TABLE_SIZE;
}




void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

int isPrime(int n) {
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;

    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int nextPrime(int n) {
    if (n < 3)
        return 3;
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

int findNumLines() {
    FILE* input = fopen("towns.txt", "r");
    if (input == NULL) {
        return 5;
    }

    int count = 0;
    char line[256];
    char district[50], town[50], hasMunicipality[4];
    int population, elevation;

    while (fgets(line, sizeof(line), input)) {
        if (readLineFields(line, district, town, &population, &elevation, hasMunicipality)) {
            count++;
        }
    }

    fclose(input);
    if (count == 0)
        count = 5;
    return count;
}

HashTable* createHashTable() {
    //intialize hash table
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    hashTable->maxSize=nextPrime(2*findNumLines());
    hashTable->table = (HashItem*)malloc(sizeof(HashItem)*(hashTable->maxSize));
    if (hashTable->table == NULL) {
        printf("Memory allocation failed.\n");
        free(hashTable);
        return NULL;
    }
    hashTable->size=0;

    for (int i = 0; i < hashTable->maxSize; i++) {
        hashTable->table[i].status = 'E';
        hashTable->table[i].district[0] = '\0';
        hashTable->table[i].town[0] = '\0';
        hashTable->table[i].population = 0;
        hashTable->table[i].elevation = 0;
        hashTable->table[i].hasMunicipality[0] = '\0';
    }

    return hashTable;
}

void freeHashTable(HashTable* ht) {
    if (ht != NULL) {
        free(ht->table);
        free(ht);
    }
}

void rehash(HashTable* oldTable) {
    //new table size
    int newSize = nextPrime(2 * oldTable->maxSize);


    HashItem* newTable = (HashItem*)malloc(sizeof(HashItem) * newSize);
    if (newTable == NULL) {
        printf("Memory allocation failed during rehashing.\n");
        return;
    }

    int size=oldTable->maxSize;
    HashItem* old=oldTable->table;
    oldTable->table=newTable;
    oldTable->size=0;
    oldTable->maxSize=newSize;

    //all slots in the new table as empty
    for (int i = 0; i < newSize; i++) {
        newTable[i].status = 'E';
        newTable[i].district[0] = '\0';
        newTable[i].town[0] = '\0';
        newTable[i].population = 0;
        newTable[i].elevation = 0;
        newTable[i].hasMunicipality[0] = '\0';
    }

    //reinsert all valid items from the old table into the new table
    for (int i = 0; i < size; i++) {
        if (old[i].status == 'O') {
            insertToHashTable(
                old[i].town,
                old[i].population,
                old[i].elevation,
                old[i].hasMunicipality,
                old[i].district,oldTable
            );

        }

    }

    //free memory allocated for the old table
    free(old);


    printf("Rehashing completed. New table size: %d.\n", newSize);

}

void insertToHashTable(char* town, int population, int elevation, char* hasMunicipality, char* district, HashTable* b) {
    if (b == NULL || b->table == NULL)
        return;

    char townCopy[50];
    strcpy(townCopy, town);

    //convert town to lowercase for case-insensitive comparison
    toLowerCase(townCopy);

    //calculate the initial hash index
    unsigned int index = hash(townCopy) % b->maxSize;

    // Handle collisions using linear probing
    int i = 0;
    while (b->table[index].status == 'O') {
        //if the town already exists, update its details
        if (strcmp(b->table[index].town, townCopy) == 0) {
            strcpy(b->table[index].district, district);
            b->table[index].population = population;
            b->table[index].elevation = elevation;
            strcpy(b->table[index].hasMunicipality, hasMunicipality);
            printf("Updated existing town '%s' at index %d.\n", townCopy, index);
            return;
        }
        //move to the next index
        index = (index + 1) % b->maxSize;
        i++;
    }

    //insert the new record in the first available spot
    strcpy(b->table[index].district, district);
    strcpy(b->table[index].town, townCopy);
    b->table[index].population = population;
    b->table[index].elevation = elevation;
    strcpy(b->table[index].hasMunicipality, hasMunicipality);
    b->table[index].status = 'O'; // Mark the slot as occupied
    b->size++;

    printf("Inserted town '%s' at index %d after %d collisions.\n", townCopy, index, i);

    //calculate the load factor
    double loadFactor = (double)b->size / b->maxSize;

    //rehash if the load factor exceeds 0.7
    if (loadFactor >= 0.7) {
        printf("Load factor %.2f exceeds 0.7. Rehashing...\n", loadFactor);
        rehash(b);
    }
}




void printHashTable(HashTable* ht) {
    printf("Hash Table:\n");
    for (int i = 0; i < ht->maxSize; i++) {
        if (ht->table[i].status == 'O') { //occupied slot
            printf("Index %d: District: %s, Town: %s, Population: %d, Elevation: %d, Municipality: %s\n",
                   i, ht->table[i].district, ht->table[i].town, ht->table[i].population,
                   ht->table[i].elevation, ht->table[i].hasMunicipality);
        } else if (ht->table[i].status == 'D') { //deleted slot
            printf("Index %d: Deleted\n", i);
        } else { // Empty slot
            printf("Index %d: Empty\n", i);
        }
    }
}


void printTableSizeAndLoadFactor(HashTable* ht) {
    int occupied = ht->size; //number of occupied slots
    float loadFactor = (float)occupied / ht->maxSize;

    printf("Hash Table Size: %d\n", ht->maxSize);
    printf("Number of Entries: %d\n", occupied);
    printf("Load Factor: %.2f\n", loadFactor);
}


void searchTown(HashTable* ht, char* town) {
    char townCopy[50];
    strcpy(townCopy, town);
    toLowerCase(townCopy); //convert town name to lowercase for case-insensitive comparison

    unsigned int index = hash(townCopy) % ht->maxSize; //initial index
    int collisions = 0;

    while (ht->table[index].status != 'E') { //continue until an empty slot is found
        if (ht->table[index].status == 'O' && strcmp(ht->table[index].town, townCopy) == 0) {
            printf("Town: %s found at Index %d with %d collisions.\n", townCopy, index, collisions);
            return;
        }
        index = (index + 1) % ht->maxSize; //move to the next slot
        collisions++;
        if (collisions >= ht->maxSize)
            break;
    }

    printf("Town: %s not found after %d collisions.\n", townCopy, collisions);
}



void deleteRecord(HashTable* ht, char* town) {
    char townCopy[50];
    strcpy(townCopy, town);
    toLowerCase(townCopy); //convert town name to lowercase for case-insensitive comparison

    unsigned int index = hash(townCopy) % ht->maxSize; //initial index
    int collisions = 0;

    while (ht->table[index].status != 'E') { //continue until an empty slot is found
        if (ht->table[index].status == 'O' && strcmp(ht->table[index].town, townCopy) == 0) {
            ht->table[index].status = 'D'; // Mark the slot as deleted
            printf("Town: %s deleted at Index %d after %d collisions.\n", townCopy, index, collisions);
            ht->size--;
            return;
        }
        index = (index + 1) % ht->maxSize; //move to the next slot
        collisions++;
        if (collisions >= ht->maxSize)
            break;
    }

    printf("Town: %s not found after %d collisions.\n", townCopy, collisions);
}

void readHashTableFromFile(HashTable* ht) {
    FILE* input = fopen("towns.txt", "r");
    if (input == NULL) {
        printf("Error opening file towns.txt. Starting with empty hash table.\n");
        return;
    }

    char district[50], town[50], hasMunicipality[4];
    int population, elevation;
    char line[256];


    while (fgets(line, sizeof(line), input)) {    //read each line and insert into the hash table
        if (readLineFields(line, district, town, &population, &elevation, hasMunicipality)) {
            insertToHashTable(town, population, elevation, hasMunicipality,district,ht);
        }

    }

    fclose(input);
    printf("Data loaded from towns.txt successfully.\n");
}

void writeHashTableToFile(HashTable* ht) {
    FILE* output = fopen("towns.txt", "w");  //open file for writing
    if (output == NULL) {
        printf("Error opening file towns.txt.\n");
        return;
    }


    for (int i = 0; i < ht->maxSize; i++) {    //loop through the hash table and write only valid entries to the file
        if (ht->table[i].status == 'O') { //write occupied slots
            fprintf(output, "%s:%s:%d:%d:%s\n",
                    ht->table[i].district,
                    ht->table[i].town,
                    ht->table[i].population,
                    ht->table[i].elevation,
                    ht->table[i].hasMunicipality);
        }
    }

    fclose(output);
    printf("Data saved to towns.txt successfully.\n");
}


int main() {
    FILE *outputFile;
    AVLNode tree = NULL;

    HashTable* hashTable = createHashTable();
    if (hashTable == NULL) {
        return 1;
    }



    int option;
    int treeChoice;
    int dataModified = 0;
    //read data from the file and populate the AVL tree
    readFromFile(&tree);

    while (1) {
        printf("\nChoose Tree Type:\n");
        printf("1. AVL Tree\n");
        printf("2. Hash Table\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        treeChoice = validChoice();

        if (treeChoice == 1) {
            printf("You chose AVL Tree.\n");

            while (1) {
                printf("\nMenu:\n");
                printf("1. Insert a new town\n");
                printf("2. Update town information\n");
                printf("3. List all towns (in alphabetical order)\n");
                printf("4. List towns with population greater than a number\n");
                printf("5. List towns with a municipality or not\n");
                printf("6. Delete a town\n");
                printf("7. Save all towns to a file\n");
                printf("8. Exit\n");
                printf("Enter your choice: ");
                option = validChoice();

                switch (option) {
                    case 1: {
                        char district[50], town[50], hasMunicipality[4], input[20];
                        int population, elevation;

                        printf("Enter district name: ");
                        scanf("%49s", district);
                        while (!isAlpha(district)) {
                            printf("Invalid input! District name must contain only letters. Try again: ");
                            scanf("%49s", district);
                        }

                        printf("Enter town name: ");
                        scanf("%49s", town);
                        while (!isAlpha(town)) {
                            printf("Invalid input! Town name must contain only letters. Try again: ");
                            scanf("%49s", town);
                        }

                        printf("Enter population: ");
                        scanf("%19s", input);
                        while (!isNumber(input)) {
                            printf("Invalid input! Please enter a valid number: ");
                            scanf("%19s", input);
                        }
                        population = atoi(input);

                        printf("Enter elevation: ");
                        scanf("%19s", input);
                        while (!isNumber(input)) {
                            printf("Invalid input! Please enter a valid number: ");
                            scanf("%19s", input);
                        }
                        elevation = atoi(input);

                        printf("Does it have a municipality (yes/no)? ");
                        scanf("%3s", hasMunicipality);
                        while (strcmp(hasMunicipality, "yes") != 0 &&
                               strcmp(hasMunicipality, "no") != 0 &&
                               strcmp(hasMunicipality, "Yes") != 0 &&
                               strcmp(hasMunicipality, "No") != 0) {
                            printf("Invalid input! Please enter 'yes' or 'no': ");
                            scanf("%3s", hasMunicipality);
                        }
                        toLowerCase(hasMunicipality);

                        tree = Insert(district, town, population, elevation, hasMunicipality, tree);
                      printf("Town %s added\n",town);
                       dataModified = 1;
                        break;
                    }
                    case 2: {
                        char district[50], town[50], input[20];
                        printf("Enter district name: ");
                        scanf("%49s", district);
                        while (!isAlpha(district)) {
                            printf("Invalid input! District name must contain only letters. Try again: ");
                            scanf("%49s", district);
                        }

                        printf("Enter town name: ");
                        scanf("%49s", town);
                        while (!isAlpha(town)) {
                            printf("Invalid input! Town name must contain only letters. Try again: ");
                            scanf("%49s", town);
                        }

                        AVLNode node = Find(town, tree);
                        if (node != NULL) {
                            printf("Found town: %s, %s\n", node->district, node->town);
                            strcpy(node->district, district);

                            printf("Enter new population: ");
                            scanf("%19s", input);
                            while (!isNumber(input)) {
                                printf("Invalid input! Please enter a valid number: ");
                                scanf("%19s", input);
                            }
                            node->population = atoi(input);

                            printf("Enter new elevation: ");
                            scanf("%19s", input);
                            while (!isNumber(input)) {
                                printf("Invalid input! Please enter a valid number: ");
                                scanf("%19s", input);
                            }
                            node->elevation = atoi(input);

                            printf("Enter new municipality status (yes/no): ");
                            scanf("%3s", node->hasMunicipality);
                            while (strcmp(node->hasMunicipality, "yes") != 0 && strcmp(node->hasMunicipality, "no") != 0 &&
                                   strcmp(node->hasMunicipality, "Yes") != 0 && strcmp(node->hasMunicipality, "No") != 0) {
                                printf("Invalid input! Please enter 'yes' or 'no': ");
                                scanf("%3s", node->hasMunicipality);
                            }
                            toLowerCase(node->hasMunicipality);

                           // writeToFile(tree);
                           dataModified = 1;
                        } else {
                            printf("Town not found.\n");
                        }
                        break;
                    }
                    case 3:
                        PrintInOrder(tree);
                        break;
                    case 4: {
                        char input[20];
                        int population;

                        printf("Enter minimum population: ");
                        scanf("%19s", input);
                        while (!isNumber(input)) {
                            printf("Invalid input! Please enter a valid number: ");
                            scanf("%19s", input);
                        }
                        population = atoi(input);

                        listTownsByPopulation(tree, population);
                        break;
                    }
                    case 5: {
                        char status[4];
                        printf("Enter municipality status (yes/no): ");
                        scanf("%3s", status);
                        while (strcmp(status, "yes") != 0 && strcmp(status, "no") != 0 &&
                               strcmp(status, "Yes") != 0 && strcmp(status, "No") != 0) {
                            printf("Invalid input! Please enter 'yes' or 'no': ");
                            scanf("%3s", status);
                        }
                        toLowerCase(status);
                        listTownsByMunicipality(tree, status);

                        break;
                    }
                 case 6: {
                char  town[50];
                printf("Enter town to delete: ");
                scanf("%49s", town);
     if (Find(town,tree)!=NULL){

                tree = Delete(town,tree);
                printf("The wanted town has been deleted\n");
                dataModified = 1;

     }
     else {
        printf("Town not found.\n");
     }

    break;
}


         case 7: // Save all towns to a file

        // Open the file in write mode to clear it first
        outputFile = fopen("towns.txt", "w");
        if (outputFile == NULL) {
            printf("Error opening the file for saving data.\n");
        } else {

            // Now write the updated data to the file
            writeToFile(tree,outputFile);
           fclose(outputFile);


        printf("Data  saved to file.\n");
        dataModified = 0;
        }

    break;

                    case 8: // Exit



                        printf("Exiting AVL Tree operations.\n");
                        break;




                    default:
                        printf("Invalid option.\n");
                }
                if (option == 8) break; // Exit AVL Tree menu
            }
        }
 /////////////////////////////////////////////////////////////
if (treeChoice == 2) {
    printf("You chose Hash Table.\n");

    //reload the hash table from towns.txt each time the hash menu is opened
    freeHashTable(hashTable);
    hashTable = createHashTable();
    if (hashTable == NULL) {
        tree = MakeEmpty(tree);
        return 1;
    }
    readHashTableFromFile(hashTable); // Load data from towns.txt

    while (1) {
        printf("\nOptions:\n");
        printf("1 - Print Hash Table\n");
        printf("2 - Print Table Size and Load Factor\n");
        printf("3 - Insert a new record\n");
        printf("4 - Search for a town\n");
        printf("5 - Delete a record\n");
        printf("6 - Save data to file\n");
        printf("7 - Exit\n");
        printf("Enter your choice: ");

        option = validChoice();

        switch (option) {
            case 1:
                printHashTable(hashTable);
                break;

            case 2:
                printTableSizeAndLoadFactor(hashTable);
                break;

            case 3: {
                char district[50], town[50], hasMunicipality[4];
                char input[50];
                int population, elevation;

                printf("Enter district name: ");
                scanf("%49s", district);
                while (!isAlpha(district)) {
                    printf("Invalid input! District name must contain only letters. Try again: ");
                    scanf("%49s", district);
                }

                printf("Enter town name: ");
                scanf("%49s", town);
                while (!isAlpha(town)) {
                    printf("Invalid input! Town name must contain only letters. Try again: ");
                    scanf("%49s", town);
                }

                printf("Enter population: ");
                scanf("%49s", input);
                while (!isNumber(input)) {
                    printf("Invalid input! Population must be a valid number. Try again: ");
                    scanf("%49s", input);
                }
                population = atoi(input);

                printf("Enter elevation: ");
                scanf("%49s", input);
                while (!isNumber(input)) {
                    printf("Invalid input! Elevation must be a valid number. Try again: ");
                    scanf("%49s", input);
                }
                elevation = atoi(input);

                printf("Has Municipality (yes/no): ");
                scanf("%3s", hasMunicipality);
                while (strcmp(hasMunicipality, "yes") != 0 && strcmp(hasMunicipality, "no") != 0 &&
                       strcmp(hasMunicipality, "Yes") != 0 && strcmp(hasMunicipality, "No") != 0) {
                    printf("Invalid input! Municipality status must be 'yes' or 'no'. Try again: ");
                    scanf("%3s", hasMunicipality);
                }
                toLowerCase(hasMunicipality);

                insertToHashTable(town, population, elevation, hasMunicipality, district, hashTable);
                break;
            }

            case 4: {
                char town[50];
                printf("Enter town name to search: ");
                scanf("%49s", town);
                while (!isAlpha(town)) {
                    printf("Invalid input! Town name must contain only letters. Try again: ");
                    scanf("%49s", town);
                }
                searchTown(hashTable, town);
                break;
            }

            case 5: {
                char town[50];
                printf("Enter town name to delete: ");
                scanf("%49s", town);
                while (!isAlpha(town)) {
                    printf("Invalid input! Town name must contain only letters. Try again: ");
                    scanf("%49s", town);
                }
                deleteRecord(hashTable, town);
                break;
            }

            case 6:
                writeHashTableToFile(hashTable);
                break;

            case 7:
                printf("Exiting Hash Table operations.\n");
                break;

            default:
                printf("Invalid option. Please try again.\n");
        }

        if (option == 7) break; // Exit Hash Table menu
    }
} else if (treeChoice == 3) {
    if (dataModified) {
        printf("You have unsaved AVL changes. Please save from AVL menu if needed.\n");
    }
    printf("Exiting program.\n");
    break;
} else if (treeChoice != 1) {
    printf("Invalid choice! Please choose 1, 2, or 3.\n");
}}

    tree = MakeEmpty(tree);
    freeHashTable(hashTable);
    return 0;
}
