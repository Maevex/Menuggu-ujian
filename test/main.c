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
void sortPriceSale();

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
        printf("4. Sort Sales berdasarkan harga\n");
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
                sortPriceSale();
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

void sortPriceSale() {
    struct sales salesArr[100]; // Menggunakan batasan maksimum data penjualan

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

    // Bubble sort berdasarkan harga
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (salesArr[j].pay > salesArr[j + 1].pay) {
                // Melakukan swap langsung di dalam fungsi sort
                struct sales temp = salesArr[j];
                salesArr[j] = salesArr[j + 1];
                salesArr[j + 1] = temp;
            }
        }
    }

    // Menampilkan data yang sudah diurutkan berdasarkan harga
    printf(" -----------------------------------------------------------------------------------------------------\n");
    printf("|Tanggal Transaksi|Nama Pelanggan                |Buku yang dipesan         |Banyak Buku|Total Harga  |\n");
    printf(" -----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("|%-17s|%-30s|%-25s|%-12d|%-13.2f|\n", salesArr[i].tanggal, salesArr[i].customerName, salesArr[i].orderedBook, salesArr[i].orderqty, salesArr[i].pay);
    }

    printf(" -----------------------------------------------------------------------------------------------------\n");
}