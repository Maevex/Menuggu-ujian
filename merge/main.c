#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct sales {
    char tanggal[20];
    char customerName[100];
    char orderedBook[100];
    char genre[100];
    float price;
    int stock;
    int orderqty;
    float pay;
};


struct bookList
{
    char name[50];
    char genre[50];
    float price;
    int stock;
};

// Define a struct to hold user data
struct UserData {
    char username[50];
    char password[50];
};

// Function to authenticate users
int login(FILE *userdata);

void entryBookData();
void displayBooks();
void entryData();
void displaySales();
void sortPriceBooks();
void sortPriceBooksDescending();
void sortStockBooks();
void sortStockBooksDescending();
void sortSalesByDate();
void sortSalesByDateDescending();
void searchBookByName();
void searchBookByGenre();
void searchBookByPrice();
void searchBookByPriceRange();
void searchBookByPriceRangeSecond();
void searchBookByPriceRangeThird();
void searchBookByStock();
void searchBookByStockRangeFirst();
void searchBookByStockRangeSecond();


int main(int argc, char const *argv[]){
    FILE *userdata;
    int loginSuccess = 0;

    // Open the user data file for reading
    userdata = fopen("userdata.txt", "r");
    if (userdata == NULL) {
        printf("Gagal membuka file userdata.\n");
        return 1;
    }

    // Perform user authentication
    loginSuccess = login(userdata);

    // Close the user data file
    fclose(userdata);

    // If login is successful, proceed to the main menu
    if (loginSuccess) {
    int choice;
    int schoice;

    do
    {
        Sleep(1500);
        printf("\n==============================================\n");
        printf("=                 Book Sales                 =\n");
        printf("==============================================\n");
        printf("1. Entry data buku\n");
        printf("2. Tampilkan buku\n");
        printf("3. Entry data sales\n");
        printf("4. Tampilkan data sales\n");
        printf("5. Sort buku berdasarkan harga Termurah\n");
        printf("6. Sort buku berdasarkan harga Termahal\n");
        printf("7. Sort buku berdasarkan stok paling sedikit\n");
        printf("8. Sort buku berdasarkan stok paling banyak\n");
        printf("9. Sort sales berdasarkan tanggal ascend\n");
        printf("10. Sort sales berdasarkan tanggal descend\n");
        printf("11. search data berdasarkan nama buku\n");
        printf("12. search data berdasarkan genre buku\n");
        printf("13. search data berdasarkan harga buku\n");
        printf("14. search data berdasarkan stok buku\n");

        printf("15. Exit\n");
        printf("Pilih: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            entryBookData();
            
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            entryData();
            break;
        case 4:
            displaySales();
            break;
        case 5:
            sortPriceBooks();    
                break;
        case 6:
            sortPriceBooksDescending();    
                break;
        case 7:
                sortStockBooks();   
                break;
        case 8:
             sortStockBooksDescending();
                break;
         case 9:
            sortSalesByDate();
                break;
        case 10:
            sortSalesByDateDescending();
            break;
        case 11:
            searchBookByName();
            break;
        case 12:
            searchBookByGenre();
            break;
        case 13:
            searchBookByPrice();
            break;
        case 14:
            searchBookByStock();
            break;
        }
    } while (choice != 15);
    system("cls");
    return 0;
    }
}

//fungsi untuk display buku

void displayBooks() {
    FILE *file;
    struct bookList book;

    file = fopen("book.txt", "r");
    if (file == NULL)
    {
        printf("Gagal membuka file.\n");
        return;
    }

     printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF)
    {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
    }
    printf(" -----------------------------------------------------\n");

    fclose(file);
}

//fungsi untuk entry data sales

