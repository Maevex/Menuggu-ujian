#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SalesData {
    char transactionDate[20];
    char cashierName[50];
    char itemType[20];
    char itemName[50];
    int itemQuantity;
    float itemPrice;
    float totalPayment;
};

void addSalesData(struct SalesData data) {
    FILE *file = fopen("sales.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(file, "%s,%s,%s,%s,%d,%.2f,%.2f\n",
            data.transactionDate, data.cashierName,
            data.itemType, data.itemName,
            data.itemQuantity, data.itemPrice, data.totalPayment);

    fclose(file);
}

void displaySalesTable() {
    FILE *file = fopen("sales.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nSales Data Table\n");
    printf("| %-25s | %-20s | %-15s | %-20s | %-10s | %-10s | %-25s |\n",
           "Transaction Date", "Cashier Name", "Item Type", "Item Name",
           "Quantity", "Item Price", "Total Payment");

    printf("\n\n");
    // printf("|-------------------------|----------------------|---------------|----------------------|------------|------------|---------------|\n");

    struct SalesData data;
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%d,%f,%f\n",
                  data.transactionDate, data.cashierName,
                  data.itemType, data.itemName,
                  &data.itemQuantity, &data.itemPrice, &data.totalPayment) == 7) {
        printf("| %-25s | %-20s | %-15s | %-20s | %-10d | %-10.2f | %-25.2f |\n",
               data.transactionDate, data.cashierName,
               data.itemType, data.itemName,
               data.itemQuantity, data.itemPrice, data.totalPayment);
    }

    fclose(file);
}

void printSortedReport() {
    // Implementasi sorting dan pencetakan laporan
    // ...

    printf("Sorted report printed.\n");
}

void searchSalesData(char key[]) {
    // Implementasi pencarian data
    // ...

    printf("Search result:\n");
    // Cetak hasil pencarian
}

int main() {
    int choice;
    struct SalesData newSale;

    do {
        printf("\nCoffee Shop Sales Management System\n");
        printf("1. Entry Data Penjualan\n");
        printf("2. Tampil Laporan Penjualan\n");
        printf("3. Search Data Penjualan\n");
        printf("4. Exit\n");
        printf("Pilih menu: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nMasukkan data penjualan:\n");
                printf("Tanggal Transaksi (format: dd/mm/yyyy): ");
                scanf("%s", newSale.transactionDate);
                printf("Nama Kasir: ");
                scanf(" %[^\n]s", newSale.cashierName);
                printf("Jenis Minuman/Makanan: ");
                scanf(" %[^\n]s", newSale.itemType);
                printf("Nama Minuman/Makanan: ");
                scanf(" %[^\n]s", newSale.itemName);
                printf("Jumlah Minuman/Makanan: ");
                scanf("%d", &newSale.itemQuantity);
                printf("Harga Minuman/Makanan per item: ");
                scanf("%f", &newSale.itemPrice);
                newSale.totalPayment = newSale.itemQuantity * newSale.itemPrice;

                addSalesData(newSale);

                printf("\nData penjualan berhasil ditambahkan!\n");
                break;

            case 2:
                displaySalesTable();
                break;

            case 3:
                printf("\nMasukkan kata kunci untuk pencarian: ");
                char key[50];
                scanf(" %[^\n]s", key);
                searchSalesData(key);
                break;

            case 4:
                printf("\nTerima kasih!\n");
                break;

            default:
                printf("\nPilihan tidak valid. Silakan coba lagi.\n");
                break;
        }
    } while (choice != 4);

    return 0;
}
