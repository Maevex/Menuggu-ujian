#include <stdio.h>
#include <string.h>

struct Sales {
    char tanggal[20];
    char customerName[50];
    char orderedBook[50];
    int orderqty;
    float pay;
};

struct BookList {
    char name[50];
    char genre[50];
    float price;
    int stock;
};

// Deklarasi fungsi-fungsi
void displayBooks();
void entryData();
void displaySales();
void sortPriceBooks();
void sortPriceBooksDescending();
void sortStockBooks();
void sortStockBooksDescending();

int main(int argc, char const *argv[]) {
    int choice;

    do {
        printf("Menu:\n");
        printf("1. Tampilkan buku\n");
        printf("2. Entry data\n");
        printf("3. Tampilkan data sales\n");
        printf("4. Sort buku berdasarkan harga Termurah\n");
        printf("5. Sort buku berdasarkan harga Termahal\n");
        printf("6. Sort buku berdasarkan stok paling sedikit\n");
        printf("7. Sort buku berdasarkan stok paling banyak\n");
        printf("8. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &choice);

        switch (choice) {
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
            case 8:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
    } while (choice != 8);

    return 0;
}

void displayBooks() {
    FILE *file;
    struct BookList book;

    // Buka file "book.txt" dalam mode baca
    file = fopen("book.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    // Menampilkan header tabel
    printf("Judul\tGenre\tHarga\t\tStok\n");

    // Membaca data dari file dan menampilkannya
    while (fscanf(file, "%[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF) {
        printf("%s\t%s\t%.2f\t%d\n", book.name, book.genre, book.price, book.stock);
    }

    // Menutup file setelah selesai membaca dan menampilkan data
    fclose(file);
}