void entryData() {
    FILE *file;
    FILE *file2;
    struct sales sale;
    struct bookList books;
    struct bookList updatedBooks[100]; 

    file = fopen("book.txt", "r");
    file2 = fopen("sales.txt", "a");

    if (file2 == NULL || file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    printf("Masukkan Tanggal Transaksi (dd/mm/yyyy): ");
    scanf("%s", sale.tanggal);

    printf("Masukkan Nama Pelanggan: ");
    scanf(" %[^\n]s", sale.customerName);

    printf("Masukkan Nama Buku: ");
    scanf(" %[^\n]s", sale.orderedBook);

    printf("Masukkan Jumlah Buku: ");
    scanf("%d", &sale.orderqty);

    int found = 0;
    int bookCount = 0;
    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", books.name, books.genre, &books.price, &books.stock) != EOF) {
        if (strcmp(books.name, sale.orderedBook) == 0) {
            found = 1;
            // Periksa apakah stok cukup sebelum mengurangi stok
            if (sale.orderqty > books.stock) {
                printf("Stok tidak mencukupi untuk pesanan ini.\n");
                fclose(file);
                fclose(file2);
                return;
            }
            // Memperbarui stok buku yang dipesan
            books.stock -= sale.orderqty;

        }
        updatedBooks[bookCount++] = books;
    }

    fclose(file);

    if (!found) {
        printf("Buku tidak ditemukan dalam daftar.\n");
        fclose(file2);
        return;
    }

    // Menulis kembali data buku yang telah diperbarui ke dalam file
    file = fopen("book.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        fclose(file2);
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "%s|%s|%.2f|%d\n", updatedBooks[i].name, updatedBooks[i].genre, updatedBooks[i].price, updatedBooks[i].stock);
    }

    sale.pay = sale.orderqty * books.price;
    strcpy(sale.genre, books.genre);
    sale.price = books.price;
    sale.stock = books.stock;


    fprintf(file2, "%s|%s|%s|%s|%.2f|%d|%d|%.2f\n", sale.tanggal, sale.customerName, sale.orderedBook, sale.genre, sale.price, sale.stock, sale.orderqty, sale.pay);

    printf("Data berhasil dimasukkan!\n");

    fclose(file);
    fclose(file2);
}


//fungsi untuk display sales


void displaySales() {
    FILE *fptr;
    struct sales sale;

    fptr = fopen("sales.txt", "r");
    if (fptr == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    printf(" -----------------------------------------------------------------------------------------------------\n");
    printf("|Tanggal Transaksi|Nama Pelanggan                |Buku yang dipesan         |Banyak Buku|Total Harga  |\n");
    printf(" -----------------------------------------------------------------------------------------------------\n");

    while (fscanf(fptr, " %[^|]|%[^|]|%[^|]|%d|%f\n", sale.tanggal, sale.customerName, sale.orderedBook, &sale.orderqty, &sale.pay) != EOF) {
        printf("|%-17s|%-30s|%-25s|%-12d|%-13.2f|\n", sale.tanggal, sale.customerName, sale.orderedBook, sale.orderqty, sale.pay);
    }

    printf(" -----------------------------------------------------------------------------------------------------\n");

    fclose(fptr);
}

//fungsi untuk sort harga buku

void sortPriceBooks() {
    struct bookList bookArr[100]; // Menggunakan batasan maksimum data buku

    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%f|%d\n", bookArr[count].name, bookArr[count].genre, &bookArr[count].price, &bookArr[count].stock) != EOF) {
        count++;
    }

    fclose(file);

    // Bubble sort berdasarkan harga buku (ascending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].price > bookArr[j + 1].price) {
                // Melakukan swap langsung di dalam fungsi sort
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
}

//fungsi untuk sort data buku desc

void sortPriceBooksDescending() {
    struct bookList bookArr[100]; // Menggunakan batasan maksimum data buku

    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%f|%d\n", bookArr[count].name, bookArr[count].genre, &bookArr[count].price, &bookArr[count].stock) != EOF) {
        count++;
    }

    fclose(file);

    // Bubble sort berdasarkan harga buku (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].price < bookArr[j + 1].price) {
                // Melakukan swap langsung di dalam fungsi sort
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
}


// sort untuk stok buku

void sortStockBooks() {
    struct bookList bookArr[100]; // Menggunakan batasan maksimum data buku

    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%f|%d\n", bookArr[count].name, bookArr[count].genre, &bookArr[count].price, &bookArr[count].stock) != EOF) {
        count++;
    }

    fclose(file);

    // Bubble sort berdasarkan stok buku (ascending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].stock > bookArr[j + 1].stock) {
                // Melakukan swap langsung di dalam fungsi sort
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
}

// sort untuk stok buku desc

void sortStockBooksDescending() {
    struct bookList bookArr[100]; // Menggunakan batasan maksimum data buku

    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%f|%d\n", bookArr[count].name, bookArr[count].genre, &bookArr[count].price, &bookArr[count].stock) != EOF) {
        count++;
    }

    fclose(file);

    // Bubble sort berdasarkan stok buku (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].stock < bookArr[j + 1].stock) {
                
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
}

