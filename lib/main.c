#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur data untuk menyimpan informasi penjualan buku
struct BookSale {
    char transactionDate[20];
    char customerName[50];
    char bookType[20];
    char bookName[50];
    int quantity;
    float price;
    float totalPayment;
};

//struktur untuk data user
struct UserData {
    char username[50];
    char password[50];
};


void entryData(FILE *file);

int compareByPriceAscending(const void *a, const void *b);

int compareByPriceDescending(const void *a, const void *b);

int compareByQuantity(const void *a, const void *b);

int compareByPrice(const void *a, const void *b);

int compareByDate(const void *a, const void *b);

int login(FILE *userdata);

void searchDataByName(FILE *file, const char *bookName);

int main() {
    FILE *file;
    int choice;
    FILE *userdata;

    // Membuka file atau membuat file baru jika belum ada
    file = fopen("sales.txt", "a+");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return 1;
    }
     userdata = fopen("userdata.txt", "r");
    if (userdata == NULL) {
        printf("Gagal membuka file userdata.txt.\n");
        return 1;
    }

    int loginSuccess = 0;
    do {
        loginSuccess = login(userdata);
    } while (!loginSuccess);

    char bookName[50];
    
    do {
        // Menampilkan menu
        printf("Menu:\n");
        printf("1. Entry Data\n");
        printf("2. Sort Data\n");
        printf("3. Search Data\n");
        printf("4. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                entryData(file);
                break;
            case 2:
                // Menampilkan menu untuk pengurutan
                printf("Pilih kriteria pengurutan:\n");
                printf("1. Stok (Quantity)\n");
                printf("2. Harga (Ascending)\n");
                printf("3. Harga (Descending)\n");
                printf("4. Tanggal Transaksi\n");
                printf("Pilih: ");
                scanf("%d", &choice);

                // Membaca seluruh data dari file ke dalam array
                fseek(file, 0, SEEK_SET);
                struct BookSale sales[100];
                int count = 0;
                while (fscanf(file, "%19[^|]|%49[^|]|%19[^|]|%49[^|]|%d|%f|%f\n",
                              sales[count].transactionDate, sales[count].customerName,
                              sales[count].bookType, sales[count].bookName, &sales[count].quantity,
                              &sales[count].price, &sales[count].totalPayment) == 7) {
                    count++;
                }

                // Melakukan pengurutan
                switch (choice) {
                    case 1:
                        qsort(sales, count, sizeof(struct BookSale), compareByQuantity);
                        break;
                    case 2:
                        qsort(sales, count, sizeof(struct BookSale), compareByPriceAscending);
                        break;
                    case 3:
                        qsort(sales, count, sizeof(struct BookSale), compareByPriceDescending);
                        break;
                    case 4:
                        qsort(sales, count, sizeof(struct BookSale), compareByDate);
                        break;
                    default:
                        printf("Pilihan tidak valid.\n");
                        continue;
                }

                // Menampilkan hasil pengurutan dalam bentuk tabel
                printf("--------------------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("| Tanggal Transaksi | Nama Pelanggan    | Jenis Buku        | Nama Buku                             | Jumlah Buku | Harga Buku | Total Pembayaran |\n");
                printf("----------------------------------------------------------------------------------------------------------------------------\n");
                for (int i = 0; i < count; i++) {
                    printf("| %-17s | %-17s | %-17s | %-37s | %-11d | %-10.2f | %-16.2f |\n",
                           sales[i].transactionDate, sales[i].customerName, sales[i].bookType,
                           sales[i].bookName, sales[i].quantity, sales[i].price, sales[i].totalPayment);
                                    printf("--------------------------------------------------------------------------------------------------------------------------------------------------\n");
                }
                break;
            case 3:
                // Menampilkan menu untuk pencarian
                printf("Pilih kriteria pencarian:\n");
                printf("1. Nama Buku\n");
                printf("Pilih: ");
                scanf("%d", &choice);

                // Melakukan pencarian
                switch (choice) {
                    case 1:
                        // Mencari berdasarkan nama buku
                        
                        printf("Masukkan Nama Buku yang dicari: ");
                        scanf(" %[^\n]s", bookName);
                        fseek(file, 0, SEEK_SET);
                        searchDataByName(file, bookName);
                        break;
                    default:
                        printf("Pilihan tidak valid.\n");
                        continue;
                }
                break;
            case 4:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
    } while (choice != 4);

    // Menutup file
    fclose(file);

    return 0;
}

// Fungsi untuk memasukkan data penjualan buku ke dalam file
void entryData(FILE *file) {
    struct BookSale sale;

    printf("Masukkan Tanggal Transaksi (dd/mm/yyyy): ");
    scanf("%s", sale.transactionDate);

    printf("Masukkan Nama Pelanggan: ");
    scanf(" %[^\n]s", sale.customerName);

    printf("Masukkan Jenis Buku: ");
    scanf(" %[^\n]s", sale.bookType);

    printf("Masukkan Nama Buku: ");
    scanf(" %[^\n]s", sale.bookName);

    printf("Masukkan Jumlah Buku: ");
    scanf("%d", &sale.quantity);

    printf("Masukkan Harga Buku: ");
    scanf("%f", &sale.price);

    // Menghitung total pembayaran
    sale.totalPayment = sale.quantity * sale.price;

    // Menulis data ke dalam file
    fprintf(file, "%s|%s|%s|%s|%d|%.2f|%.2f\n", sale.transactionDate, sale.customerName,
            sale.bookType, sale.bookName, sale.quantity, sale.price, sale.totalPayment);

    printf("Data berhasil dimasukkan!\n");
}

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan harga (ascending)
int compareByPriceAscending(const void *a, const void *b) {
    return ((struct BookSale *)a)->price - ((struct BookSale *)b)->price;
}

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan harga (descending)
int compareByPriceDescending(const void *a, const void *b) {
    return ((struct BookSale *)b)->price - ((struct BookSale *)a)->price;
}

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan stok (quantity)
int compareByQuantity(const void *a, const void *b) {
    return ((struct BookSale *)b)->quantity - ((struct BookSale *)a)->quantity;
}

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan harga
int compareByPrice(const void *a, const void *b) {
    return ((struct BookSale *)b)->price - ((struct BookSale *)a)->price;
}

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan tanggal transaksi
int compareByDate(const void *a, const void *b) {
    return strcmp(((struct BookSale *)a)->transactionDate, ((struct BookSale *)b)->transactionDate);
}

// Fungsi untuk mencari data berdasarkan nama buku
void searchDataByName(FILE *file, const char *bookName) {
    struct BookSale sale;
    int found = 0;

    // Membaca file dan mencari data
    while (fscanf(file, "%19[^|]|%49[^|]|%19[^|]|%49[^|]|%d|%f|%f\n", sale.transactionDate,
                  sale.customerName, sale.bookType, sale.bookName, &sale.quantity, &sale.price,
                  &sale.totalPayment) == 7) {
        if (strcmp(sale.bookName, bookName) == 0) {
            found = 1;
            printf("Tanggal Transaksi: %s\n", sale.transactionDate);
            printf("Nama Pelanggan: %s\n", sale.customerName);
            printf("Jenis Buku: %s\n", sale.bookType);
            printf("Nama Buku: %s\n", sale.bookName);
            printf("Jumlah Buku: %d\n", sale.quantity);
            printf("Harga Buku: %.2f\n", sale.price);
            printf("Total Pembayaran: %.2f\n", sale.totalPayment);
            printf("\n");
        }
    }

    if (!found) {
        printf("Data tidak ditemukan.\n");
    }
}


//fungsi untuk login
int login(FILE *userdata) {
    struct UserData user;
    char inputUsername[50];
    char inputPassword[50];
    int found = 0;

    printf("Masukkan username: ");
    scanf(" %[^\n]s", inputUsername);
    printf("Masukkan password: ");
    scanf(" %[^\n]s", inputPassword);

    // Membaca file userdata.txt dan memeriksa kecocokan informasi login
    while (fscanf(userdata, "%49[^|]|%49[^\n]\n", user.username, user.password) == 2) {
        if (strcmp(user.username, inputUsername) == 0 && strcmp(user.password, inputPassword) == 0) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Login berhasil!\n");
        return 1; 
    } else {
        printf("Login gagal. Username atau password salah.\n");
        return 0; 
    }
}

