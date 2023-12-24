#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include <ctype.h>

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


struct UserData {
    char username[50];
    char password[50];
};


int login(FILE *userdata, char *loggedInUsername);

void pause();
void exitAnimation();
void entryBookData();
void displayBooks();
void entryData();
void displaySales();
void sort();
void sortPriceBooks();
void sortPriceBooksDescending();
void sortStockBooks();
void sortStockBooksDescending();
void sortSalesByDate();
void sortSalesByDateDescending();
void search();
void searchBookByName();
void searchBookByGenre();
void searchBookByPrice();
void searchBookByPriceRange();
void searchBookByPriceRangeSecond();
void searchBookByPriceRangeThird();
void searchBookByStock();
void searchBookByStockRangeFirst();
void searchBookByStockRangeSecond();
int isValidDate(const char *date);
int convertDateToInt(const char *date);



int main(int argc, char const *argv[]){
    FILE *userdata;
    int loginSuccess = 0;
    char loggedInUsername[50];

    system("cls");
    userdata = fopen("userdata.txt", "r");
    if (userdata == NULL) {
        printf("Gagal membuka file userdata.\n");
        return 1;
    }

   
    loginSuccess = login(userdata, loggedInUsername);

    
    fclose(userdata);

    
    if (loginSuccess) {
    int choice;
    int schoice;

    do
    {
        system("cls");
        printf("\n==============================================\n");
        printf("=                 Book Sales                 =\n");
        printf("==============================================\n");
        printf("Welcome aboard %s!", loggedInUsername);
        printf("\n1. Entry data buku\n");
        printf("2. Tampilkan buku\n");
        printf("3. Entry data sales\n");
        printf("4. Tampilkan data sales\n");
        printf("5. Sort data buku atau sales\n");
        printf("6. Search data\n");
       
        printf("7. Exit\n");
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
            sort();    
                break;
        case 6:
            search();    
                break;
        default:
            printf("invalid");
            break;
        }
        

    } while (choice != 7);
    exitAnimation();
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

     printf("\n -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF)
    {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", book.name, book.genre, book.price, book.stock);
    }
    printf(" -----------------------------------------------------\n");

    fclose(file);
    printf("Press Any to continue...");
    pause();

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

    int found = 0;
    int bookCount = 0;
    int validInput = 0;

    while (!validInput) {
        printf("\nMasukkan Tanggal Transaksi (dd/mm/yyyy)\t: ");
        scanf("%s", sale.tanggal);

        if (!isValidDate(sale.tanggal)) {
            printf("Format tanggal tidak valid. Masukkan tanggal dengan format dd/mm/yyyy.\n");
            continue;
        }

        printf("Masukkan Nama Pelanggan\t\t\t: ");
        scanf(" %[^\n]s", sale.customerName);

        printf("Masukkan Nama Buku\t\t\t: ");
        scanf(" %[^\n]s", sale.orderedBook);

        printf("Masukkan Jumlah Buku\t\t\t: ");
        scanf("%d", &sale.orderqty);

        rewind(file); // Reset file pointer

        while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", books.name, books.genre, &books.price, &books.stock) != EOF) {
            if (strcmp(books.name, sale.orderedBook) == 0) {
                found = 1;
                strcpy(sale.genre, books.genre);
                sale.price = books.price;
                sale.stock = books.stock;

                // Periksa apakah stok cukup
                if (sale.orderqty > books.stock) {
                    printf("Stok tidak mencukupi. Silakan masukkan kembali data.\n");
                    found = 0;
                    break; // Jika stok tidak mencukupi, kembali ke langkah awal (input tanggal)
                } else {
                    validInput = 1; // Jika stok cukup, keluar dari loop
                    sale.pay = sale.orderqty * books.price;
                    books.stock -= sale.orderqty; // Mengurangi stok buku
                    sale.stock = books.stock;
                }
            }
            updatedBooks[bookCount++] = books;
        }
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

    // Menulis data transaksi ke dalam file
    fprintf(file2, "%s|%s|%s|%s|%.2f|%d|%d|%.2f\n", sale.tanggal, sale.customerName, sale.orderedBook, sale.genre, sale.price, sale.stock, sale.orderqty, sale.pay);

    printf("Data berhasil dimasukkan!\n");

    fclose(file);
    fclose(file2);

     printf("Press Any to continue...");
    pause();
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

    printf("\n -------------------------------------------------------------------------------------------------------------------\n");
    printf("| Tanggal Transaksi | Nama Pelanggan| Buku Dipesan      | Genre Buku | Harga Buku  | Stok | Banyak | Total Harga   |\n");
    printf(" -------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fptr, " %[^|]|%[^|]|%[^|]|%[^|]|%f|%d|%d|%f\n", sale.tanggal, sale.customerName, sale.orderedBook, sale.genre, &sale.price, &sale.stock, &sale.orderqty, &sale.pay) != EOF) {
        printf("| %-17s | %-13s | %-17s | %-10s | %-11.2f | %-4d | %-6d | %-13.2f |\n", sale.tanggal, sale.customerName, sale.orderedBook, sale.genre, sale.price, sale.stock, sale.orderqty, sale.pay);
    }

    printf(" -------------------------------------------------------------------------------------------------------------------\n");

    fclose(fptr);

    printf("Press Any to continue...");
    pause();
}

//fungsi untuk sort harga buku

void sortPriceBooks() {
    struct bookList bookArr[100];

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

   
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].price > bookArr[j + 1].price) {
              
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf("\n -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
    printf("Press Any to continue...");
    pause();
}

//fungsi untuk sort data buku desc

void sortPriceBooksDescending() {
    struct bookList bookArr[100]; 

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

   
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].price < bookArr[j + 1].price) {
               
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf("\n -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
    printf("Press Any to continue...");
    pause();
}


// sort untuk stok buku

void sortStockBooks() {
    struct bookList bookArr[100];

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

   
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].stock > bookArr[j + 1].stock) {
               
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf("\n -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
    printf("Press Any to continue...");
    pause();
}

// sort untuk stok buku desc

void sortStockBooksDescending() {
    struct bookList bookArr[100]; 

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

    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArr[j].stock < bookArr[j + 1].stock) {
                
                struct bookList temp = bookArr[j];
                bookArr[j] = bookArr[j + 1];
                bookArr[j + 1] = temp;
            }
        }
    }

    printf("\n -----------------------------------------------------\n");
    printf("| Judul               | Genre     | Harga       | Stok |\n");
    printf(" -----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("|%-21s|%-11s|%-13.2f|%-6d|\n", bookArr[i].name, bookArr[i].genre, bookArr[i].price, bookArr[i].stock);
    }
    printf(" -----------------------------------------------------\n");
    printf("Press Any to continue...");
    pause();
    
}