//fungsi untuk entry data buku

void entryBookData() {
    FILE *file = fopen("book.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList newBook;

    printf("Masukkan Judul Buku: ");
    scanf(" %[^\n]s", newBook.name);

    printf("Masukkan Genre Buku: ");
    scanf(" %[^\n]s", newBook.genre);

    printf("Masukkan Harga Buku: ");
    scanf("%f", &newBook.price);

    printf("Masukkan Stok Buku: ");
    scanf("%d", &newBook.stock);

    fprintf(file, "%s|%s|%.2f|%d\n", newBook.name, newBook.genre, newBook.price, newBook.stock);
    printf("Data buku berhasil dimasukkan!\n");

    fclose(file);
}

//sort untuk sales berdasarkan tanggal

void sortSalesByDate() {
    struct sales salesArr[100]; // Maksimum 100 entri penjualan

    FILE *file = fopen("sales.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%d|%f\n", salesArr[count].tanggal, salesArr[count].customerName, salesArr[count].orderedBook, &salesArr[count].orderqty, &salesArr[count].pay) != EOF) {
        count++;
    }

    fclose(file);

    // Bubble sort berdasarkan tanggal penjualan (ascending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(salesArr[j].tanggal, salesArr[j + 1].tanggal) > 0) {
                
                struct sales temp = salesArr[j];
                salesArr[j] = salesArr[j + 1];
                salesArr[j + 1] = temp;
            }
        }
    }

    printf(" -----------------------------------------------------------------------------------------------------\n");
    printf("|Tanggal Transaksi|Nama Pelanggan                |Buku yang dipesan         |Banyak Buku|Total Harga  |\n");
    printf(" -----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("|%-17s|%-30s|%-25s|%-12d|%-13.2f|\n", salesArr[i].tanggal, salesArr[i].customerName, salesArr[i].orderedBook, salesArr[i].orderqty, salesArr[i].pay);
    }

    printf(" -----------------------------------------------------------------------------------------------------\n");
}

// sort untuk sales berdasarkan tanggal descending

void sortSalesByDateDescending() {
    struct sales salesArr[100]; // Maksimum 100 entri penjualan

    FILE *file = fopen("sales.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%d|%f\n", salesArr[count].tanggal, salesArr[count].customerName, salesArr[count].orderedBook, &salesArr[count].orderqty, &salesArr[count].pay) != EOF) {
        count++;
    }

    fclose(file);

    // Bubble sort berdasarkan tanggal penjualan (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(salesArr[j].tanggal, salesArr[j + 1].tanggal) < 0) {
                // Melakukan swap langsung di dalam fungsi sort
                struct sales temp = salesArr[j];
                salesArr[j] = salesArr[j + 1];
                salesArr[j + 1] = temp;
            }
        }
    }

    printf(" -----------------------------------------------------------------------------------------------------\n");
    printf("|Tanggal Transaksi|Nama Pelanggan                |Buku yang dipesan         |Banyak Buku|Total Harga  |\n");
    printf(" -----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("|%-17s|%-30s|%-25s|%-12d|%-13.2f|\n", salesArr[i].tanggal, salesArr[i].customerName, salesArr[i].orderedBook, salesArr[i].orderqty, salesArr[i].pay);
    }

    printf(" -----------------------------------------------------------------------------------------------------\n");
}

//fungsi untuk search data berdasarkan nama buku

void searchBookByName() {
    char bookName[50];
    printf("Masukkan nama buku yang ingin dicari: ");
    scanf(" %[^\n]s", bookName);

    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList book;
    int found = 0;

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        if (strcmp(book.name, bookName) == 0) {
            printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
            found = 1;
        }
    }

    printf(" -----------------------------------------------------\n");

    if (!found) {
        printf("Buku dengan nama '%s' tidak ditemukan.\n", bookName);
    }

    fclose(file);
}

//fungsi untuk search data berdasarkan genre buku

void searchBookByGenre() {
    char bookGenre[50];
    printf("Masukkan genre buku yang ingin dicari: ");
    scanf(" %[^\n]s", bookGenre);

    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList book;
    int found = 0;

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        if (strcmp(book.genre, bookGenre) == 0) {
            printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
            found = 1;
        }
    }

    printf(" -----------------------------------------------------\n");

    if (!found) {
        printf("Buku dengan genre '%s' tidak ditemukan.\n", bookGenre);
    }

    fclose(file);
}

