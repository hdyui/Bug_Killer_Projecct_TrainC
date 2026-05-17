/* =========================================================
 * customer.c - Cài đặt quản lý khách hàng
 * ========================================================= */

#include <stdio.h>
#include <string.h>
#include "customer.h"
#include "fileio.h"
#include "utils.h"
#include "constants.h"
#include "models.h"

/* --- Định nghĩa bộ nhớ toàn cục --- */

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;

/* =========================================================
 * THÊM KHÁCH HÀNG
 * ========================================================= */

int addCustomer(void) {
    char tempName[NAME_LEN];
    char tempPhone[PHONE_LEN];
    char tempPlate[PLATE_LEN];
    char tempType[CAR_TYPE_LEN];
    int  foundIdx;

    if (customerCount >= MAX_CUSTOMERS) {
        printError("He thong da day, khong the them khach hang moi.");
        return 0;
    }
    do {
        printf("  Ho va ten (0 de quay lai): ");
        scanf(" %99[^\n]", tempName);   
        while (getchar() != '\n');      
        if (strcmp(tempName, "0") == 0) return 0;
        if ( !isValidName(tempName) ) {
            printError("Ho ten khong hop le.");
        }
    } while ( !isValidName(tempName) );
 
    while (1) {
        printf("  So dien thoai (0 de quay lai): ");
        scanf(" %10s", tempPhone);      
        while (getchar() != '\n');
        if (strcmp(tempPhone, "0") == 0) return 0;
        if (isValidPhone(tempPhone) == 0) {
            printError("SDT khong hop le (chi chua so, 10 ky tu).");
            continue;
        }
        foundIdx = findCustomerByPhone(tempPhone);
        if (foundIdx != -1) {
            printError("SDT nay da ton tai trong he thong.");
            return 0; 
        }
        break;
    }
    while (1) {
        printf("  Bien so xe (0 de quay lai, VD: 51FF-123.45): ");
        scanf(" %14s", tempPlate);
        while (getchar() != '\n');
        if (strcmp(tempPlate, "0") == 0) return 0;
        if (isValidPlate(tempPlate) == 0) {
            printError("Bien so khong hop le.");
            continue;
        }
        break;
    }
    do {
        printf("  Loai xe (0 de quay lai, VD: Xe may, O to, Xe dap dien): ");
        scanf(" %29[^\n]", tempType);
        while (getchar() != '\n');
        if (strcmp(tempType, "0") == 0) return 0;
        if (!isValidName(tempType)) {
            printError("Loai xe khong hop le.");
        }
    } while (!isValidName(tempType));
    strcpy(customers[customerCount].fullName,    tempName);
    strcpy(customers[customerCount].phoneNumber, tempPhone);
    strcpy(customers[customerCount].carPlate,    tempPlate);
    strcpy(customers[customerCount].carType,     tempType);
    customers[customerCount].orderCount = 0;
    generateCustomerId();
    customerCount++;
    saveCustomers();
    printSuccess("Da them khach hang thanh cong!");
    printf("  Ma KH duoc cap: %s\n", customers[customerCount - 1].customerId);
    return 1;
}

/* =========================================================
 * SỬA KHÁCH HÀNG
 * ========================================================= */

int editCustomer(void) {
	
    char phone[PHONE_LEN];
    int  idx;
    int  choice;
    do {
    	printf("  Nhap SDT khach hang can sua (0 de quay lai): ");
    	scanf(" %14s", phone);
        if (strcmp(phone, "0") == 0) return 0;
    	if( !isValidPhone(phone) ){
    		printError("  SDT khong hop le, xin hay nhap lai!");
		}
	}
    while (!isValidPhone(phone));
 
    idx = findCustomerByPhone(phone);
    if (idx == -1) {
        printError("Khong tim thay khach hang voi SDT nay.");
        return 0;
    }
    printCustomer(&customers[idx]);
 
    printf("  [1] Sua ho ten\n");
    printf("  [2] Sua bien so xe\n");
    printf("  [3] Sua loai xe\n");
    printf("  [0] Huy\n");
    printf("  Lua chon: ");
    scanf(" %d", &choice);
    while (getchar() != '\n');
 
    if (choice == 1) {
        char tempName[NAME_LEN];
        do {
            printf("  Ho ten moi (0 de quay lai): ");
            scanf(" %99[^\n]", tempName);
            while (getchar() != '\n');
            if (strcmp(tempName, "0") == 0) return 0;
            if (!isValidName(tempName)) {
                printError("Ho ten khong hop le.");
            }
        } while (!isValidName(tempName));
        strcpy(customers[idx].fullName, tempName);
 
    } else if (choice == 2) {
 
        char newPlate[PLATE_LEN];
        while (1) {
            printf("  Bien so xe moi (0 de quay lai): ");
            scanf(" %14s", newPlate);
            while (getchar() != '\n');
            if (strcmp(newPlate, "0") == 0) return 0;
            if (isValidPlate(newPlate) == 0) {
                printError("Bien so khong hop le.");
                continue;
            }
            break;
        }
        strcpy(customers[idx].carPlate, newPlate);
 
    } else if (choice == 3) {
 
        char tempType[CAR_TYPE_LEN];
        do {
            printf("  Loai xe moi (0 de quay lai): ");
            scanf(" %29[^\n]", tempType);
            while (getchar() != '\n');
            if (strcmp(tempType, "0") == 0) return 0;
            if (!isValidName(tempType) ) {
                printError("Loai xe khong hop le.");
            }
        } while (!isValidName(tempType));
        strcpy(customers[idx].carType, tempType);
 
    } else if (choice == 0) {
        puts("  Da huy thao tac.");
        return 0;
 
    } else {
        printError("Lua chon khong hop le.");
        return 0;
    }
 
    saveCustomers();
    printSuccess("Da cap nhat thong tin khach hang.");
    return 1;
}