//fungsi untuk entry data buku

void entryBookData() {
    FILE *file = fopen("book.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    struct bookList newBook;

    printf("\nMasukkan Judul Buku\t: ");
    scanf(" %[^\n]s", newBook.name);

    printf("Masukkan Genre Buku\t: ");
    scanf(" %[^\n]s", newBook.genre);

    printf("Masukkan Harga Buku\t: ");
    scanf("%f", &newBook.price);

    printf("Masukkan Stok Buku\t: ");
    scanf("%d", &newBook.stock);

    fprintf(file, "%s|%s|%.2f|%d\n", newBook.name, newBook.genre, newBook.price, newBook.stock);
    printf("Data buku berhasil dimasukkan!\n");

    fclose(file);
}



//sort untuk sales berdasarkan tanggal desc

void sortSalesByDateDescending() {
    struct sales salesArr[100];

    FILE *file = fopen("sales.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%f|%d|%d|%f\n", 
                  salesArr[count].tanggal, salesArr[count].customerName, 
                  salesArr[count].orderedBook, salesArr[count].genre, 
                  &salesArr[count].price, &salesArr[count].stock, 
                  &salesArr[count].orderqty, &salesArr[count].pay) != EOF) {
        count++;
    }

    fclose(file);

    // Menggunakan bubble sort untuk mengurutkan data berdasarkan tanggal transaksi secara descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int date1 = convertDateToInt(salesArr[j].tanggal);
            int date2 = convertDateToInt(salesArr[j + 1].tanggal);
            
            if (date1 < date2) {
                struct sales temp = salesArr[j];
                salesArr[j] = salesArr[j + 1];
                salesArr[j + 1] = temp;
            }
        }
    }

    // Menampilkan data yang sudah diurutkan secara descending berdasarkan tanggal
    printf("\n --------------------------------------------------------------------------------------------------------\n");
    printf("|Tanggal Transaksi|Nama Pelanggan    |Buku Dipesan     |Genre Buku|Harga Buku |Stok|Banyak|Total Harga  |\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("|%-17s|%-18s|%-17s|%-10s|%-11.2f|%-4d|%-6d|%-13.2f|\n", salesArr[i].tanggal, salesArr[i].customerName, salesArr[i].orderedBook, salesArr[i].genre, salesArr[i].price, salesArr[i].stock, salesArr[i].orderqty, salesArr[i].pay);
    }

    printf(" --------------------------------------------------------------------------------------------------------\n");
    printf("Press Any to continue...");
    // Fungsi pause() harus ditambahkan atau diganti dengan fungsi yang sesuai
     pause();
}

