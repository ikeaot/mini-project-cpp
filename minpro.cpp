#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Product {
    int id;
    string name;
    int stock;
    int minStock;
    double price;
};

struct CartItem {
    string name;
    int qty;
    double price;
};

struct Member {
    int id;
    string name;
};

Product inventory[10] = {
    {101, "Keyboard Mechanical", 25, 10, 250000},
    {102, "Mouse Wireless", 12, 10, 120000},
    {103, "Headset Gaming", 8, 10, 350000},
    {104, "SSD 512GB NVMe", 4, 10, 800000}
};
int productCount = 4;

Member daftarMember[5] = {
    {1001, "Andi"},
    {1002, "Bayu"},
    {1003, "Chandra"},
    {2001, "Doni"},
    {2002, "Evan"}
};
int memberCount = 5;

Product* cariProduk(int id) {
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) return &inventory[i];
    }
    return nullptr;
}

Product* cariProduk(string nama) {
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].name == nama) return &inventory[i];
    }
    return nullptr;
}

Member* cariMember(int id) {
    for (int i = 0; i < memberCount; i++) {
        if (daftarMember[i].id == id) return &daftarMember[i];
    }
    return nullptr;
}

// baca angka dengan aman, kalau input bukan angka, clear buffer & return false
bool bacaAngka(int &value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        return false;
    }
    return true;
}

// garis panjang "=", tinggal panggil garis() daripada ngetik "====...." manual
void garis() {
    cout << string(58, '=') << endl;
}

// judul halaman, otomatis dikasih garis di atas & bawah
void judul(string teks) {
    garis();
    cout << teks << endl;
    garis();
}

void lihatInventory() {
    judul("KATALOG & STOK GUDANG");
    cout << left << setw(6) << "ID" << setw(24) << "Nama" << setw(8) << "Stok" << setw(8) << "Min" << "Harga" << endl;
    for (int i = 0; i < productCount; i++) {
        cout << left << setw(6) << inventory[i].id << setw(24) << inventory[i].name
             << setw(8) << inventory[i].stock << setw(8) << inventory[i].minStock
             << "Rp" << fixed << setprecision(0) << inventory[i].price << endl;
    }
}

void tambahProduk() {
    if (productCount >= 10) {
        cout << endl << "Inventory sudah penuh!" << endl;
        return;
    }
    Product &p = inventory[productCount];
    judul("TAMBAH PRODUK");
    cout << "ID Produk: "; cin >> p.id;
    cin.ignore();
    cout << "Nama Produk: "; getline(cin, p.name);
    cout << "Stok Awal: "; cin >> p.stock;
    cout << "Batas Minimum: "; cin >> p.minStock;
    cout << "Harga: "; cin >> p.price;
    productCount++;
    cout << endl << "Produk berhasil ditambahkan!" << endl;
}

void restock() {
    judul("RESTOCK BARANG");
    int id, jumlah;
    cout << "Masukkan ID produk: "; cin >> id;
    Product* p = cariProduk(id);
    if (p != nullptr) {
        cout << "Produk: " << p->name << " | Stok: " << p->stock << endl;
        cout << "Jumlah restock: "; cin >> jumlah;
        p->stock += jumlah;
        cout << endl << "Restock berhasil! Stok sekarang: " << p->stock << endl;
    } else {
        cout << endl << "Produk tidak ditemukan!" << endl;
    }
}

void checkLowStock() {
    judul("LOW STOCK ALERT");
    bool ada = false;
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].stock <= inventory[i].minStock) {
            cout << inventory[i].id << " | " << inventory[i].name
                 << " | Stok: " << inventory[i].stock << " | PERLU RESTOCK" << endl;
            ada = true;
        }
    }
    if (!ada) cout << "Tidak ada barang yang membutuhkan restock." << endl;
}

