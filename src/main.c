/* =========================================================
 * main.c - Điểm vào chương trình, menu chính
 * Chỉ chứa điều hướng - không viết logic nghiệp vụ ở đây
 * ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "utils.h"
#include "customer.h"
#include "service.h"
#include "repair.h"
#include "report.h"
#include "fileio.h"

/* Khai báo các hàm menu con (định nghĩa bên dưới) */
static void menuCustomer(void);
static void menuRepair(void);
static void menuService(void);

/* =========================================================
 * MAIN
 * ========================================================= */

int main(void) {
    int choice;
    loadAllData();
    do {
        printHeader("QUAN LY TIEM SUA XE - NHOM BUG KILLER");
        printf("  [1] Quan ly khach hang\n");
        printf("  [2] Quan ly phieu sua chua\n");
        printf("  [3] Quan ly danh muc dich vu\n");
        printf("  [4] Thong ke & Hoa don\n");
        printf("  [0] Thoat chuong trinh\n");
        printDivider();
        printf("  Lua chon: ");
        scanf(" %d", &choice);
        while (getchar() != '\n');
        switch (choice) {
            case 1:
                menuCustomer(); 
                break;
            
            case 2:
                menuRepair();   
                break;
            
            case 3:
                menuService();  
                break;
            
            case 4: 
                reportMenu();   
                break;
            case 0:
                saveAllData();
                printSuccess("Da luu du lieu. Tam biet!");
                break;
            default:
                printError("Lua chon khong hop le. Vui long thu lai.");
        }
    } while (choice != 0);
    return 0;
}

/* =========================================================
 * MENU KHÁCH HÀNG
 * ========================================================= */

static void menuCustomer(void) {
    int choice;
    do {
        printHeader("QUAN LY KHACH HANG");
        printf("  [1] Them khach hang moi\n");
        printf("  [2] Sua thong tin khach hang\n");
        printf("  [3] Tim kiem khach hang\n");
        printf("  [4] Xem danh sach tat ca khach hang\n");
        printf("  [0] Quay lai\n");
        printDivider();
        printf("  Lua chon: ");
        scanf(" %d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: addCustomer();        break;
            case 2: editCustomer();       break;
            case 3: searchCustomerMenu(); break;
            case 4: listAllCustomers();   break;
            case 0: break;
            default: printError("Lua chon khong hop le.");
        }
    } while (choice != 0);
}

/* =========================================================
 * MENU PHIẾU SỬA
 * ========================================================= */

static void menuRepair(void) {
    int choice;
    do {
        printHeader("QUAN LY PHIEU SUA CHUA");
        printf("  [1] Tao phieu sua moi\n");
        printf("  [2] Cap nhat trang thai phieu\n");
        printf("  [3] Xem chi tiet phieu\n");
        printf("  [4] Xem tat ca phieu\n");
        printf("  [5] Loc phieu theo trang thai\n");
        printf("  [6] Lich su sua chua cua khach hang\n");
        printf("  [7] Tim kiem phieu\n");
        printf("  [0] Quay lai\n");
        printDivider();
        printf("  Lua chon: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: 
                createRepairOrder();   
                break;
            case 2: 
                updateOrderStatus();   
                break;
            case 3: {
                char oid[ID_LEN];
                printf("  Nhap ma phieu (0 de quay lai): ");
                readLine(oid, ID_LEN);
                if (strcmp(oid, "0") == 0) break;
                int idx = findOrderById(oid);
                if (idx == -1) printError("Khong tim thay phieu.");
                else printOrder(&orders[idx]);
                break;
            }
            case 4: 
                listOrders(-1);        
                break;
            case 5: {
                int st = -1;
                printf("  Trang thai [0=Tiep nhan, 1=Dang sua, 2=Hoan thanh, 3=Quay lai]: ");
                if (scanf(" %d", &st) != 1) {
                    while (getchar() != '\n');
                    printError("Trang thai khong hop le.");
                    break;
                }
                while (getchar() != '\n');
                if (st == 3) break;
                if (st < 0 || st > 2) {
                    printError("Trang thai khong hop le.");
                    break;
                }
                listOrders(st);
                break;
            }
            case 6: 
                viewCustomerHistory(); 
                break;
            case 7: 
                searchOrderMenu();     
                break;
            case 0: 
                break;
            default: 
                printError("Lua chon khong hop le.");
        }
    } while (choice != 0);
}

/* =========================================================
 * MENU DỊCH VỤ
 * ========================================================= */

static void menuService(void) {
    int choice;
    do {
        printHeader("QUAN LY DANH MUC DICH VU");
        printf("  [1] Them dich vu moi\n");
        printf("  [2] Sua dich vu\n");
        printf("  [3] Xem danh sach dich vu\n");
        printf("  [0] Quay lai\n");
        printDivider();
        printf("  Lua chon: ");
        scanf(" %d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: addService();      break;
            case 2: editService();     break;
            case 3: loadServices(); listAllServices(); break;
            case 0: break;
            default: printError("Lua chon khong hop le.");
        }
    } while (choice != 0);
}