/* =========================================================
 * TÌM KIẾM
 * ========================================================= */

int findCustomerByPhone(const char *phone) {
    int index = -1;
    for (int i = 0; i < customerCount; i++){
        if (strcmp(customers[i].phoneNumber, phone) == 0){
            index = i;
            break;
        }
    }
    return index;
}

int findCustomerByPlate(const char *plate) {
    int i;
    for (i = 0; i < customerCount; i++) {
        if (strCmpIgnoreCase(customers[i].carPlate, plate) == 0) {
            return i;
        }
    }
    return -1; 
}


void searchCustomerMenu(void) {
    int choice;
    int idx;
 
    /* Hiển thị lựa chọn tìm kiếm */
    do {
        printf("  [1] Tim theo SDT\n");
        printf("  [2] Tim theo bien so xe\n");
        printf("  [0] Quay lai\n");
        printf("  Lua chon: ");
        scanf(" %d", &choice);
        while (getchar() != '\n');
        
        if (choice != 1 && choice != 2 && choice != 0) {
            printError("Lua chon khong hop le.");
        }
    } while (choice != 1 && choice != 2 && choice != 0);
 
    idx = -1;
 
    if (choice == 1) {
    	char phone[PHONE_LEN];
        do {
        	printf("  Nhap SDT (nhap 0 de quay lai): ");
        	scanf(" %11s", phone);
            while (getchar() != '\n');
            if(strcmp(phone, "0") == 0) return;
        	if (!isValidPhone(phone)) {
                printError("SDT khong hop le.");
            }
		}
        while(!isValidPhone(phone) );
		idx = findCustomerByPhone(phone);
 
    } else if (choice == 2) {
    	char plate[PLATE_LEN];
        do {
			printf("  Nhap bien so xe (nhap 0 de quay lai): ");
            scanf(" %11s", plate);
            while (getchar() != '\n');
            if(strcmp(plate, "0") == 0) return;
        	if (!isValidPlate(plate)) {
                printError("Bien so khong hop le.");
            }
    	}
        while(!isValidPlate(plate));
        idx = findCustomerByPlate(plate);
 
    } else if (choice == 0) {
        return;
    }
 
    /* In kết quả */
    if (idx == -1) {
        printError("Khong tim thay khach hang.");
    } else {
        printCustomer(&customers[idx]);
    }
}


/* =========================================================
 * HIỂN THỊ
 * ========================================================= */

void printCustomer(const Customer *c) {
    /* TODO:
     * printf("  Ma KH    : %s\n", c->customerId);
     * printf("  Ho ten   : %s\n", c->fullName);
     * ... các trường còn lại
     */
    printDivider();
    printf("  Ma KH     : %s\n", c->customerId);
    printf("  Ho ten    : %s\n", c->fullName);
    printf("  SDT       : %s\n", c->phoneNumber);
    printf("  Bien so   : %s\n", c->carPlate);
    printf("  Loai xe   : %s\n", c->carType);
    printf("  So phieu  : %d\n", c->orderCount);
    printDivider();
}

void listAllCustomers(void) {
    if (customerCount == 0) {
        puts("  Chua co khach hang nao trong he thong.");
        return;
    }
 
    /* In header bảng */
    printDivider();
    printf("  %-4s %-10s %-25s %-12s %-12s %-15s %s\n",
           "STT", "Ma KH", "Ho ten", "SDT", "Bien so", "Loai xe", "Phieu");
    printDivider();
 
    /* Duyệt vòng for in từng dòng */
    for (int i = 0; i < customerCount; i++) {
        printf("  %-4d %-10s %-25s %-12s %-12s %-15s %d\n",
               i + 1,
               customers[i].customerId,
               customers[i].fullName,
               customers[i].phoneNumber,
               customers[i].carPlate,
               customers[i].carType,
               customers[i].orderCount);
    }
    printDivider();
    printf("  Tong so khach hang: %d\n", customerCount);
}