// sort untuk sales berdasarkan tanggal asc

void sortSalesByDateAscending() {
    struct sales salesArr[100];

    FILE *file = fopen("sales.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    int count = 0;
    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%f|%d|%d|%f\n", 
                  salesArr[count].tanggal, salesArr[count].customerName, 
                  salesArr[count].orderedBook, salesArr[count].genre, 
                  &salesArr[count].price, &salesArr[count].stock, 
                  &salesArr[count].orderqty, &salesArr[count].pay) != EOF) {
        count++;
    }

    fclose(file);

    // Menggunakan bubble sort untuk mengurutkan data berdasarkan tanggal transaksi
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int date1 = convertDateToInt(salesArr[j].tanggal);
            int date2 = convertDateToInt(salesArr[j + 1].tanggal);
            
            if (date1 > date2) {
                struct sales temp = salesArr[j];
                salesArr[j] = salesArr[j + 1];
                salesArr[j + 1] = temp;
            }
        }
    }

    // Menampilkan data yang sudah diurutkan berdasarkan tanggal
    printf("\n --------------------------------------------------------------------------------------------------------\n");
    printf("|Tanggal Transaksi|Nama Pelanggan    |Buku Dipesan     |Genre Buku|Harga Buku |Stok|Banyak|Total Harga  |\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("|%-17s|%-18s|%-17s|%-10s|%-11.2f|%-4d|%-6d|%-13.2f|\n", salesArr[i].tanggal, salesArr[i].customerName, salesArr[i].orderedBook, salesArr[i].genre, salesArr[i].price, salesArr[i].stock, salesArr[i].orderqty, salesArr[i].pay);
    }

    printf(" --------------------------------------------------------------------------------------------------------\n");
    printf("Press Any to continue...");
    // Fungsi pause() harus ditambahkan atau diganti dengan fungsi yang sesuai
     pause();
}


//fungsi untuk search data berdasarkan nama buku

void searchBookByName() {
    char bookName[50];
    printf("\nMasukkan nama buku yang ingin dicari: ");
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
    printf("Press Any to continue...");
    pause();
}

//fungsi untuk search data berdasarkan genre buku

void searchBookByGenre() {
    char bookGenre[50];
    printf("\nMasukkan genre buku yang ingin dicari: ");
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
    printf("Press Any to continue...");
    pause();
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
    printf("Press Any to continue...");
    pause();
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

    printf("\n -----------------------------------------------------\n");
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
    printf("Press Any to continue...");
    pause();
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

    printf("\n -----------------------------------------------------\n");
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
    printf("Press Any to continue...");
    pause();
}

void searchBookByPrice() {
    int choice;

    printf("\n1. 1,000-10,000\n");
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

    printf("\n -----------------------------------------------------\n");
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
    printf("Press Any to continue...");
    pause();
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

    printf("\n -----------------------------------------------------\n");
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
    printf("Press Any to continue...");
    pause();

}

