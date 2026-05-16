/* =========================================================
 * report.c - Thống kê doanh thu và xuất hóa đơn
 * (Tính năng nâng cao - có điểm cộng)
 * ========================================================= */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "report.h"
#include "repair.h"
#include "customer.h"
#include "service.h"
#include "utils.h"
#include "constants.h"

Invoice invoices[MAX_REPAIR_ORDERS];
int invoiceCount = 0;
/* =========================================================
 * THỐNG KÊ DOANH THU NGÀY
 * ========================================================= */

void reportDailyRevenue(void) {
    time_t now = time(NULL);
    struct tm today = *localtime(&now);

    int day   = today.tm_mday;
    int month = today.tm_mon;
    int year  = today.tm_year;

    double totalRevenue = 0;
    int totalOrders = 0;

    for (int i = 0; i < orderCount; i++) {
        RepairOrder *o = &orders[i];
        if (o->status != STATUS_DONE) continue;
        struct tm orderDate = *localtime(&o->updatedDate);

        if (orderDate.tm_mday == day &&
            orderDate.tm_mon  == month &&
            orderDate.tm_year == year) {

            totalRevenue += o->totalAmount;
            totalOrders++;
        }
    }

    printHeader("DOANH THU TRONG NGAY");

    char moneyBuf[30];
    formatMoney(totalRevenue, moneyBuf);

    printf("  So phieu hoan thanh: %d\n", totalOrders);
    printf("  Tong doanh thu     : %s\n", moneyBuf);

    printDivider();
    
    printf("  Cu the:\n");

	for (int i = 0; i < orderCount; i++) {
	RepairOrder *o = &orders[i];

    if (o->status != STATUS_DONE) continue;

    struct tm orderDate = *localtime(&o->updatedDate);

    if (orderDate.tm_mday == day &&
        orderDate.tm_mon  == month &&
        orderDate.tm_year == year) {
        char buf[30];
        formatMoney(o->totalAmount, buf);
        printf(" + %s = %s\n", o->orderId, buf);

        totalRevenue += o->totalAmount;
        totalOrders++;
    	}
	}
}

/* =========================================================
 * TOP DỊCH VỤ BÁN CHẠY
 * ========================================================= */

void reportTopServices(void) {
    int quantities[MAX_SERVICES] = {0}; 
    int indices[MAX_SERVICES];          
    for (int i = 0; i < serviceCount; i++) {
        indices[i] = i;
    }

    for (int i = 0; i < orderCount; i++) {
        for (int j = 0; j < orders[i].itemCount; j++) {
            RepairItem *item = &orders[i].items[j];
            int sIdx = findServiceById(item->serviceId);
            if (sIdx != -1) {
                quantities[sIdx] += item->quantity;
            }
        }
    }
    for (int i = 0; i < serviceCount - 1; i++) {
        for (int j = i + 1; j < serviceCount; j++) {
            if (quantities[indices[i]] < quantities[indices[j]]) {
                int tempIdx = indices[i];
                indices[i] = indices[j];
                indices[j] = tempIdx;
            }
        }
    }
    printf("\n=================================================================\n");
    printf("                TOP 5 DICH VU DUOC SU DUNG NHIEU NHAT            \n");
    printf("=================================================================\n");
    printf("%-12s | %-35s | %-10s\n", "Ma DV", "Ten dich vu", "Tong SL");
    printf("-----------------------------------------------------------------\n");

    int printed = 0;
    for (int i = 0; i < serviceCount; i++) {
        int originalIdx = indices[i];
        if (quantities[originalIdx] == 0) {
            break;
        }
        printf("%-12s | %-35s | %d\n", 
            services[originalIdx].serviceId, 
            services[originalIdx].name, 
            quantities[originalIdx]);
        
        printed++;
        if (printed == 5) break;
    }

    if (printed == 0) {
        printf("Chua co du lieu dich vu hoac chua co hoa don nao!\n");
    }
    printf("=================================================================\n\n");
}

/* =========================================================
 * XUẤT HÓA ĐƠN
 * ========================================================= */

