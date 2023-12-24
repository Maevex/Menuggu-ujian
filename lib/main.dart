import 'dart:io';

// Struktur data untuk menyimpan informasi penjualan buku
class BookSale {
  late String transactionDate;
  late String customerName;
  late String bookType;
  late String bookName;
  late int quantity;
  late double price;
  late double totalPayment;
}

// Struktur untuk data user
class UserData {
  late String username;
  late String password;
}

void entryData(File file) {
  var sale = BookSale();

  stdout.write("Masukkan Tanggal Transaksi (dd/mm/yyyy): ");
  sale.transactionDate = stdin.readLineSync()!;

  stdout.write("Masukkan Nama Pelanggan: ");
  sale.customerName = stdin.readLineSync()!;

  stdout.write("Masukkan Jenis Buku: ");
  sale.bookType = stdin.readLineSync()!;

  stdout.write("Masukkan Nama Buku: ");
  sale.bookName = stdin.readLineSync()!;

  stdout.write("Masukkan Jumlah Buku: ");
  sale.quantity = int.parse(stdin.readLineSync()!);

  stdout.write("Masukkan Harga Buku: ");
  sale.price = double.parse(stdin.readLineSync()!);

  // Menghitung total pembayaran
  sale.totalPayment = sale.quantity * sale.price;

  // Menulis data ke dalam file
  file.writeAsStringSync("${sale.transactionDate}|${sale.customerName}|${sale.bookType}|${sale.bookName}|${sale.quantity}|${sale.price}|${sale.totalPayment}\n");

  print("Data berhasil dimasukkan!");
}

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan harga (ascending)
int compareByPriceAscending(dynamic a, dynamic b) =>
    (a as BookSale).price.compareTo((b as BookSale).price);

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan harga (descending)
int compareByPriceDescending(dynamic a, dynamic b) =>
    (b as BookSale).price.compareTo((a as BookSale).price);

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan stok (quantity)
int compareByQuantity(dynamic a, dynamic b) =>
    (b as BookSale).quantity.compareTo((a as BookSale).quantity);

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan harga
int compareByPrice(dynamic a, dynamic b) =>
    (b as BookSale).price.compareTo((a as BookSale).price);

// Fungsi untuk membandingkan dua data penjualan buku berdasarkan tanggal transaksi
int compareByDate(dynamic a, dynamic b) =>
    (a as BookSale).transactionDate.compareTo((b as BookSale).transactionDate);

// Fungsi untuk mencari data berdasarkan nama buku
void searchDataByName(File file, String bookName) {
  var sale = BookSale();
  var found = false;

  // Membaca file dan mencari data
  final lines = file.readAsLinesSync();
  for (var line in lines) {
    var data = line.split('|');
    sale = BookSale()
      ..transactionDate = data[0]
      ..customerName = data[1]
      ..bookType = data[2]
      ..bookName = data[3]
      ..quantity = int.parse(data[4])
      ..price = double.parse(data[5])
      ..totalPayment = double.parse(data[6]);

    if (sale.bookName == bookName) {
      found = true;
      print("Tanggal Transaksi: ${sale.transactionDate}");
      print("Nama Pelanggan: ${sale.customerName}");
      print("Jenis Buku: ${sale.bookType}");
      print("Nama Buku: ${sale.bookName}");
      print("Jumlah Buku: ${sale.quantity}");
      print("Harga Buku: ${sale.price.toStringAsFixed(2)}");
      print("Total Pembayaran: ${sale.totalPayment.toStringAsFixed(2)}\n");
    }
  }

  if (!found) {
    print("Data tidak ditemukan.");
  }
}

// Fungsi search berdasarkan harga
void searchDataByPrice(File file, double price) {
  var sale = BookSale();
  var found = false;

  // Membaca file dan mencari data
  final lines = file.readAsLinesSync();
  for (var line in lines) {
    var data = line.split('|');
    sale = BookSale()
      ..transactionDate = data[0]
      ..customerName = data[1]
      ..bookType = data[2]
      ..bookName = data[3]
      ..quantity = int.parse(data[4])
      ..price = double.parse(data[5])
      ..totalPayment = double.parse(data[6]);

    if (sale.price == price) {
      found = true;
      print("Tanggal Transaksi: ${sale.transactionDate}");
      print("Nama Pelanggan: ${sale.customerName}");
      print("Jenis Buku: ${sale.bookType}");
      print("Nama Buku: ${sale.bookName}");
      print("Jumlah Buku: ${sale.quantity}");
      print("Harga Buku: ${sale.price.toStringAsFixed(2)}");
      print("Total Pembayaran: ${sale.totalPayment.toStringAsFixed(2)}\n");
    }
  }

  if (!found) {
    print("Data tidak ditemukan.");
  }
}

