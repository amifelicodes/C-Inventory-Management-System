#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for storing product data
typedef struct {
    int id;
    char name[50]; //max of 49 letters + null variable
    float price;
    int quantity;
} Product;

//global variables
Product* inventory = NULL; //inventory is a pointer and a struct
int productCount = 0;
int inventoryCapacity = 0;

//function declarations
int ReadFile();
void addProduct();
void deleteProduct();
void printInventory(Product* inventory, int count);
void updateMenu(Product* inventory, int count);
void updatePrice(Product* inventory, int count);
void updateQuantity(Product* inventory, int count);
void searchMenu(Product* inventory, int count);
void searchByName(Product* inventory, int count);
void searchByPrice(Product* inventory, int count);
void sortMenu(Product* inventory, int* count);
void sortByNameAscending(inventory, count);
void sortByNameDescending(inventory, count);
void sortByPriceAscending(inventory, count);
void sortByPriceDescending(inventory, count);
void sortByQuantityAscending(inventory, count);
void sortByQuantityDescending(inventory, count);
void SaveToFile(Product* inventory, int count);

int main() {
    int choice, running = 1;

    inventoryCapacity = 10;  //sets initial memory block size to 10

    inventory = malloc(inventoryCapacity * sizeof(Product));
    if (inventory == NULL) { //malloc check
        printf("Initial memory allocation failed.\n");
        return 1;
    }

    //reads file and count total amount of products
    productCount = ReadFile();

    while (running) {
        printf("\n-----------------------------\n");
        printf("Inventory Management System\n");
        printf("-----------------------------\n");

        printf("1. Display Inventory\n");
        printf("2. Add a New Product\n");
        printf("3. Update Product Details\n");
        printf("4. Delete a Product\n");
        printf("5. Search for a Product\n");
        printf("6. Sort Inventory\n");
        printf("7. Save and Exit\n");

        printf("\nEnter choice: ");
        scanf(" %d", &choice);
        while (getchar() != '\n'); //removes newline

        switch (choice) {
        case 1:
            printInventory(inventory, productCount);
            break;
        case 2:
            addProduct(inventory, &productCount);
            break;
        case 3:
            updateMenu(inventory, productCount);
            break;
        case 4:
            deleteProduct(inventory, &productCount);
            break;
        case 5:
            searchMenu(inventory, productCount);
            break;
        case 6:
            sortMenu(inventory, &productCount);
            printInventory(inventory, productCount);
            break;
        case 7:
            printf("\nSaving and exiting...\n");
            SaveToFile(inventory, productCount);
            running = 0;
            break;
        default:
            printf("Invalid input. Try again.\n");
            break;
        }
    }
    free(inventory);
    return 0;
}

//function for loading inventory from csv file
int ReadFile() {
    //reads csv file
    FILE* fp = fopen("StoreInv.csv", "r");
    if (fp == NULL) {
        printf("Error: Inventory file not found.\n");
        exit(EXIT_FAILURE);
    }

    //skips header
    char header[256];//creates temp storage for the first row
    fgets(header, sizeof(header), fp); //reads until it reaches newline and stores it into header

    int i = 0;
    //scans file until there are no more values to read
    while (fscanf(fp, "%d , %49[^,], %f , %d",
        &inventory[i].id,
        inventory[i].name,
        &inventory[i].price,
        &inventory[i].quantity) == 4) {
        i++;

        //if memory allocated is not enough, it is resized and doubled
        if (i >= inventoryCapacity) {
            inventoryCapacity *= 2;
            Product* temp = realloc(inventory, inventoryCapacity * sizeof(Product));
            if (temp == NULL) {
                printf("Warning: Could not expand memory during file read\n");
                fclose(fp);
                return i;
            }
            inventory = temp;
        }
    }
    fclose(fp);
    return i;
}

//function for adding products to inventory
void addProduct() {

    //allocates more memory if inventory reaches initial capacity
    if (productCount >= inventoryCapacity) {
        Product* temp = realloc(inventory, (inventoryCapacity * 2) * sizeof(Product));
        if (temp == NULL) {
            printf("Failed to expand inventory. Product not added.\n");
            return;
        }
        inventory = temp;
        inventoryCapacity *= 2;
    }

    int new_id = 101 + productCount;

    //assigns a name
    printf("Enter product name: ");
    fgets(inventory[productCount].name, 50, stdin);
    inventory[productCount].name[strcspn(inventory[productCount].name, "\n")] = '\0'; //removes newline, replaces it with null

    //assigns a price
    printf("Enter price ($): ");
    while (scanf("%f", &inventory[productCount].price) != 1 || inventory[productCount].price <= 0) { //ensures price is a float and over 0
        printf("Invalid price. Try again: ");
        while (getchar() != '\n'); 
    }

    //assigns a quantity
    printf("Enter quantity: ");
    while (scanf("%d", &inventory[productCount].quantity) != 1 || inventory[productCount].quantity < 0) { //ensures price is an int and over 0
        printf("Invalid quantity. Try again: ");
        while (getchar() != '\n');
    }

    //assigns new id
    inventory[productCount].id = new_id;

    productCount++;
    //saves to csv file
    SaveToFile(inventory, productCount);
    printf("Product added (ID: %d).\n\n", new_id);
}