int createInvoice(const char * orderId){
    int orderIdx = findOrderById(orderId);
    if(orderIdx == -1){
        printError("Khong tim thay phieu!");
        return 0;
    }
    int customerIdx = findCustomerByPhone(orders[orderIdx].customerPhone);
    if(customerIdx == -1){
        printError("Khong tim thay khach hang!");
        return 0;
    }
    strcpy(invoices[invoiceCount].orderId, orders[orderIdx].orderId);
    strcpy(invoices[invoiceCount].customerName, customers[customerIdx].fullName);
    strcpy(invoices[invoiceCount].customerPhone, customers[customerIdx].phoneNumber);
    strcpy(invoices[invoiceCount].carType, customers[customerIdx].carType);
    strcpy(invoices[invoiceCount].symptom, orders[orderIdx].symptom);
    strcpy(invoices[invoiceCount].carPlate, customers[customerIdx].carPlate);
    for(int i = 0; i < orders[orderIdx].itemCount; i++){
        strcpy(invoices[invoiceCount].items[i].serviceId, orders[orderIdx].items[i].serviceId);
        strcpy(invoices[invoiceCount].items[i].serviceName, orders[orderIdx].items[i].serviceName);
        invoices[invoiceCount].items[i].quantity = orders[orderIdx].items[i].quantity;
        invoices[invoiceCount].items[i].unitPrice = orders[orderIdx].items[i].unitPrice;
        invoices[invoiceCount].items[i].subtotal = orders[orderIdx].items[i].subtotal;
    }
    invoices[invoiceCount].createdDate = time(NULL);
    invoices[invoiceCount].itemCount = orders[orderIdx].itemCount;
    invoices[invoiceCount].totalAmount = orders[orderIdx].totalAmount;
    invoiceCount++;
    printSuccess("Da tao hoa don trong he thong. Vui long chon [3] trong menu thong ke & hoa don de xuat hoa don ra file.");
    return 1;
}
int exportInvoice(const char *orderId) {
    int orderIndex = findOrderById(orderId);
    if (orderIndex == -1) {
        printf("Loi: Khong tim thay don hang mang ma %s!\n", orderId);
        return 0;
    }
    RepairOrder *o = &orders[orderIndex];
    
    char filename[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char datetime[64];
    strftime(datetime, sizeof(datetime), "%d/%m/%Y %H:%M:%S", t);
    sprintf(filename, "invoice_%s.txt", orderId);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Loi: Khong the tao file %s de ghi hoa don!\n", filename);
        return 0;
    }
    char bufTotal[30];
    formatMoney(o->totalAmount, bufTotal);
    fprintf(fp, "========================================================\n");
    fprintf(fp, "                    HOA DON DICH VU                     \n");
    fprintf(fp, "Ngay xuat     : %s\n", datetime);  
    fprintf(fp, "========================================================\n");
    fprintf(fp, "Ma don hang   : %s\n", o->orderId);
    fprintf(fp, "So dien thoai : %s\n", o->customerPhone);
    fprintf(fp, "Yeu cau/Loi   : %s\n", o->symptom);
    fprintf(fp, "--------------------------------------------------------\n");
    
    // Header bảng dịch vụ
    fprintf(fp, "%-4s | %-25s | %-4s | %-12s\n", "STT", "Ten dich vu", "SL", "Thanh tien");
    fprintf(fp, "--------------------------------------------------------\n");
    for (int i = 0; i < o->itemCount; i++) {
        RepairItem *it = &o->items[i];
        char bufSub[30];
        formatMoney(it->subtotal, bufSub);
        fprintf(fp, "%-4d | %-25s | %-4d | %-12s\n", 
                i + 1, 
                it->serviceName, 
                it->quantity, 
                bufSub);
    }
    fprintf(fp, "--------------------------------------------------------\n");
    fprintf(fp, "TONG CONG: %s\n", bufTotal); 
    fprintf(fp, "========================================================\n");
    fprintf(fp, "             XIN CAM ON VA HEN GAP LAI!                 \n");
    fclose(fp);
    printSuccess("Da xuat hoa don"); 
    
    return 1;
}

/* =========================================================
 * MENU THỐNG KÊ
 * ========================================================= */

void reportMenu(void) {
    int choice;
    do {
        printHeader("THONG KE & HOA DON");
        printf("  [1] Doanh thu trong ngay\n");
        printf("  [2] Dich vu ban chay nhat\n");
        printf("  [3] Xuat hoa don phieu sua\n");
        printf("  [0] Quay lai\n");
        printDivider();
        printf("  Lua chon: ");
        scanf(" %d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: reportDailyRevenue(); break;
            case 2: reportTopServices();  break;
            case 3: {
                char oid[ID_LEN];
                printf("  Nhap ma phieu (0 de quay lai): ");
                readLine(oid, ID_LEN);
                if (strcmp(oid, "0") == 0) break;
                exportInvoice(oid);
                break;
            }
            case 0: break;
            default: printError("Lua chon khong hop le.");
        }
    } while (choice != 0);
}
