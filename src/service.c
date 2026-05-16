/* =========================================================
 * service.c - Cài đặt quản lý danh mục dịch vụ
 * ========================================================= */

#include <stdio.h>
#include <string.h>
#include "service.h"
#include "fileio.h"
#include "utils.h"
#include "constants.h"

/* --- Định nghĩa bộ nhớ toàn cục --- */
Service services[MAX_SERVICES];
int     serviceCount = 0;

/* =========================================================
 * KHỞI TẠO
 * ========================================================= */
 
void initServices(void) {
    serviceCount = 0;
    memset(services, 0, sizeof(services));
}

/* =========================================================
 * THÊM DỊCH VỤ
 * ========================================================= */

    /* TODO:
     * 1. Kiểm tra serviceCount < MAX_SERVICES
     * 2. Nhập tên dịch vụ (không rỗng)
     * 3. Nhập đơn giá (phải > 0)
     * 4. generateServiceId(serviceCount + 1, services[serviceCount].serviceId)
     * 5. Gán isActive = 1; serviceCount++;
     * 6. saveServices(); printSuccess(); return 1;
     */
    /* 1. Kiểm tra giới hạn mảng */
    if (serviceCount >= MAX_SERVICES) {
        printError("He thong da day, khong the them dich vu moi.");
        return 0;
    }

    char tempName[SERVICE_NAME_LEN];
    double tempPrice;

    /* 2. Nhập tên dịch vụ (không rỗng) */
    do {
        printf("  Ten dich vu (0 de quay lai): ");
        readLine(tempName, SERVICE_NAME_LEN);
        if (strcmp(tempName, "0") == 0) return 0;
        if (tempName[0] == '\0') {
            printError("Ten dich vu khong duoc de trong.");
        }
    } while (tempName[0] == '\0');

    /* 3. Nhập đơn giá (phải > 0) */
    do {
        printf("  Don gia (VND, 0 de quay lai): ");
        if (scanf("%lf", &tempPrice) != 1) {
            while (getchar() != '\n'); /* Xóa buffer nếu nhập sai kiểu (chữ) */
            printError("Don gia phai la mot so.");
            tempPrice = -1;
        } else {
            while (getchar() != '\n'); /* Xóa buffer sau khi lấy số */
            if (tempPrice == 0) return 0;
            if (tempPrice < 0) {
                printError("Don gia phai lon hon 0.");
            }
        }
    } while (tempPrice <= 0);

    /* 4. Khởi tạo mã DV và đưa vào mảng */
    strcpy(services[serviceCount].name, tempName);
    services[serviceCount].unitPrice = tempPrice;
    services[serviceCount].isActive = 1;
    generateServiceId();

    /* 5. Tăng biến đếm và lưu file */
    serviceCount++;
    saveServices();
    
    printSuccess("Da them dich vu thanh cong!");
    printf("  Ma DV duoc cap: %s\n", services[serviceCount - 1].serviceId);
        
    return 1; 
}

/* =========================================================
 * SỬA DỊCH VỤ
 * ========================================================= */

int editService(void) {
    /* TODO:
     * 1. listAllServices()
     * 2. Nhập serviceId cần sửa
     * 3. int idx = findServiceById(id); if (idx == -1) { lỗi; return 0; }
     * 4. Menu: [1] Sửa tên  [2] Sửa đơn giá  [0] Huỷ
     * 5. Cập nhật, saveServices(), printSuccess(), return 1
     */


    listAllServices();
    if (serviceCount == 0) return 0; /* Không có dịch vụ để sửa */

    char id[ID_LEN];
    printf("  Nhap ma dich vu can sua (0 de quay lai, VD: SV000001): ");
    readLine(id, ID_LEN);
    if (strcmp(id, "0") == 0) return 0;

    int idx = findServiceById(id);
    if (idx == -1) {
        printError("Khong tim thay dich vu hoac dich vu khong hoat dong.");
        return 0;
    }


    int choice;
    printf("  Dich vu dang chon: %s - %s\n", services[idx].serviceId, services[idx].name);
    printf("  [1] Sua ten dich vu\n");
    printf("  [2] Sua don gia\n");
    printf("  [3] Kich hoat/Tam ngung dich vu\n");
    printf("  [0] Huy\n");
    printf("  Lua chon: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

  
    if (choice == 1) {
        char newName[SERVICE_NAME_LEN];
        do {
            printf("  Ten dich vu moi (0 de quay lai): ");
            readLine(newName, SERVICE_NAME_LEN);
            if (strcmp(newName, "0") == 0) return 0;
            if (newName[0] == '\0') {
                printError("Ten dich vu khong duoc de trong.");
            }
        } while (newName[0] == '\0');
        strcpy(services[idx].name, newName);

    } else if (choice == 2) {
        double newPrice;
        do {
            printf("  Don gia moi (VND, 0 de quay lai): ");
            if (scanf("%lf", &newPrice) != 1) {
                while (getchar() != '\n');
                printError("Don gia phai la mot so.");
                newPrice = -1;
            } else {
                while (getchar() != '\n');
                if (newPrice == 0) return 0;
                if (newPrice < 0) {
                    printError("Don gia phai lon hon 0.");
                }
            }
        } while (newPrice <= 0);
        services[idx].unitPrice = newPrice;

    } 
    else if (choice == 3) {
        services[idx].isActive = !services[idx].isActive;
        printf("  Dich vu da duoc %s.\n", services[idx].isActive ? "kich hoat" : "tam ngung");
    }
    else if (choice == 0) {
        printf("  Da huy thao tac.\n");
        return 0;
    } else {
        printError("Lua chon khong hop le.");
        return 0;
    }


    saveServices();
    printSuccess("Da cap nhat thong tin dich vu.");
    return 1;

}

/* =========================================================
 * TÌM KIẾM
 * ========================================================= */

int findServiceById(const char *serviceId) {
    int index = -1;
    for(int i = 0; i < serviceCount; i++){
        if(strcmp(services[i].serviceId, serviceId) == 0){
            index = i;
            break;
        }
    }
    return index;
}

/* =========================================================
 * HIỂN THỊ
 * ========================================================= */

void listAllServices(void) {
    /* TODO:
     * In header: STT | Mã DV | Tên dịch vụ | Đơn giá
     * Duyệt for, chỉ in isActive == 1; dùng formatMoney cho đơn giá
     */
    int activeCount = 0;
    for (int i = 0; i < serviceCount; i++) {
        if (services[i].isActive == 1) {
            activeCount++;
        }
    }


    if (serviceCount == 0) {
        printError("Chua co dich vu nao trong he thong!");
        return;
    }

    /* In Header */
    printDivider();
    printf("  %-4s %-10s %-30s %-20s %-20s\n", "STT", "Ma DV", "Ten dich vu", "Don gia", "Trang thai");
    printDivider();


    int stt = 1;
    for (int i = 0; i < serviceCount; i++) {
        char priceBuf[30];
        formatMoney(services[i].unitPrice, priceBuf); 
        printf("  %-4d %-10s %-30s %-20s %-20s\n",
                stt++,
                services[i].serviceId,
                services[i].name,
                priceBuf,
                services[i].isActive ? "Hoat dong" : "Khong hoat dong");
    }
    printDivider();
    printf("  Tong so dich vu dang hoat dong: %d\n", activeCount);
}