//function to delete products from inventory
void deleteProduct() {

    if (productCount == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int id;
    printf("\nEnter product ID to delete: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    //finds matching id and overwrites it with info of succeeding products
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) {
            for (int j = i; j < productCount - 1; j++) {
                inventory[j] = inventory[j + 1];
            }
            productCount--;

           //resizes memory block if there are less products than initial inventory capacity
            if (inventoryCapacity > 10 && productCount < inventoryCapacity / 2) {
                inventoryCapacity /= 2;
                Product* temp = realloc(inventory, inventoryCapacity * sizeof(Product));
                if (temp != NULL) {
                    inventory = temp;
                }
            }

            //saves to csv file
            SaveToFile(inventory, productCount);
            printf("Product deleted successfully.\n");
            return;
        }
    }
    printf("Product ID not found.\n");
}

//function to print/display inventory
void printInventory(Product* inventory, int count) {

    printf("\n-------------------------------------------------------\n");
    printf("Product ID | Product Name | Price ($) | Stock Quantity\n");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-10d | %-12s | %-9.2f | %d\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
    }
}

//function for the menu of update choices
void updateMenu(Product* inventory, int count) {

    int choice;

    printf("\n-----------------------------\n");
    printf("Update Product Details\n");
    printf("-----------------------------\n");
    printf("1. Update Price\n");
    printf("2. Update Stock Quantity\n");
    printf("3. Return to Main Menu\n");

    printf("\nEnter choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n'); 

    switch (choice) {
    case 1:
        updatePrice(inventory, count);
        break;
    case 2:
        updateQuantity(inventory, count);
        break;
    case 3:
        return;
    default:
        printf("Invalid choice.\n"); //returns to main menu
    }
}

//function to update price
void updatePrice(Product* inventory, int count) {

    int id;
    float newPrice;

    printf("\nEnter product ID to update: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    //finds matching id and assigns new price
    for (int i = 0; i < count; i++) {
        if (inventory[i].id == id) {
            printf("Current price: $%.2f\n", inventory[i].price);
            printf("Enter new price: ");
            scanf("%f", &newPrice);
            while (getchar() != '\n');

            //ensures price is above 0
            if (newPrice > 0) {
                inventory[i].price = newPrice;
                printf("Price updated successfully.\n");
                SaveToFile(inventory, count); //saves to csv file
            }
            else {
                printf("Invalid price. Must be positive number.\n");
            }
            return;
        }
    }
    printf("Product ID not found.\n");
}

//function to update quantity
void updateQuantity(Product* inventory, int count) {

    int id, newQty;
    printf("\nEnter product ID to update: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    //finds matching id and assigns new quantity
    for (int i = 0; i < count; i++) {
        if (inventory[i].id == id) {
            printf("Current quantity: %d\n", inventory[i].quantity);
            printf("Enter new quantity: ");
            scanf("%d", &newQty);
            while (getchar() != '\n');

            if (newQty >= 0) {
                inventory[i].quantity = newQty;
                printf("Quantity updated successfully.\n");
                SaveToFile(inventory, count); //saves to csv file
            }
            else {
                printf("Invalid quantity. Must be non-negative number.\n");
            }
            return;
        }
    }
    printf("Product ID not found.\n");
}

//function for menu of search choices
void searchMenu(Product* inventory, int count) {
    int choice;
    printf("\n-----------------------------\n");
    printf("Search Products\n");
    printf("-----------------------------\n");
    printf("1. Search by Name\n");
    printf("2. Search by Price\n");
    printf("3. Return to Main Menu\n");

    printf("\nEnter choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
    case 1:
        searchByName(inventory, count);
        break;
    case 2:
        searchByPrice(inventory, count);
        break;
    case 3:
        return;
    default:
        printf("Invalid choice.\n");
    }
}

//function for searching by name
void searchByName(Product* inventory, int count) {

    //sets char limit to the same as the char limit in struct
    char searchName[50];

    printf("\nEnter product name to search: ");
    fgets(searchName, 50, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    printf("\nSearch Results:\n");
    printf("-------------------------------------------------------\n");
    printf("Product ID | Product Name | Price ($) | Stock Quantity\n");
    printf("-------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(inventory[i].name, searchName) != NULL) { //searches inventory names for same characters in searchName (case-sensitive)
            printf("%-10d | %-12s | %-9.2f | %d\n",
                inventory[i].id, inventory[i].name,
                inventory[i].price, inventory[i].quantity);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No products found matching '%s'\n", searchName);
    }
}

//function for searching by price
void searchByPrice(Product* inventory, int count) {

    float searchPrice;

    printf("\nEnter exact price to search for: ");
    scanf("%f", &searchPrice);
    while (getchar() != '\n');

    printf("\nSearch Results (price = $%.2f):\n", searchPrice);
    printf("-------------------------------------------------------\n");
    printf("Product ID | Product Name | Price ($) | Stock Quantity\n");
    printf("-------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < count; i++) {

        if (inventory[i].price == searchPrice) {
            printf("%-10d | %-12s | %-9.2f | %d\n",
                inventory[i].id, inventory[i].name,
                inventory[i].price, inventory[i].quantity);
            found = 1;
        }
    }

    if (!found) {
        printf("No products found with price $%.2f\n", searchPrice);
    }
}

//function for menu of sort choices
void sortMenu(Product* inventory, int *count) {
    int choice;
    printf("\n-----------------------------\n");
    printf("Sort Inventory\n");
    printf("-----------------------------\n");
    printf("1. Sort by Name (A-Z)\n");
    printf("2. Sort by Name (Z-A)\n");
    printf("3. Sort by Price (Low-High)\n");
    printf("4. Sort by Price (High-Low)\n");
    printf("5. Sort by Quantity (Low-High)\n");
    printf("6. Sort by Quantity (High-Low)\n");
    printf("7. Return to Main Menu\n");

    printf("\nEnter choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
    case 1:
        sortByNameAscending(inventory, *count);
        break;
    case 2:
        sortByNameDescending(inventory, *count);
        break;
    case 3:
        sortByPriceAscending(inventory, *count);
        break;
    case 4:
        sortByPriceDescending(inventory, *count);
        break;
    case 5:
        sortByQuantityAscending(inventory, *count);
        break;
    case 6:
        sortByQuantityDescending(inventory, *count);
        break;
    case 7:
        return;
    default:
        printf("Invalid choice.\n");
    }
    printf("Inventory sorted successfully.\n");
}

//sorts inventory from A-Z
void sortByNameAscending(Product* inventory, int count) {
    
    //rearranges order of products in array
    for (int i = 0; i < count - 1; i++) { //loops until all products have gotten a chance to be compared

        for (int j = 0; j < count - i - 1; j++) { //compares adjacent products

            //swaps when first product is alphabetically later than second
            if (strcmp(inventory[j].name, inventory[j + 1].name) > 0) { //positive number is returned if compared products are not in order
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
}

//sorts inventory from Z-A
void sortByNameDescending(Product* inventory, int count) {

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            //swaps when first product is alphabetically earlier than second
            if (strcmp(inventory[j].name, inventory[j + 1].name) < 0) { //negative number is returned if compared products are not in order
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
}

//sorts inventory from low-high
void sortByPriceAscending(Product* inventory, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (inventory[j].price > inventory[j + 1].price) { //first product is greater than second product, it switches it to a later position
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
}

//sorts inventory from high-low
void sortByPriceDescending(Product* inventory, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (inventory[j].price < inventory[j + 1].price) { //first product is less than second product, it switches it to a later position
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
}

//sorts inventory from low-high
void sortByQuantityAscending(Product* inventory, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (inventory[j].quantity > inventory[j + 1].quantity) { //first product is greater than second product, it switches it to a later position
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
}

//sorts inventory from high-low
void sortByQuantityDescending(Product* inventory, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (inventory[j].quantity < inventory[j + 1].quantity) { //first product is less than second product, it switches it to a later position
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
}

//saves inventory in csv file
void SaveToFile(Product* inventory, int count) {
    //opens and rewrites entire file
    FILE* fp = fopen("StoreInv.csv", "w");
    if (!fp) {
        printf("Error: Could not save file.\n");
        return;
    }

    //write header first
    fprintf(fp, "ID,Name,Price ($),Quantity\n");

    //write all products
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%.2f,%d\n",
            inventory[i].id,
            inventory[i].name,
            inventory[i].price,
            inventory[i].quantity);
    }
    fclose(fp);
}