void searchBookByStock() {
    int choice;

    printf("\n1. 1-50\n");
    printf("2. 50-100\n");
    
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

//fungsi login

int login(FILE *userdata, char *loggedInUsername) {
    struct UserData user;
    char inputUsername[50];
    char inputPassword[50];
    int found = 0;

    printf("Loading");

  
    for (int i = 0; i < 10; ++i) {
        printf(".");
        fflush(stdout);
     
        for (int i = 0; i < 20000 / 10; ++i) {
       
        for (int j = 0; j < 100000; ++j) {
           
        }
    }
    }

    system("cls");

    
    printf("\nMasukkan username: ");
    scanf(" %[^\n]s", inputUsername);
    printf("Masukkan password: ");
    scanf(" %[^\n]s", inputPassword);

    
    while (fscanf(userdata, "%49[^|]|%49[^\n]\n", user.username, user.password) == 2) {
        if (strcmp(user.username, inputUsername) == 0 && strcmp(user.password, inputPassword) == 0) {
            found = 1;
           
            strcpy(loggedInUsername, user.username);
            break;
        }
    }

   
    if (found) {
        printf("Login berhasil!\n");
        system("cls");
        return 1;
    } else {
        printf("Login gagal. Username atau password salah.\n");
        return 0; 
    }
}


void sort(){
    int choice;

    printf("\n1. Sort buku berdasarkan harga\n");
    printf("2. Sort buku berdasarkan stok\n");
    printf("3. Sort sales berdasarkan tanggal\n");
    printf("Pilih : ");
    scanf("%d", &choice);
    getchar();

    switch (choice)
    {
    case 1:
        printf("\n1. ascending\n");
        printf("2. descending\n");
        printf("Pilih : ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            sortPriceBooks();
            break;
        case 2:
            sortPriceBooksDescending();
            break;
        
        default:
            printf("invalid");
            break;
        }
        break;
    case 2:
        printf("\n1. ascending\n");
        printf("2. descending\n");
        printf("Pilih : ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            sortStockBooks();
            break;
        case 2:
            sortStockBooksDescending();
            break;
        
        default:
        printf("invalid");
            break;
        }
        break;
    case 3:
        printf("\n1. ascending\n");
        printf("2. descending\n");
        printf("Pilih : ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            sortSalesByDateAscending();
            break;
        case 2:
            sortSalesByDateDescending();
            break;
        
        default:
        printf("invalid");
            break;
        }
        break;

    
    default:
    printf("invalid");
        break;
    }
}

void search(){

    int choice;

    printf("1. search data berdasarkan nama buku\n");
    printf("2. search data berdasarkan genre buku\n");
    printf("3. mencari data berdasarkan kisaran harga buku\n");
    printf("4. mencari data berdasarkan kisaran stok buku\n");
    printf("Pilih : ");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        searchBookByName();
        break;
    case 2:
        searchBookByGenre();
        break;
    case 3:
        searchBookByPrice();
        break;
    case 4:
        searchBookByStock();    
    
    default:
    printf("invalid");
        break;
    }
}

void exitAnimation() {
    system("cls");  // Clear the console screen

    // Display exit animation
    for (int i = 10; i >= 0; i--) {
        printf("\n\n\t\tExiting in %d seconds...", i);
        Sleep(1000);  // Sleep for 1 second (1000 milliseconds)
        system("cls");  // Clear the console screen for the next iteration
    }

    // You can add additional farewell messages or cleanup code here
    printf("\n\n\t\tGoodbye! Thank you for using the program.\n");
}

void pause() {
    while (!kbhit()) {  // Loops until a key is pressed
        // Optionally, perform other tasks within the loop
    }
}

int isValidDate(const char *date) {
    // Check if the date string has the correct length
    if (strlen(date) != 10)
        return 0;

    // Check the format: "dd/mm/yyyy"
    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5) && date[i] != '/')
            return 0;
        else if (i != 2 && i != 5 && !isdigit(date[i]))
            return 0;
    }

    // Validate the day, month, and year values
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year);

    if (year < 1000 || year > 9999)
        return 0;
    if (month < 1 || month > 12)
        return 0;

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        daysInMonth[2] = 29;

    if (day < 1 || day > daysInMonth[month])
        return 0;

    return 1;
}

int convertDateToInt(const char *date) {
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year);
    return year * 10000 + month * 100 + day;
}
