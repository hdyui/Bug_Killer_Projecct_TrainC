/* =========================================================
 * fileio.c - Đọc/ghi dữ liệu ra file nhị phân
 * ========================================================= */

#include <stdio.h>
#include "fileio.h"
#include "customer.h"
#include "repair.h"
#include "service.h"
#include "utils.h"
#include "constants.h"
#include <stdlib.h>

/* =========================================================
 * TIỆN ÍCH
 * ========================================================= */

void loadAllData(void) {
	loadCustomers();
    loadServices();
    loadOrders();
}

void saveAllData(void) {
	saveCustomers(); 
    saveServices();  
    saveOrders();    
}


/* =========================================================
 * KHÁCH HÀNG
 * ========================================================= */
 

int saveCustomers(void) {

    if (!validateCustomers()) {
        printError("Du lieu khach hang khong hop le!");
        debugValidateCustomers(); // in chi tiet
        return 0; // KHÔNG lưu
    }

    FILE *fp = fopen(FILE_CUSTOMERS, "w");

    if (!fp) {
        printError("Khong the ghi file khach hang.");
        return 0;
    }

    fprintf(fp, "%d\n", customerCount);

    for (int i = 0; i < customerCount; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%d\n",
            customers[i].customerId,
            customers[i].fullName,
            customers[i].phoneNumber,
            customers[i].carPlate,
            customers[i].carType,
            customers[i].orderCount
        );
    }

    fclose(fp);
    //printSuccess("Da luu du lieu khach hang!");
    return 1;
}

int loadCustomers(void) {
    FILE *fp = fopen(FILE_CUSTOMERS, "r");
    if (!fp) return 0;

    fscanf(fp, "%d\n", &customerCount);

    for (int i = 0; i < customerCount; i++) {
        fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d\n",
            customers[i].customerId,
            customers[i].fullName,
            customers[i].phoneNumber,
            customers[i].carPlate,
            customers[i].carType,
            &customers[i].orderCount
        );
    }
    
    if (!validateCustomers()) {
       printError("Du lieu khach hang co loi");
       debugValidateCustomers();
       return 0;
    }

    fclose(fp);
    return 1;

}

/* =========================================================
 * PHIẾU SỬA
 * ========================================================= */

int saveOrders(void) {

    if (!validateOrders()) {
        printError("Du lieu phieu sua khong hop le!");
        debugValidateOrders();
        return 0;
    }

    FILE *fp = fopen(FILE_ORDERS, "w");

    if (!fp) {
        printError("Khong the ghi file phieu sua.");
        return 0;
    }

    fprintf(fp, "%d\n", orderCount);

    for (int i = 0; i < orderCount; i++) {
        RepairOrder *o = &orders[i];

        fprintf(fp, "%s|%s|%s|%d|%ld|%ld|%d|%.2f\n",
            o->orderId,
            o->customerPhone,
            o->symptom,
            o->status,
            o->createdDate,
            o->updatedDate,
            o->itemCount,
            o->totalAmount
        );

        for (int j = 0; j < o->itemCount; j++) {
            RepairItem *it = &o->items[j];

            fprintf(fp, "%s|%s|%d|%.2f|%.2f\n",
                it->serviceId,
                it->serviceName,
                it->quantity,
                it->unitPrice,
                it->subtotal
            );
        }
    }

    fclose(fp);
    printSuccess("Da luu phieu sua!");
    return 1;
}

int loadOrders(void) {
    FILE *fp = fopen(FILE_ORDERS, "r");
    
    fscanf(fp, "%d\n", &orderCount);
    
    for (int i = 0; i < orderCount; i++) {
        RepairOrder *o = &orders[i];

        fscanf(fp, "%[^|]|%[^|]|%[^|]|%d|%ld|%ld|%d|%lf\n",
            o->orderId,
            o->customerPhone,
            o->symptom,
            &o->status,
            &o->createdDate,
            &o->updatedDate,
            &o->itemCount,
            &o->totalAmount
        );
        
        for (int j = 0; j < o->itemCount; j++) {
            RepairItem *it = &o->items[j];
            
            fscanf(fp, "%[^|]|%[^|]|%d|%lf|%lf\n",
                it->serviceId,
                it->serviceName,
                &it->quantity,
                &it->unitPrice,
                &it->subtotal
            );
        }
    }
    
    if (!fp){
        printError("Du lieu phieu sua khong hop le!");
        debugValidateOrders();
        return 0;
    }
    fclose(fp);
    return 1;
}

/* =========================================================
 * DANH MỤC DỊCH VỤ
 * ========================================================= */

int saveServices(void) {
    
    if (!validateServices()) {
        printError("Du lieu dich vu khong hop le. Khong the luu!");
        debugValidateServices();
        return 0;
    }

    FILE *fp = fopen(FILE_SERVICES, "w");

    if (!fp) {
        printError("Khong the ghi file dich vu.");
        return 0;
    }

    for (int i = 0; i < serviceCount; i++) {

        fprintf(fp, "Service ID  : %s\n", services[i].serviceId);
        fprintf(fp, "Name        : %s\n", services[i].name);
        fprintf(fp, "Unit Price  : %.2f\n", services[i].unitPrice);
        fprintf(fp, "Is Active   : %d\n", services[i].isActive);
        fprintf(fp, "-----------------------------------\n");
    }

    fclose(fp);
    return 1;
}

int loadServices(void) {
    FILE *fp = fopen(FILE_SERVICES, "r");

    
    char line[256];
    serviceCount = 0;
    
    while (fgets(line, sizeof(line), fp)) {

        // Tìm dòng bắt đầu của 1 service
        if (strncmp(line, "Service ID", 10) == 0) {

            if (serviceCount >= MAX_SERVICES) break;
            
            Service *s = &services[serviceCount];

            // --- Service ID ---
            sscanf(line, "Service ID  : %s", s->serviceId);
            
            // --- Name ---
            if (fgets(line, sizeof(line), fp)) {
                sscanf(line, "Name        : %[^\n]", s->name);
            }
            
            // --- Unit Price ---
            if (fgets(line, sizeof(line), fp)) {
                sscanf(line, "Unit Price  : %lf", &s->unitPrice);
            }
            
            // --- Is Active ---
            if (fgets(line, sizeof(line), fp)) {
                sscanf(line, "Is Active   : %d", &s->isActive);
            }
            
            // --- Skip dòng gạch ---
            fgets(line, sizeof(line), fp);
            
            serviceCount++;
        }
    }
    if (!validateServices()) {
        printError("Du lieu dich vu khong hop le");
        debugValidateServices();
        return 0;
    }

    fclose(fp);
    return 1;
}