void adminDashboard() {
    int pilihan;
    do {
        judul("DASHBOARD ADMIN");
        cout << "[1] Lihat Katalog\n[2] Tambah Produk\n[3] Restock Barang\n[4] Cek Stok Rendah\n[0] Logout" << endl;
        cout << "Pilih Menu: ";
        if (!bacaAngka(pilihan)) {
            cout << endl << "Input tidak valid!" << endl;
            continue;
        }
        switch (pilihan) {
            case 1: lihatInventory(); break;
            case 2: tambahProduk(); break;
            case 3: restock(); break;
            case 4: checkLowStock(); break;
            case 0: cout << endl << "Logout berhasil." << endl; break;
            default: cout << endl << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 0);
}

// return 0 = transaksi berhasil, 1 = user minta logout, 2 = user batal & kembali ke menu
int transaksi(bool isMember, string nama = "") {
    CartItem cart[10];
    int cartCount = 0;
    double total = 0;
    char lanjut;

    judul("TRANSAKSI PENJUALAN");
    cout << (isMember ? "Mode: MEMBER (Diskon 5%)" : "Mode: GUEST") << endl;

    do {
        int pilihan;
        cout << endl << "Cari berdasarkan (1:ID, 2:Nama): "; cin >> pilihan;
        Product* p = nullptr;

        if (pilihan == 1) {
            int id;
            cout << "Masukkan ID: "; cin >> id;
            p = cariProduk(id);
        } else if (pilihan == 2) {
            string namaBarang;
            cin.ignore();
            cout << "Masukkan Nama: "; getline(cin, namaBarang);
            p = cariProduk(namaBarang);
        }

        if (p == nullptr) {
            cout << endl << "Produk tidak ditemukan!" << endl;
        } else {
            cout << endl << p->id << " | " << p->name << " | Stok: " << p->stock
                 << " | Rp" << fixed << setprecision(0) << p->price << endl;
            int qty;
            cout << "Jumlah Beli: "; cin >> qty;
            if (p->stock >= qty) {
                cart[cartCount] = {p->name, qty, p->price};
                cartCount++;
                p->stock -= qty;
                total += p->price * qty;
                cout << endl << "Ditambahkan ke keranjang." << endl;
            } else {
                cout << endl << "Stok tidak mencukupi!" << endl;
            }
        }
        cout << endl << "Tambah barang lagi? (Y/N): "; cin >> lanjut;
    } while (lanjut == 'Y' || lanjut == 'y');

    double diskon = isMember ? total * 0.05 : 0;
    double totalNett = total - diskon;

    judul("STRUK PEMBAYARAN");
    for (int i = 0; i < cartCount; i++) {
        double subtotal = cart[i].qty * cart[i].price;
        cout << cart[i].name << " x" << cart[i].qty << " = Rp" << subtotal << endl;
    }
    cout << "Total Gross     : Rp" << total << endl;
    cout << "Potongan Member : Rp" << diskon << endl;
    cout << "TOTAL TAGIHAN   : Rp" << totalNett << endl;

    double bayar;
    do {
        cout << endl << "Nominal Bayar: Rp"; cin >> bayar;

        if (bayar >= totalNett) {
            cout << "Kembalian: Rp" << (bayar - totalNett) << endl;
            cout << endl << "Transaksi berhasil!" << endl;
            if (isMember) cout << "Terima kasih, " << nama << "!" << endl;
            else cout << "Terima kasih telah berbelanja!" << endl;
            return 0;
        }

        cout << endl << "Uang tidak mencukupi! Kurang Rp" << fixed << setprecision(0) << (totalNett - bayar) << endl;
        judul("STRUK PEMBAYARAN");
        cout << "TOTAL TAGIHAN   : Rp" << totalNett << endl;
        cout << "[1] Lanjutkan Pembayaran\n[2] Kembali ke Menu\n[0] Logout" << endl;
        cout << "Pilihan: ";

        int aksi;
        if (!bacaAngka(aksi)) aksi = 1; // input aneh, anggap mau lanjut bayar lagi

        if (aksi == 2) return 2;
        if (aksi == 0) return 1;
        // aksi == 1 (atau lainnya) -> ulangi, minta nominal bayar lagi
    } while (true);
}

// sub-menu verifikasi customer: member atau guest
// input salah / member ID gagal cuma balik ke menu ini lagi (loop)
void customerMenu() {
    int pilihan;
    do {
        judul("MODE TRANSAKSI CUSTOMER");
        cout << "[1] Member\n[2] Guest\n[0] Kembali" << endl;
        cout << "Pilih Mode: ";
        if (!bacaAngka(pilihan)) {
            cout << endl << "Input tidak valid!" << endl;
            continue;
        }

        int hasil = -1; // -1 = tidak ada transaksi yang berjalan di iterasi ini

        if (pilihan == 1) {
            int memberID;
            cout << endl << "Masukkan Member ID: ";
            if (!bacaAngka(memberID)) {
                cout << endl << "Input tidak valid!" << endl;
                continue;
            }
            Member* m = cariMember(memberID);
            if (m != nullptr) {
                cout << endl << "Selamat datang kembali, " << m->name << "!" << endl;
                hasil = transaksi(true, m->name);
            } else {
                cout << endl << "Member ID tidak terdaftar!" << endl;
            }
        } else if (pilihan == 2) {
            hasil = transaksi(false);
        } else if (pilihan == 0) {
            cout << endl << "Kembali ke menu utama..." << endl;
        } else {
            cout << endl << "Pilihan tidak valid!" << endl;
        }

        if (hasil == 1) {
            // logout langsung dipilih dari layar pembayaran (uang kurang)
            cout << endl << "Logout berhasil." << endl;
            pilihan = 0;
        } else if (hasil == 0) {
            // transaksi berhasil, tanya mau lanjut transaksi lagi atau logout
            cout << endl << "[1] Kembali ke Menu\n[0] Logout" << endl;
            cout << "Pilihan: ";
            int aksi;
            if (!bacaAngka(aksi)) aksi = 1; // input aneh, default balik ke menu

            if (aksi == 0) {
                cout << endl << "Logout berhasil." << endl;
                pilihan = 0;
            }
        }
        // hasil == 2 (batal dari layar pembayaran) atau -1 (belum sempat transaksi)
        // langsung balik ke menu customer tanpa prompt tambahan
    } while (pilihan != 0);
}

int main() {
    int role = -1;
    do {
        judul("AIM MAIN SYSTEM");
        cout << "[1] Login Admin\n[2] Transaksi Customer\n[0] Keluar" << endl;
        cout << "Pilih Peran: ";
        if (!bacaAngka(role)) {
            cout << endl << "Input tidak valid!" << endl;
            role = -1;
            continue;
        }

        if (role == 1) {
            string password;
            cout << endl << "Password Admin: "; cin >> password;
            if (password == "admin123") {
                cout << endl << "Login Berhasil! Selamat datang, Admin." << endl;
                adminDashboard();
            } else {
                cout << endl << "Password salah!" << endl;
            }
        } else if (role == 2) {
            customerMenu();
        } else if (role == 0) {
            cout << endl << "Terima kasih telah menggunakan AIM." << endl;
        } else {
            cout << endl << "Pilihan tidak valid!" << endl;
        }
    } while (role != 0);
}
