#include <stdio.h>
#include <string.h>

struct sales
{
    char tanggal[20];
    char customerName[50];
    char orderedBook[50];
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

void displayBooks();
void entryData();
void displaySales();
void sortPriceBooks();
void sortPriceBooksDescending();
void sortStockBooks();
void sortStockBooksDescending();

int main(int argc, char const *argv[])
{
    int choice;
    int schoice;

    do
    {
        printf("Menu:\n");
        printf("1. Tampilkan buku\n");
        printf("2. Entry data\n");
        printf("3. Tampilkan data sales\n");
        printf("4. Sort buku berdasarkan harga Termurah\n");
        printf("5. Sort buku berdasarkan harga Termahal\n");
        printf("6. Sort buku berdasarkan stok paling sedikit\n");
        printf("6. Sort buku berdasarkan stok paling banyak\n");
        printf("Pilih: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayBooks();
            break;
        case 2:
            entryData();
            break;
        case 3:
            displaySales();
            break;
        case 4:
            sortPriceBooks();    
                break;
        case 5:
            sortPriceBooksDescending();    
                break;
        case 6:
                sortStockBooks();
                break;
        case 7:
             sortStockBooksDescending();
                break;
        default:
            printf("Pilihan tidak valid.\n");
            break;
        }
    } while (choice != 5);

    return 0;
}

void displayBooks() {
    FILE *file;
    struct bookList book;

    file = fopen("book.txt", "r");
    if (file == NULL)
    {
        printf("Gagal membuka file.\n");
        return;
    }

    printf("Judul\tGenre\tHarga\t\tStok\n");
    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF)
    {
        printf("%s\t%s\t%.2f\t%d\n", book.name, book.genre, book.price, book.stock);
    }

    fclose(file);
}

void entryData() {
    FILE *file;
    FILE *file2;
    struct sales sale;
    struct bookList books;
    struct bookList updatedBooks[100]; // Menggunakan batasan maksimum data buku

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

    // Menghitung total pembayaran
    sale.pay = sale.orderqty * books.price;

    // Menulis data penjualan ke dalam file
    fprintf(file2, "%s|%s|%s|%d|%.2f\n", sale.tanggal, sale.customerName, sale.orderedBook, sale.orderqty, sale.pay);

    printf("Data berhasil dimasukkan!\n");

    fclose(file);
    fclose(file2);
}


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