// Fungsi untuk login
int login(File userdata) {
  var user = UserData();
  var inputUsername = '';
  var inputPassword = '';
  var found = false;

  stdout.write("Masukkan username: ");
  inputUsername = stdin.readLineSync()!;
  stdout.write("Masukkan password: ");
  inputPassword = stdin.readLineSync()!;

  // Membaca file userdata.txt dan memeriksa kecocokan informasi login
  final lines = userdata.readAsLinesSync();
  for (var line in lines) {
    var data = line.split('|');
    user = UserData()
      ..username = data[0]
      ..password = data[1];

    if (user.username == inputUsername && user.password == inputPassword) {
      found = true;
      break;
    }
  }

  if (found) {
    print("Login berhasil!");
    return 1;
  } else {
    print("Login gagal. Username atau password salah.");
    return 0;
  }
}

void main() {
  var file = File("sales.txt");
  var userdata = File("userdata.txt");
  var choice = 0;

  // Membuka file atau membuat file baru jika belum ada
  if (!file.existsSync()) {
    file.createSync();
  }

  if (!userdata.existsSync()) {
    print("Gagal membuka file userdata.txt.");
    return;
  }

  var loginSuccess = 0;
  do {
    loginSuccess = login(userdata);
  } while (loginSuccess != 1);

  var bookName = '';
  var searchPrice = 0.0;

  do {
    // Menampilkan menu
    print("Menu:");
    print("1. Entry Data");
    print("2. Sort Data");
    print("3. Search Data");
    print("4. Keluar");
    stdout.write("Pilih: ");
    choice = int.parse(stdin.readLineSync()!);

    switch (choice) {
      case 1:
        entryData(file);
        break;
      case 2:
        // Menampilkan menu untuk pengurutan
        print("Pilih kriteria pengurutan:");
        print("1. Stok (Quantity)");
        print("2. Harga (Ascending)");
        print("3. Harga (Descending)");
        print("4. Tanggal Transaksi");
        stdout.write("Pilih: ");
        choice = int.parse(stdin.readLineSync()!);

        // Membaca seluruh data dari file ke dalam list
        final lines = file.readAsLinesSync();
        var sales = <BookSale>[];
        for (var line in lines) {
          var data = line.split('|');
          var sale = BookSale()
            ..transactionDate = data[0]
            ..customerName = data[1]
            ..bookType = data[2]
            ..bookName = data[3]
            ..quantity = int.parse(data[4])
            ..price = double.parse(data[5])
            ..totalPayment = double.parse(data[6]);
          sales.add(sale);
        }

        // Melakukan pengurutan
        switch (choice) {
          case 1:
            sales.sort(compareByQuantity);
            break;
          case 2:
            sales.sort(compareByPriceAscending);
            break;
          case 3:
            sales.sort(compareByPriceDescending);
            break;
          case 4:
            sales.sort(compareByDate);
            break;
          default:
            print("Pilihan tidak valid.");
            continue;
        }

        // Menampilkan hasil pengurutan dalam bentuk tabel
        print("--------------------------------------------------------------------------------------------------------------------------------------------------");
        print("| Tanggal Transaksi | Nama Pelanggan    | Jenis Buku        | Nama Buku                             | Jumlah Buku | Harga Buku | Total Pembayaran |");
        print("----------------------------------------------------------------------------------------------------------------------------");
        for (var sale in sales) {
          print("| ${sale.transactionDate} | ${sale.customerName} | ${sale.bookType} | ${sale.bookName} | ${sale.quantity} | ${sale.price} | ${sale.totalPayment} |");
          print("--------------------------------------------------------------------------------------------------------------------------------------------------");
        }
        break;
      case 3:
        // Menampilkan menu untuk pencarian
        print("Pilih kriteria pencarian:");
        print("1. Nama Buku");
        print("2. Harga Buku");
        stdout.write("Pilih: ");
        choice = int.parse(stdin.readLineSync()!);

        // Melakukan pencarian
        switch (choice) {
          case 1:
            // Mencari berdasarkan nama buku
            stdout.write("Masukkan Nama Buku yang dicari: ");
            bookName = stdin.readLineSync()!;
            searchDataByName(file, bookName);
            break;

          case 2:
            // Mencari berdasarkan nama buku
            stdout.write("Masukkan Harga Buku yang dicari: ");
            searchPrice = double.parse(stdin.readLineSync()!);
            searchDataByPrice(file, searchPrice);
            break;

          default:
            print("Pilihan tidak valid.");
            continue;
        }
        break;

      case 4:
        print("Keluar dari program.");
        break;

      default:
        print("Pilihan tidak valid.");
        break;
    }
  } while (choice != 4);

  // Menutup file
  file.close();
}
