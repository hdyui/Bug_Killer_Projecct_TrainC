/* =========================================================
 * utils.c - Cài đặt các hàm tiện ích dùng chung
 * ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"
#include "constants.h"
#include "models.h"


/* =========================================================
 * VALIDATE
 * ========================================================= */

int isValidPhone(const char *phone) {
     if ( phone != NULL && strlen(phone) == 10 ){
     	for ( int i = 0;  i < strlen(phone); i++){
     		if ( !isdigit(phone[i])) return 0;
		 }
		 return 1;
	 }
    return 0; 
}

int isValidPlate(const char *plate) {
    if (plate == NULL) return 0;
    /* 65AA-123.45 => độ dài = 10 */
    if (strlen(plate) != 11) return 0;
    if (!isdigit(plate[0])) return 0;
    if (!isdigit(plate[1])) return 0;
    if (!isalpha(plate[2])) return 0;
    if (!isalpha(plate[3])) return 0;
    if (plate[4] != '-')    return 0;
    if (!isdigit(plate[5])) return 0;
    if (!isdigit(plate[6])) return 0;
    if (!isdigit(plate[7])) return 0;
    if (plate[8] != '.')    return 0;
    if (!isdigit(plate[9])) return 0;
    if (!isdigit(plate[10])) return 0;
    return 1;
}

int isValidName(const char *name) {
	if (name == NULL || *name == '\0')
        return 0;

    while (*name)
    {
        if (!isalpha(*name) && *name != ' ')
            return 0;
        name++;
    }
    return 1;
}
int validateServices(void) {
    for (int i = 0; i < serviceCount; i++) {

        if (strlen(services[i].serviceId) == 0)
            return 0;

        if (strlen(services[i].name) == 0)
            return 0;

        if (services[i].unitPrice <= 0)
            return 0;

        if (services[i].isActive != 0 && services[i].isActive != 1)
            return 0;
    }
    return 1;
}

void debugValidateServices(void) {
    for (int i = 0; i < serviceCount; i++) {

        if (strlen(services[i].serviceId) == 0)
            printError("Service ID rong!");

        if (strlen(services[i].name) == 0)
            printError("Ten dich vu rong!");

        if (services[i].unitPrice <= 0)
            printError("Don gia khong hop le (phai > 0)!");

        if (services[i].isActive != 0 && services[i].isActive != 1)
            printError("Trang thai hoat dong khong hop le (chi duoc 0 hoac 1)!");
    }
}

int validateCustomers(void) {
    for (int i = 0; i < customerCount; i++) {

        if (!isValidName(customers[i].fullName))
            return 0;

        if (!isValidPhone(customers[i].phoneNumber))
            return 0;

        if (!isValidPlate(customers[i].carPlate))
            return 0;

        if (!isValidName(customers[i].carType))
            return 0;
    }
    return 1;
}
void debugValidateCustomers(void) {
    for (int i = 0; i < customerCount; i++) {

        if (!isValidName(customers[i].fullName))
            printError("Ten khach hang khong hop le!");

        if (!isValidPhone(customers[i].phoneNumber))
            printError("So dien thoai khong hop le!");

        if (!isValidPlate(customers[i].carPlate))
            printError("Bien so khong hop le!");

        if (!isValidName(customers[i].carType))
            printError("Loai xe khong hop le!");
    }
}

int validateOrders(void) {
    for (int i = 0; i < orderCount; i++) {

        if (strlen(orders[i].orderId) == 0)
            return 0;

        if (!isValidPhone(orders[i].customerPhone))
            return 0;

        if (orders[i].status < 0 || orders[i].status > 2)
            return 0;

        if (orders[i].itemCount < 0 || orders[i].itemCount > MAX_ITEMS_PER_ORDER)
            return 0;

        if (orders[i].totalAmount < 0)
            return 0;

        for (int j = 0; j < orders[i].itemCount; j++) {
            if (strlen(orders[i].items[j].serviceId) == 0)
                return 0;
        }
    }
    return 1;
}