//fungsi untuk search data berdasarkan harga buku 1000-10000

void searchBookByPriceRange() {
    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList book;
    int found = 0;

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        if (book.price >= 1000 && book.price <= 10000) {
            printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
            found = 1;
        }
    }

    printf(" -----------------------------------------------------\n");

    if (!found) {
        printf("Buku dalam kisaran harga 1000 hingga 10000 tidak ditemukan.\n");
    }

    fclose(file);
}

//fungsi untuk search data berdasarkan harga buku 10000-30000

void searchBookByPriceRangeSecond() {
    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList book;
    int found = 0;

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        if (book.price >= 10000 && book.price <= 30000) {
            printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
            found = 1;
        }
    }

    printf(" -----------------------------------------------------\n");

    if (!found) {
        printf("Buku dalam kisaran harga 10000 hingga 30000 tidak ditemukan.\n");
    }

    fclose(file);
}

//fungsi untuk search data berdasarkan harga buku 30000-60000

void searchBookByPriceRangeThird() {
    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList book;
    int found = 0;

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        if (book.price >= 30000 && book.price <= 60000) {
            printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
            found = 1;
        }
    }

    printf(" -----------------------------------------------------\n");

    if (!found) {
        printf("Buku dalam kisaran harga 30000 hingga 60000 tidak ditemukan.\n");
    }

    fclose(file);
}

void searchBookByPrice() {
    int choice;

    printf("1. 1,000-10,000\n");
    printf("2. 10,000-30,000\n");
    printf("3. 30,000-60,000\n");
    printf("Pilih: ");

    scanf("%d", &choice);

    switch (choice) {
        case 1:
            searchBookByPriceRange();
            break;
        
        case 2:
            searchBookByPriceRangeSecond();
            break;

        case 3:
            searchBookByPriceRangeThird();
            break;
    }
}

//fungsi untuk search data berdasarkan stok buku 1-50

void searchBookByStockRangeFirst() {
    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList book;
    int found = 0;

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        if (book.stock >= 1 && book.stock <= 50) {
            printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
            found = 1;
        }
    }

    printf(" -----------------------------------------------------\n");

    if (!found) {
        printf("Buku dalam kisaran stok 1 hingga 50 tidak ditemukan.\n");
    }

    fclose(file);
}

//fungsi untuk search data berdasarkan stok buku 50-100

void searchBookByStockRangeSecond() {
    FILE *file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList book;
    int found = 0;

    printf(" -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        if (book.stock >= 50 && book.stock <= 100) {
            printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
            found = 1;
        }
    }

    printf(" -----------------------------------------------------\n");

    if (!found) {
        printf("Buku dalam kisaran stok 50 hingga 100 tidak ditemukan.\n");
    }

    fclose(file);
}

void searchBookByStock() {
    int choice;

    printf("1. 1,000-10,000\n");
    printf("2. 10,000-30,000\n");
    printf("3. 30,000-60,000\n");
    printf("Pilih: ");

    scanf("%d", &choice);

    switch (choice) {
        case 1:
            searchBookByPriceRange();
            break;
        
        case 2:
            searchBookByPriceRangeSecond();
            break;

        case 3:
            searchBookByPriceRangeThird();
            break;
    }
}

int login(FILE *userdata) {
    struct UserData user;
    char inputUsername[50];
    char inputPassword[50];
    int found = 0;

    // Prompt the user for username and password
    printf("Masukkan username: ");
    scanf(" %[^\n]s", inputUsername);
    printf("Masukkan password: ");
    scanf(" %[^\n]s", inputPassword);

    // Check for matching username and password in the file
    while (fscanf(userdata, "%49[^|]|%49[^\n]\n", user.username, user.password) == 2) {
        if (strcmp(user.username, inputUsername) == 0 && strcmp(user.password, inputPassword) == 0) {
            found = 1;
            break;
        }
    }

    // Provide feedback to the user based on the authentication result
    if (found) {
        printf("Login berhasil!\n");
        system("cls");
        return 1; // Successful login
    } else {
        printf("Login gagal. Username atau password salah.\n");
        return 0; // Failed login
    }

}