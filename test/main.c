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


int main(int argc, char const *argv[])
{
    int choice;

    do
    {
        printf("Menu:\n");
        printf("1. Tampilkan buku\n");
        printf("2. Entry data\n");
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
        default:
            printf("Pilihan tidak valid.\n");
            break;
        }
    } while (choice != 2);

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

    // Menggunakan file untuk membaca data buku dan mencari buku yang sesuai
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", books.name, books.genre, &books.price, &books.stock) != EOF) {
        if (strcmp(books.name, sale.orderedBook) == 0) {
            found = 1;
            break;
        }
    }

    if (found) {
        // Menghitung total pembayaran
        sale.pay = sale.orderqty * books.price;

        // Menulis data penjualan ke dalam file
        fprintf(file2, "%s|%s|%s|%d|%.2f\n", sale.tanggal, sale.customerName, sale.orderedBook, sale.orderqty, sale.pay);

        printf("Data berhasil dimasukkan!\n");
    } else {
        printf("Buku tidak ditemukan dalam daftar.\n");
    }

    fclose(file);
    fclose(file2);
}