void debugValidateOrders(void) {
    for (int i = 0; i < orderCount; i++) {

        if (strlen(orders[i].orderId) == 0)
            printError("Order ID rong!");

        if (!isValidPhone(orders[i].customerPhone))
            printError("SDT khong hop le!");

        if (orders[i].status < 0 || orders[i].status > 2)
            printError("Trang thai khong hop le!");

        if (orders[i].itemCount < 0 || orders[i].itemCount > MAX_ITEMS_PER_ORDER)
            printError("So item khong hop le!");

        if (orders[i].totalAmount < 0)
            printError("Tong tien khong hop le!");

        for (int j = 0; j < orders[i].itemCount; j++) {
            if (strlen(orders[i].items[j].serviceId) == 0)
                printError("Service ID rong!");
        }
    }
}

/* =========================================================
 * CHUỖI
 * ========================================================= */

int strCmpIgnoreCase(const char *a, const char *b) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') { 
        char ca = tolower((unsigned char)a[i]);
        char cb = tolower((unsigned char)b[i]);
        if (ca != cb) {
            return ca - cb; 
        }
        i++;
    }

    return tolower((unsigned char)a[i]) - tolower((unsigned char)b[i]); 
}

void strTrim(char *str) {
    if (str == NULL) return;
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    char *end = str + strlen(str) - 1;

    while (end >= str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    } 
}

void readLine(char *buffer, int maxLen) {
    if (fgets(buffer, maxLen, stdin) == NULL) {
        buffer[0] = '\0'; // nếu lỗi → chuỗi rỗng
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';

    strTrim(buffer);
}

/* =========================================================
 * SINH ID
 * ========================================================= */

void generateCustomerId() {
    sprintf(customers[customerCount].customerId, "CU%06d", customerCount+1);

}

void generateOrderId() {
    sprintf(orders[orderCount].orderId,"RO%06d", orderCount + 1);
}

void generateServiceId() {
    sprintf(services[serviceCount].serviceId, "SV%06d", serviceCount + 1);
}


/* =========================================================
 * THỜI GIAN
 * ========================================================= */

void formatDateTime(time_t t, char *buffer) {
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 20, "%d/%m/%Y %H:%M", tm_info);
}


void getTodayString(char *buffer) {
    time_t now = time(NULL);                 // lấy thời gian hiện tại
    struct tm *tm_info = localtime(&now);    // chuyển sang dạng ngày giờ dễ đọc
    strftime(buffer, 12, "%d/%m/%Y", tm_info); // format thành chuỗi
    
}
char * getStatusString(int status){
    char * message;
    switch(status){
        case STATUS_RECEIVED:
            message = "Tiep nhan";
            break;
        case STATUS_REPAIRING:
            message =  "Dang sua";
            break;
        case STATUS_DONE:
            message = "Hoan thanh";
            break;
        default:
            message = "Khong xac dinh";
    }
    return message;
}

/* =========================================================
 * HIỂN THỊ
 * ========================================================= */

void printDivider(void) {
    for (int i = 0; i < 60; i++) {
        printf("-");
    }
    printf("\n");
}

void printHeader(const char *title) {
    printDivider();
    printf(COLOR_BOLD COLOR_CYAN "  %s\n" COLOR_RESET, title);
    printDivider(); 
}

void printSuccess(const char *msg) {

    printf(COLOR_GREEN "[OK] %s\n" COLOR_RESET, msg);
}

void printError(const char *msg) {
    printf(COLOR_RED "[LOI] %s\n" COLOR_RESET, msg);
}

void printStatus(int status) {
    switch (status) {
        case STATUS_RECEIVED:
            printf(COLOR_GRAY "Tiep nhan" COLOR_RESET);
            break;
        case STATUS_REPAIRING:
            printf(COLOR_YELLOW "Dang sua" COLOR_RESET);
            break;
        case STATUS_DONE:
            printf(COLOR_GREEN "Hoan thanh" COLOR_RESET);
            break;
        default:
            printf(COLOR_RED "Khong xac dinh" COLOR_RESET);
    }
}
void formatMoney(double amount, char *buffer) {
    long long num = (long long) amount; 
    char temp[30]; 
    int i = 0, count = 0;
    if (num == 0) {
        strcpy(buffer, "0 VND");
        return;
    }
    
    while (num > 0) {
        if (count == 3) {
            temp[i++] = ',';
            count = 0;
        }
        temp[i++] = (num % 10) + '0';
       
        num /= 10;
        count++;
    }
    temp[i] = '\0';

    int len = strlen(temp);
    for (int j = 0; j < len; j++) {
        buffer[j] = temp[len - j - 1];
    }
    buffer[len] = '\0'; 
    strcat(buffer, " VND"); 
}

