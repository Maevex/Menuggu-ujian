#include <stdio.h>

struct sales
{
    char tanggal[20];
    char customerName[50];
    
};


struct bookList
{
    char name[50];
    char genre[50];
    float price;
    int stock;
};

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
    while (fscanf(file, "si %[^|]|%[^|]|%f|%d\n", book.name, book.genre, &book.price, &book.stock) != EOF)
    {
        printf("%s\t%s\t%.2f\t%d\n", book.name, book.genre, book.price, book.stock);
    }

    fclose(file);
}

int main(int argc, char const *argv[])
{
    int choice;

    do
    {
        printf("Menu:\n");
        printf("1. Tampilkan buku\n");
        printf("2. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayBooks();
            break;
        case 2:
            printf("Terima kasih!\n");
            break;
        default:
            printf("Pilihan tidak valid.\n");
            break;
        }
    } while (choice != 2);

    return 0;
}
