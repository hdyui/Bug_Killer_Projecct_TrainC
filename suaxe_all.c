/* =========================================================
 * suaxe_all.c - Quản Lý Tiệm Sửa Xe (bản gộp 1 file)
 * Nhóm: Bug Killer | Mentor: Nguyễn Đức Bảo Lâm
 * CLB F-Code - Trường ĐH FPT TP.HCM
 *
 * Build: gcc -Wall -std=c99 -o suaxe suaxe_all.c
 * Run  : ./suaxe
 * ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


/* =========================================================
 * SECTION 1: HẰNG SỐ & ĐỊNH NGHĨA
 * ========================================================= */

/* --- Giới hạn hệ thống --- */
#define MAX_CUSTOMERS       10000
#define MAX_REPAIR_ORDERS   50000
#define MAX_SERVICES        200
#define MAX_ITEMS_PER_ORDER 10

/* --- Độ dài chuỗi --- */
#define ID_LEN           10
#define NAME_LEN         100
#define PHONE_LEN        15
#define PLATE_LEN        15
#define CAR_TYPE_LEN     30
#define SYMPTOM_LEN      200
#define SERVICE_NAME_LEN 100

/* --- Trạng thái phiếu sửa --- */
#define STATUS_RECEIVED  0   /* Tiếp nhận  */
#define STATUS_REPAIRING 1   /* Đang sửa   */
#define STATUS_DONE      2   /* Hoàn thành */

/* --- Đường dẫn file --- */
#define FILE_CUSTOMERS "customers.txt"
#define FILE_ORDERS    "orders.txt"
#define FILE_SERVICES  "services.txt"

/* --- Màu ANSI --- */
#define COLOR_RESET  "\033[0m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_GRAY   "\033[90m"
#define COLOR_CYAN   "\033[36m"
#define COLOR_RED    "\033[31m"
#define COLOR_BOLD   "\033[1m"

/* =========================================================
 * SECTION 2: STRUCT (MODELS)
 * ========================================================= */

typedef struct {
    char   serviceId[ID_LEN];
    char   serviceName[SERVICE_NAME_LEN];
    int    quantity;
    double unitPrice;
    double subtotal;
} RepairItem;

typedef struct {
    char   serviceId[ID_LEN];
    char   name[SERVICE_NAME_LEN];
    double unitPrice;
    int    isActive;
} Service;

typedef struct {
    char customerId[ID_LEN];
    char fullName[NAME_LEN];
    char phoneNumber[PHONE_LEN];
    char carPlate[PLATE_LEN];
    char carType[CAR_TYPE_LEN];
    int  orderCount;
} Customer;

typedef struct {
    char       orderId[ID_LEN];
    char       customerPhone[PHONE_LEN];
    char       symptom[SYMPTOM_LEN];
    int        status;
    time_t     createdDate;
    time_t     updatedDate;
    RepairItem items[MAX_ITEMS_PER_ORDER];
    int        itemCount;
    double     totalAmount;
} RepairOrder;

/* =========================================================
 * SECTION 3: BIẾN TOÀN CỤC
 * ========================================================= */

Customer   customers[MAX_CUSTOMERS];
int        customerCount = 0;

RepairOrder orders[MAX_REPAIR_ORDERS];
int         orderCount = 0;

Service    services[MAX_SERVICES];
int        serviceCount = 0;

/* =========================================================
 * SECTION 4: KHAI BÁO TRƯỚC (FORWARD DECLARATIONS)
 * ========================================================= */

/* utils */
int    isValidPhone(const char *phone);
int    isValidPlate(const char *plate);
int	   isValidName(const char *name);
int	   isValidType(const char *type);
int    strCmpIgnoreCase(const char *a, const char *b);
void   strTrim(char *str);
void   readLine(char *buffer, int maxLen);
void   generateCustomerId();
void   generateOrderId();
void   generateServiceId();
void   formatDateTime(time_t t, char *buffer);
void   getTodayString(char *buffer);
char * getStatusString(int status);
void   printDivider(void);
void   printHeader(const char *title);
void   printSuccess(const char *msg);
void   printError(const char *msg);
void   printStatus(int status);
void   formatMoney(double amount, char *buffer);
int    validateServices(void);
int    validateCustomers(void);
int    validateOrders(void);

/* fileio */
int    saveCustomers(void);
int    loadCustomers(void);
int    saveOrders(void);
int    loadOrders(void);
int    saveServices(void);
int    loadServices(void);
void   loadAllData(void);
void   saveAllData(void);

/* customer */
void   initCustomers(void);
int    addCustomer(void);
int    editCustomer(void);
int    findCustomerByPhone(const char *phone);
int    findCustomerByPlate(const char *plate);
void   searchCustomerMenu(void);
void   printCustomer(const Customer *c);
void   listAllCustomers(void);

/* service */
void   initServices(void);
int    addService(void);
int    editService(void);
int    findServiceById(const char *serviceId);
void   listAllServices(void);

/* repair */
void   initOrders(void);
int   createRepairOrder(void);
int    addItemToOrder(int orderIdx, int serviceIdx);
int    updateOrderStatus(void);
int    findOrderById(const char *orderId);
int    findOrdersByPhone(const char *phone, int *result, int maxResult);
int    findOrdersByPlate(const char *plate, int *result, int maxResult);
void   printOrder(const RepairOrder *o);
void   listOrders(int statusFilter);
void   viewCustomerHistory(void);
void   searchOrderMenu(void);

/* report */
void   reportDailyRevenue(void);
void   reportTopServices(void);
int    exportInvoice(const char *orderId);
void   reportMenu(void);

/* menus */
static void menuCustomer(void);
static void menuRepair(void);
static void menuService(void);

/* =========================================================
 * SECTION 5: UTILS
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

int strCmpIgnoreCase(const char *a, const char *b) {
    /* TODO:
     * 1. Duyệt từng ký tự của a và b cùng lúc
     * 2. So sánh tolower(a[i]) với tolower(b[i])
     * 3. Nếu khác nhau, trả về hiệu của hai ký tự đó
     * 4. Nếu đến cuối cả hai, trả về 0
     */
    
    // Hàm này để so sánh 2 chuỗi nhưng không phân biệt chữ hoa hay chữ thường
    
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') { 
	// Mình đang muốn so sánh 2 chuỗi với nhau nên nếu 1 trong 2 kết thúc thì việc biến chữ hoa thành chữ thường ở những ký tự sau là không cần thiết
        char ca = tolower((unsigned char)a[i]);
        char cb = tolower((unsigned char)b[i]);
        /*
        - Hai biến ca và cb này mục tiêu là để biến chữ in hoa thành chữ thường và ở đây ta ép kiểu unsigned char là để tránh lỗi khi char âm thì đây
        việc ép kiểu là cách viết an toàn
        */

        if (ca != cb) {
            return ca - cb;  // khác nhau → trả về độ lệch
        }
        /*
        - Mục đích của bước này là để cho biết 2 chuỗi này khác nhau ở đâu và chuỗi đc so sánh theo thứ tự từ trái sang phải thì nếu khác nhau ở ký tự
        đầu tiên luôn thì output luôn kết quả, nếu ko có bước này thì nó sẽ duyệt hết chuỗi luôn 
        */

        i++;
    }

    // nếu 1 trong 2 chuỗi kết thúc trước
    return tolower((unsigned char)a[i]) - tolower((unsigned char)b[i]); // Nếu cả 2 chuỗi có độ dài bằng nhau thì lúc này nó sẽ tương đương return 0
}

void strTrim(char *str) {
    /* TODO:
     * 1. Tìm vị trí ký tự đầu tiên không phải khoảng trắng (isspace)
     * 2. Dịch chuyển chuỗi về đầu nếu có khoảng trắng phía trước
     * 3. Tìm từ cuối về, cắt bỏ khoảng trắng cuối bằng cách đặt '\0'
     */
     
    if (str == NULL) return;

    char *start = str;

    // 1. Bỏ khoảng trắng đầu
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    // 2. Dồn chuỗi về đầu (nếu có khoảng trắng đầu)
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
		/* 
		- memmove là copy từ chỗ A sang chỗ B; và dù A và B có ghi đè lên nhau cũng không làm hỏng dữ liệu
        - memmove sẽ sao chép toàn bộ nội dung chuỗi mà start trỏ tới (bao gồm cả ký tự '\0') vào vùng nhớ bắt đầu tại str 
		(nếu không có + 1 thì sẽ không thể lấy ký tự '\0' điều này gây ra việc in rác khi mà chuỗi ko có ký tự kết thúc chuỗi) 
        - Dùng memmove là vì str và start có thể bị ghi chồng lên vùng nhớ của nhau và memmove đảm bảo copy đúng dữ liệu dù bị chồng lấp*/
    }

    // 3. Xoá khoảng trắng cuối
    char *end = str + strlen(str) - 1;

    while (end >= str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    } 
}

void readLine(char *buffer, int maxLen) {
    /* TODO:
     * 1. Dùng fgets(buffer, maxLen, stdin) để đọc cả dòng
     * 2. Tìm ký tự '\n' cuối và thay bằng '\0'
     * 3. Nếu fgets trả về NULL, gán buffer[0] = '\0'
     * 4. Gọi strTrim để xoá khoảng trắng thừa
     */
    
	// 1. Đọc cả dòng từ stdin
    if (fgets(buffer, maxLen, stdin) == NULL) {
        buffer[0] = '\0'; // nếu lỗi → chuỗi rỗng
        return;
    }

    // 2. Tìm ký tự '\n' và thay bằng '\0'
    buffer[strcspn(buffer, "\n")] = '\0';

    // 3. Xoá khoảng trắng đầu/cuối
    strTrim(buffer);
}

void generateCustomerId() {
    sprintf(customers[customerCount].customerId, "CU%06d", customerCount+1);
}

void generateOrderId() {
    sprintf(orders[orderCount].orderId,"RO%06d", orderCount + 1);
}

void generateServiceId() {
    sprintf(services[serviceCount].serviceId, "SV%06d", serviceCount + 1);
}

void formatDateTime(time_t t, char *buffer) {
    /* TODO:
     * struct tm *tm_info = localtime(&t);
     * strftime(buffer, 20, "%d/%m/%Y %H:%M", tm_info);
     */
    
    /*
	- Kiểu dữ liệu time_t là một dải số thời gian dạng máy tính, thì để con người có thể đọc được thì nó cần phải được chuyển đổi
	- Thì logic xử lý ở đây sẽ phân tích dải số đó ra thành ngày giờ dễ dùng bằng localtime và truyền vào từng field cho struct tm
    */
    struct tm *tm_info = localtime(&t);
    /* 
	- struct tm là một struct chuẩn có sẵn khi include thư viện time.h
	struct tm {
    int tm_sec;   // giây
    int tm_min;   // phút
    int tm_hour;  // giờ
    int tm_mday;  // ngày
    int tm_mon;   // tháng (0-11)
    int tm_year;  // năm - 1900
	};

	- localtime là hàm sẽ nhận vào time_t rồi trả về con trỏ tới struct tm nghĩa là nó sẽ lấy thời gian t, chuyển thành struct tm, 
	rồi lưu địa chỉ vào tm_info. Dữ liệu truyền vào cho hàm localtime là con trỏ bởi vì struct tm khá lớn, tránh việc copy dữ liệu để tối
	ưu hiệu năng
	*/
    strftime(buffer, 20, "%d/%m/%Y %H:%M", tm_info);
    /*
    - Nhiệm vụ của dòng này là format struct tm thành một chuỗi theo format mà mình muốn
    - Tham số của hàm là buffer (nơi chứa cái kết quả), 20 là số ký tự tối đa (tính cả '\0') và cái đống trong ngoặc kép là format mình
    muốn, còn tm_info là mình muốn nó in ra dữ liệu từ tm_info (Ở đây việc chỉ ra từng field trong struct mà tm_info đang trỏ đến là ko cần
    thiết là vì hàm strftime đã xử lý hết cho chúng ta rồi, nó sẽ tự nhìn vào data specifier để tự lấy dữ liệu và format theo cái mà mình
    muốn)
    
    */
}



void getTodayString(char *buffer) {
    /* TODO:
     * time_t now = time(NULL);
     * struct tm *tm_info = localtime(&now);
     * strftime(buffer, 12, "%d/%m/%Y", tm_info);
     */
    time_t now = time(NULL);                 // lấy thời gian hiện tại
    struct tm *tm_info = localtime(&now);    // chuyển sang dạng ngày giờ dễ đọc
    strftime(buffer, 12, "%d/%m/%Y", tm_info); // format thành chuỗi
    /*
    - Dòng đầu của hàm này dùng để lấy thời gian hiện tại
    - Hai dòng còn lại thì có ý nghĩa như đã giải thích trên hàm formatDateTime
    */
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

void printDivider(void) {
    /* TODO: In 60 ký tự '-' rồi xuống dòng */
    for (int i = 0; i < 60; i++) {
        printf("-");
    }
    printf("\n");
}

void printHeader(const char *title) {
    /* TODO:
     * printDivider();
     * printf(COLOR_BOLD COLOR_CYAN "  %s\n" COLOR_RESET, title);
     * printDivider();
     */
    
	printDivider();  // dòng kẻ trên

    printf(COLOR_BOLD COLOR_CYAN "  %s\n" COLOR_RESET, title);
    // in tiêu đề với màu + in đậm

    printDivider();  // dòng kẻ dưới
}

void printSuccess(const char *msg) {
    /* TODO: printf(COLOR_GREEN "[OK] %s\n" COLOR_RESET, msg); */
    
    printf(COLOR_GREEN "[OK] %s\n" COLOR_RESET, msg);
}

void printError(const char *msg) {
    /* TODO: printf(COLOR_RED "[LOI] %s\n" COLOR_RESET, msg); */
    
    printf(COLOR_RED "[LOI] %s\n" COLOR_RESET, msg);
}

void printStatus(int status) {
    /* TODO:
     * switch(status):
     *   0 -> COLOR_GRAY  "Tiep nhan"
     *   1 -> COLOR_YELLOW "Dang sua"
     *   2 -> COLOR_GREEN  "Hoan thanh"
     * Sau đó COLOR_RESET
     */
     
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
    /* TODO:
     * Chuyển amount thành chuỗi có dấu phân cách hàng nghìn + " VND"
     * VD: 1500000 -> "1,500,000 VND"
     */
    long long num = (long long) amount; // Tiền Việt Nam không có phần lẻ nên tạo 1 biến num và ép kiểu amount lên long long để chứa số nguyên dài hơn

    char temp[30]; // Chuỗi temp này để lưu số tiền ngược (cụ thể thì cứ xem ở dưới)
    int i = 0, count = 0;

    if (num == 0) {
        strcpy(buffer, "0 VND");
        return;
    }

    // build ngược
    while (num > 0) {
        if (count == 3) {
            temp[i++] = ',';
            count = 0;
        }
        temp[i++] = (num % 10) + '0';
        /*
        - temp[i++] gồm 2 việc: Đầu tiên là gán vào temp[i] sau đó mới tăng i = i + 1
        - (num % 10) + '0' chia ra 2 phần nhỏ: "(num % 10)" là lấy ký tự cuối cùng của num và phần "+ '0'" là để chuyển số thành ký tự
        - Giải thích hơn về việc chuyển số thành ký tự thì ta cần biết về ASCII:
        	Ví dụ để hiểu rõ hơn: ta đang muốn chuyển số 5 thành ký tự '5':
			+ Ký tự '0' có mã ASCII là 48
			+ Ký tự '5' có mã ASCII là 53
			=> Vậy 5 + '0' nghĩa là 5 + 48 = 53 (Cũng chính là ký tự '5')
		- Vậy chung quy lại dòng này dùng để chuyển từng số của num thành các ký tự số và gán vào từng index của mảng temp và sau khi gán thì sẽ tăng 1 index
        */
        num /= 10;
        count++;
    }
    temp[i] = '\0';
    /*
    - Vòng lặp này chạy khi num > 0 nghĩa là số tiền > 0
    - Sau đó chuyển từng số của num thành các ký tự số và gán vào từng index của mảng temp và sau khi gán thì sẽ tăng 1 index
    - Cập nhật lại số num
    - Tăng biến count
    - Nếu biến count = 3 nghĩa là đã đủ 3 ký tự số đã được đọc thì thêm dấu phẩy vào vị trí tiếp theo và reset biến count (Giải thích trong hàm if)
    
    - Việc lưu như thế này chúng ta có thể hình dung nó sẽ bị ngược so với số tiền thông thường, vậy nên ta phải tạo mảng temp để lưu giá trị ngược này
    và sau đó phải đổi chiều lại
    	Ví dụ để cho thấy điều đó (num = 1000):
    	+ Vòng lặp đầu tiên: temp[0] = '0' -> num = 100 -> count = 1
		+ Vòng lặp T2: temp[1] = '0' -> num = 10 -> count = 2 
		+ Vòng lặp T3: temp[2] = '0' -> num = 1 -> count = 3
		+ Vòng lặp T4: temp[3] = ',' -> num = 1 -> count = 0 -> temp[4] = '1' -> num = 0 -> count = 1
		=> temp[5] = "000,1\0"
    */

    // đảo lại
    int len = strlen(temp); // Lấy độ dài của chuỗi ký tự số của tiền
    for (int j = 0; j < len; j++) {
        buffer[j] = temp[len - j - 1]; // Lấy ký tự cuối của temp và ghi vào đầu buffer
    }
    buffer[len] = '\0'; // Kết thúc chuỗi bằng '\0'
    

    strcat(buffer, " VND"); // Dùng strcat để nối cụm " VND" vào sau số tiền từ buffer
}

int validateServices(void) {
    for (int i = 0; i < serviceCount; i++) {

        // ID không rỗng
        if (strlen(services[i].serviceId) == 0)
            return 0;

        // Name không rỗng
        if (strlen(services[i].name) == 0)
            return 0;

        // Giá phải > 0
        if (services[i].unitPrice <= 0)
            return 0;

        // isActive phải là 0 hoặc 1
        if (services[i].isActive != 0 && services[i].isActive != 1)
            return 0;
    }
    return 1;
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
    }
    return 1;
}

/* =========================================================
 * SECTION 6: FILE I/O
 * ========================================================= */

int saveCustomers(void) {
	if (!validateCustomers()) {
        printError("Du lieu khach hang khong hop le!");
        return 0;
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

    fclose(fp);
    return 1;

}

int saveOrders(void) {
    if (!validateOrders()) {
        printError("Du lieu phieu sua khong hop le!");
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
    return 1;
}

int loadOrders(void) {
    FILE *fp = fopen(FILE_ORDERS, "r");
    if (!fp) return 0;

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

    fclose(fp);
    return 1;
}

int saveServices(void) {

    if (!validateServices()) {
        printError("Du lieu dich vu khong hop le. Khong the luu!");
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
    if (!fp) return 0;

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

    fclose(fp);
    return 1;
}

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
 * SECTION 7: CUSTOMER
 * ========================================================= */

int addCustomer(void) {
    /* Biến tạm để nhập trước, sau khi hợp lệ mới gán vào mảng */
    char tempName[NAME_LEN];
    char tempPhone[PHONE_LEN];
    char tempPlate[PLATE_LEN];
    char tempType[CAR_TYPE_LEN];
    int  foundIdx;
 
    /* Kiểm tra hệ thống còn chỗ không */
    if (customerCount >= MAX_CUSTOMERS) {
        printError("He thong da day, khong the them khach hang moi.");
        return 0;
    }
    /* --- Nhập họ tên --- */
    /* Lặp cho đến khi người dùng nhập tên không rỗng */
    do {
        printf("  Ho va ten: ");
        scanf(" %99[^\n]", tempName);   
        while (getchar() != '\n');      
        if ( !isValidName(tempName) ) {
            printError("Ho ten khong hop le.");
        }
    } while ( !isValidName(tempName) );
 
    /* --- Nhập số điện thoại --- */
    while (1) {
        printf("  So dien thoai: ");
        scanf(" %10s", tempPhone);      
        while (getchar() != '\n');
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
    /* --- Nhập biển số xe --- */
    while (1) {
        printf("  Bien so xe (VD: 51F-123.45): ");
        scanf(" %14s", tempPlate);
        while (getchar() != '\n');
        if (isValidPlate(tempPlate) == 0) {
            printError("Bien so khong hop le.");
            continue;
        }
        break;
    }
    /* --- Nhập loại xe --- */
    do {
        printf("  Loai xe (VD: Xe may, O to, Xe dap dien): ");
        scanf(" %29[^\n]", tempType);
        while (getchar() != '\n');
        if (!isValidName(tempType)) {
            printError("Loai xe khong hop le.");
        }
    } while (!isValidName(tempType));
    /* --- Ghi dữ liệu vào ô mảng tại vị trí customerCount --- */
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

int editCustomer(void) {
	
    char phone[PHONE_LEN];
    int  idx;
    int  choice;
 
    /* Bước 1: nhập SĐT cần sửa */
    do {
    	printf("  Nhap SDT khach hang can sua: ");
    	scanf(" %14s", phone);
    	if( !isValidPhone(phone) ){
    		printError("  SDT khong hop le, xin hay nhap lai!");
		}
	}
    while (!isValidPhone(phone));
 
    /* Bước 2: tìm khách theo SĐT */
    idx = findCustomerByPhone(phone);
    if (idx == -1) {
        printError("Khong tim thay khach hang voi SDT nay.");
        return 0;
    }
 
    /* Bước 3: in thông tin hiện tại để người dùng xem trước khi sửa */
    printCustomer(&customers[idx]);
 
    /* Bước 4: hiển thị menu chọn trường muốn sửa */
    printf("  [1] Sua ho ten\n");
    printf("  [2] Sua bien so xe\n");
    printf("  [3] Sua loai xe\n");
    printf("  [0] Huy\n");
    printf("  Lua chon: ");
    scanf(" %d", &choice);
    while (getchar() != '\n');
 
    /* Bước 5: nhập giá trị mới, validate rồi gán vào customers[idx] */
    if (choice == 1) {
        do {
            printf("  Ho ten moi: ");
            scanf(" %99[^\n]", customers[idx].fullName);
            while (getchar() != '\n');
            if (!isValidName(customers[idx].fullName)) {
                printError("Ho ten khong hop le.");
            }
        } while (!isValidName(customers[idx].fullName));
 
    } else if (choice == 2) {
 
        /* Sửa biển số*/
        char newPlate[PLATE_LEN];
        while (1) {
            printf("  Bien so xe moi: ");
            scanf(" %14s", newPlate);
            while (getchar() != '\n');
            if (isValidPlate(newPlate) == 0) {
                printError("Bien so khong hop le.");
                continue;
            }
            break;
        }
        strcpy(customers[idx].carPlate, newPlate);
 
    } else if (choice == 3) {
 
        /* Sửa loại xe - không được rỗng */
        do {
            printf("  Loai xe moi: ");
            scanf(" %29[^\n]", customers[idx].carType);
            while (getchar() != '\n');
            if (!isValidName(customers[idx].carType) ) {
                printError("Loai xe khong hop le.");
            }
        } while (!isValidName(customers[idx].carType));
 
    } else if (choice == 0) {
        puts("  Da huy thao tac.");
        return 0;
 
    } else {
        printError("Lua chon khong hop le.");
        return 0;
    }
 
    /* Bước 6: lưu file và thông báo thành công */
    saveCustomers();
    printSuccess("Da cap nhat thong tin khach hang.");
    return 1;
}

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
    /* TODO:
     * for (int i = 0; i < customerCount; i++)
     *     if (strCmpIgnoreCase(customers[i].carPlate, plate) == 0) return i;
     * return -1;
     */
    int i;
    for (i = 0; i < customerCount; i++) {
        if (strCmpIgnoreCase(customers[i].carPlate, plate) == 0) {
            return i;
        }
    }
    return -1; 
}

void searchCustomerMenu(void) {
    /* TODO:
     * Hiển thị: [1] Tìm theo SĐT  [2] Tìm theo biển số
     * Nhập từ khoá, gọi hàm tìm tương ứng
     * In printCustomer nếu tìm thấy, printError nếu không
     */
    int choice;
    int idx;
 
    /* Hiển thị lựa chọn tìm kiếm */
    printf("  [1] Tim theo SDT\n");
    printf("  [2] Tim theo bien so xe\n");
    printf("  [0] Quay lai\n");
    printf("  Lua chon: ");
    scanf(" %d", &choice);
    while (getchar() != '\n');
 
    idx = -1;
 
    if (choice == 1) {
    	char phone[PHONE_LEN];
        do {
        	printf("  Nhap SDT: ");
        	scanf(" %11s", phone);
        	if (!isValidPhone(phone)) {
                printError("SDT khong hop le.");
            }
		}
        while(!isValidPhone(phone));
		idx = findCustomerByPhone(phone);
 
    } else if (choice == 2) {
    	char plate[PLATE_LEN];
        do {
			printf("  Nhap bien so xe: ");
        	scanf(" %11s", plate);
        	if (!isValidPlate(plate)) {
                printError("Bien so khong hop le.");
            }
    	}
        while(!isValidPlate(plate));
        idx = findCustomerByPlate(plate);
 
    } else {
        //printError("Lua chon khong hop le.");
        return;
    }
 
    /* In kết quả */
    if (idx == -1) {
        printError("Khong tim thay khach hang.");
    } else {
        printCustomer(&customers[idx]);
    }
}

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
	////////// Mỗi Khách hàng có một sdt và biển số xe duy nhất ( cần cải tiến ) 
    /* Kiểm tra danh sách rỗng */
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
/* =========================================================
 * SECTION 8: SERVICE
 * ========================================================= */

void initServices(void) {
    serviceCount = 0;
    memset(services, 0, sizeof(services));
}

int addService(void) {
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
        printf("  Ten dich vu: ");
        readLine(tempName, SERVICE_NAME_LEN);
        if (tempName[0] == '\0') {
            printError("Ten dich vu khong duoc de trong.");
        }
    } while (tempName[0] == '\0');

    /* 3. Nhập đơn giá (phải > 0) */
    do {
        printf("  Don gia (VND): ");
        if (scanf("%lf", &tempPrice) != 1) {
            while (getchar() != '\n'); /* Xóa buffer nếu nhập sai kiểu (chữ) */
            printError("Don gia phai la mot so.");
            tempPrice = -1;
        } else {
            while (getchar() != '\n'); /* Xóa buffer sau khi lấy số */
            if (tempPrice <= 0) {
                printError("Don gia phai lon hon 0.");
            }
        }
    } while (tempPrice <= 0);

    /* 4. Khởi tạo mã DV và đưa vào mảng */
    strcpy(services[serviceCount].name, tempName);
    services[serviceCount].unitPrice = tempPrice;
    services[serviceCount].isActive = 1;
    sprintf(services[serviceCount].serviceId, "SV%06d", serviceCount + 1);

    /* 5. Tăng biến đếm và lưu file */
    serviceCount++;
    saveServices();
    
    printSuccess("Da them dich vu thanh cong!");
    printf("  Ma DV duoc cap: %s\n", services[serviceCount - 1].serviceId);
        
    return 1; 
}

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
    printf("  Nhap ma dich vu can sua (VD: SV000001): ");
    readLine(id, ID_LEN);

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
            printf("  Ten dich vu moi: ");
            readLine(newName, SERVICE_NAME_LEN);
            if (newName[0] == '\0') {
                printError("Ten dich vu khong duoc de trong.");
            }
        } while (newName[0] == '\0');
        strcpy(services[idx].name, newName);

    } else if (choice == 2) {
        double newPrice;
        do {
            printf("  Don gia moi (VND): ");
            if (scanf("%lf", &newPrice) != 1) {
                while (getchar() != '\n');
                printError("Don gia phai la mot so.");
                newPrice = -1;
            } else {
                while (getchar() != '\n');
                if (newPrice <= 0) {
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

/* =========================================================
 * SECTION 9: REPAIR ORDER
 * ========================================================= */

int createRepairOrder(void) {
    int status = 1;
    int index; //vị trí khách hàng trong mảng 
    char phoneNumber[PHONE_LEN];
    int isConfirm;
    if(orderCount >= MAX_REPAIR_ORDERS){
        printf("So luong vuot muc toi da\n");
        status = 0;
        return status;
    }
   
    do{
        printf("Nhap so dien thoai khach hang: ");
        scanf("%[^\n]", phoneNumber);
        
        printf("Xac nhan so dien thoai ban muon tim: %s\n", phoneNumber);
        printf("[1] Xac nhan\n");
        printf("[0] Nhap lai\n");
        printf("Nhap lua chon: ");
        scanf("%d", &isConfirm);
        while (getchar() != '\n');
        if(findCustomerByPhone(phoneNumber) == -1){
            printf("Khong ton tai so dien nay nay: %s\n", phoneNumber);
        }
        else{
            printf("%-20s %-20s %-20s %-20s %-20s %-20s\n",
            "ID", "Name", "Phone", "Car plate", "Car type", "Order count");
            index = findCustomerByPhone(phoneNumber);
            printf("%-20s %-20s %-20s %-20s %-20s %-20d\n",
                customers[index].customerId, customers[index].fullName,
                customers[index].phoneNumber, customers[index].carPlate,
                customers[index].carType, customers[index].orderCount);
        }
        if(isConfirm != 1){
                printf("Vui long nhap [1] hoac [0]\n");
        }
    }
    while(isConfirm != 1 || findCustomerByPhone(phoneNumber) == -1);
    generateOrderId();
    strcpy(orders[orderCount].customerPhone, customers[index].phoneNumber);
    char symptom[SYMPTOM_LEN];
    do{
        printf("Nhap tinh trang xe cua ban: ");
        scanf("%[^\n]", symptom);
        while (getchar() != '\n');
    }
    while(strlen(symptom) == 0);
    strcpy(orders[orderCount].symptom, symptom);
    orders[orderCount].status = STATUS_RECEIVED;
    orders[orderCount].createdDate = time(NULL);
    orders[orderCount].itemCount = 0;
    orders[orderCount].totalAmount = 0;


    listAllServices();
    char serviceId[ID_LEN];
    do{
        printf("Nhap ma dich vu can them vao phieu (VD: SV000001), nhap 0 de ket thuc: ");
        scanf("%[^\n]", serviceId);
        while (getchar() != '\n');
        if(strcmp(serviceId, "0") == 0){
            break;
        }
        int serviceIdx = findServiceById(serviceId);
        if(serviceIdx == -1 || services[serviceIdx].isActive == 0){
            printf("Khong tim thay dich vu hoac dich vu khong hoat dong.\n");
            continue;
        }
        addItemToOrder(orderCount, serviceIdx);
        
    }
    while(1);
        
    orderCount++;
    customers[index].orderCount++;


    
    saveOrders();
    saveCustomers();
    printf("Tao phieu thanh cong\n");
    return status;
}

int addItemToOrder(int orderIdx, int serviceIdx) {
    /* TODO:
    
     * 5.   ms[itemCount++]
     * 6. orders[idx].totalAmount += subtotal
     * 7. return 1
     */
    int status = 1; 
    if(orders[orderIdx].itemCount >= MAX_ITEMS_PER_ORDER){
        printf("So luong dich vu vuot muc toi da\n");
        status = 0;
        return status;
    }   
    int quantity;
    do{
        printf("Nhap so luong: ");
        scanf("%d", &quantity);
        while(getchar () != '\n');
    }
    while(quantity <= 0);
    int itemIdx = orders[orderIdx].itemCount;

    strcpy(orders[orderIdx].items[itemIdx].serviceId,
                    services[serviceIdx].serviceId);

    strcpy(orders[orderIdx].items[itemIdx].serviceName,
            services[serviceIdx].name);

    orders[orderIdx].items[itemIdx].unitPrice =
           services[serviceIdx].unitPrice;

    orders[orderIdx].items[itemIdx].quantity = quantity;

    orders[orderIdx].items[itemIdx].subtotal = 
            quantity * services[serviceIdx].unitPrice;
    
    orders[orderIdx].totalAmount += orders[orderIdx].items[itemIdx].subtotal;


    orders[orderIdx].itemCount++;
    return status;
}

int updateOrderStatus(void) {
    /* TODO:
     * 1. Nhập orderId
     * 2. findOrderById() -> nếu -1 báo lỗi return 0
     * 3. Nếu status == STATUS_DONE -> cảnh báo "Phieu da hoan thanh", return 0
     * 4. In trạng thái hiện tại và kế tiếp, hỏi xác nhận
     * 5. status++; updatedDate = time(NULL)
     * 6. saveOrders(); printSuccess(); return 1
     */
    char orderId[ID_LEN];
    
    printf("Nhap ma phieu: ");
    readLine(orderId, ID_LEN);

    int idx = findOrderById(orderId);
    if (idx == -1) {
        printError("Khong tim thay phieu!");
        return 0;
    }

    RepairOrder *o = &orders[idx];

    // Nếu đã hoàn thành thì chặn
    if (o->status == STATUS_DONE) {
        printError("Phieu da hoan thanh, khong the cap nhat.");
        return 0;
    }

    // Hiển thị trạng thái hiện tại
    printf("Trang thai hien tai: ");
    printStatus(o->status);
    printf("\n");

    // Hiển thị trạng thái tiếp theo
    printf("Trang thai ke tiep: ");
    printStatus(o->status + 1);
    printf("\n");

    // Xác nhận
    int confirm;
    do {
        printf("Ban co chac chan cap nhat? [1: Yes | 0: No]: ");
        scanf("%d", &confirm);
        while (getchar() != '\n');

        if (confirm != 0 && confirm != 1) {
            printError("Nhap 0 hoac 1.");
        }
    } while (confirm != 0 && confirm != 1);

    if (confirm == 0) {
        printf("Da huy cap nhat.\n");
        return 0;
    }

    // Update trạng thái
    o->status++;
    o->updatedDate = time(NULL);

    // Lưu file
    if (!saveOrders()) {
        printError("Loi khi luu du lieu.");
        return 0;
    }

    printSuccess("Cap nhat trang thai thanh cong.");
    return 1;
}


int findOrderById(const char *orderId) {
    int left = 0, right = orderCount - 1;
    while(left <= right){
        int mid = left + (right - left) / 2;
        if(strcmp(orderId, orders[mid].orderId) < 0){
            right = mid - 1;
        }
        else if(strcmp(orderId, orders[mid].orderId) > 0){
            left = mid + 1;
        }
        else{
            return mid;
        }
    }
    return -1;

}

int findOrdersByPhone(const char *phone, int *result, int maxResult) {
    int count = 0;

    for (int i = 0; i < orderCount; i++) {
        if (count >= maxResult){
            break; 
        }

        if (strcmp(orders[i].customerPhone, phone) == 0) {
            result[count] = i; 
            count++;          
        }
    }
    return count; 
}
int findOrdersByPlate(const char *plate, int *result, int maxResult) {
    
    int cIdx = findCustomerByPlate(plate);
    if(cIdx == -1) {
        return 0;
    }

    return findOrdersByPhone(customers[cIdx].phoneNumber, result, maxResult); /* placeholder */
}

void printOrder(const RepairOrder *o) {
    printDivider();

    printf("  CHI TIET PHIEU SUA CHUA\n");
    printDivider();

    printf("  Ma phieu   : %s\n", o->orderId);
    printf("  SDT KH     : %s\n", o->customerPhone);
    printf("  Trieu chung: %s\n", o->symptom);

    char dtBuf[20]; // dtBuf (Date time buffer): là cái mảng chứa chuỗi kết quả sau khi format thời gian 
    formatDateTime(o->createdDate, dtBuf); // Lấy thời gian dạng số (time_t) → convert → ghi vào dtBuf
    printf("  Ngay tao   : %s\n", dtBuf); // In ra thời gian

    printf("  Trang thai : ");
    printStatus(o->status);
    printf("\n");

    // thông tin khách
    int cIdx = findCustomerByPhone(o->customerPhone);
    // cIdx (Customer index): Vị trí của khách hàng trong mảng, giá trị của biến này được trả về khi gọi hàm findCustomerByPhone
    if (cIdx != -1) {
        printDivider();
        printf("  THONG TIN KHACH HANG\n");
        printCustomer(&customers[cIdx]);
    }

    printDivider();

    // bảng dịch vụ
    printf("  DANH SACH DICH VU\n");
    printDivider();

    printf("  %-4s %-25s %-5s %-15s %-15s\n", "STT", "Ten DV", "SL", "Don gia", "Thanh tien");
    printDivider();

    for (int i = 0; i < o->itemCount; i++) { // Vòng lặp này dùng để duyệt từng dịch vụ trong phiếu
        char priceBuf[30], subBuf[30]; // priceBuf để lưu giá tiền 1 lần làm dịch vụ và subBuf để lưu tổng giá tiền của 1 dịch vụ

        formatMoney(o->items[i].unitPrice, priceBuf);
        formatMoney(o->items[i].subtotal, subBuf);

        printf("  %-4d %-25s %-5d %-15s %-15s\n",
               i + 1, // i + 1 là số thứ tự
               o->items[i].serviceName, // Tên dịch vụ
               o->items[i].quantity, // Số lượng lần làm cái dịch vụ đó
               priceBuf, // Số tiền 1 lần làm dịch vụ đó
               subBuf); // Tổng số tiền làm dịch vụ đó (Ví dụ như thay bánh xe thì thay 2 bánh thì tổng số tiền sẽ là 2 * số tiền thay 1 bánh
    }

    printDivider();

    // tổng tiền
    char totalBuf[30];
    formatMoney(o->totalAmount, totalBuf);
    printf("  TONG TIEN: %s\n", totalBuf);
    printDivider();
}

void listOrders(int statusFilter) {
    printDivider();
    printf("  %-4s %-10s %-15s %-20s %-15s %s\n",
           "STT", "Ma phieu", "SDT KH", "Ngay tao", "Trang thai", "Tong tien");
    printDivider();
    for(int i = 0; i < orderCount; i ++){
        if(statusFilter == -1 || statusFilter == orders[i].status){
            char dateFormatted[20];
            char moneyFormatted[30];
            formatDateTime(orders[i].createdDate, dateFormatted);
            formatMoney(orders[i].totalAmount, moneyFormatted);
            printf("  %-4d %-10s %-15s %-20s ",
       		i + 1,
       		orders[i].orderId,
       		orders[i].customerPhone,
       		dateFormatted);

			// In status
			printStatus(orders[i].status);

			// Tính độ dài text KHÔNG có màu
			int len = strlen(getStatusString(orders[i].status));

			// padding cho đủ 15 ký tự
			for (int k = 0; k < (15 - len); k++) {
    		printf(" ");
			}

// In tổng tiền
printf(" %s\n", moneyFormatted);
        }
    }
}

void viewCustomerHistory(void) {
   
    char phone[PHONE_LEN];
    printf("Nhap so dien thoai khach hang: ");
    scanf("%[^\n]", phone);
    int index_array[MAX_REPAIR_ORDERS];
    int n = findOrdersByPhone(phone, index_array, MAX_REPAIR_ORDERS);
    if (n == 0) {
        printf("Khach hang chua co lich su sua chua.\n");
    }
    else{
        for(int i = 0; i < n; i++){
            printOrder(&orders[index_array[i]]);
        }
    }
    
}

void searchOrderMenu(void) {
    
    int choice;
    do{
        printf("Lua chon tim kiem:\n");
        printf("[1] Tim theo ma phieu\n");
        printf("[2] Tim theo bien so xe\n");
        printDivider();
        printf("Nhap lua chon: ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        if(choice != 1 && choice != 2){
            printf("Vui long nhap [1] hoac [2]\n");
        }
        
    }
    while(choice != 1 && choice != 2);
    if(choice == 1){
        char OrderId[ID_LEN];
        printf("Nhap ma phieu: ");
        scanf("%[^\n]", OrderId);
        int idx = findOrderById(OrderId); 
        if(idx = -1){
            printf("Khong tim thay phieu\n");
        }
        else{
            printOrder(&orders[idx]);
        }
    }
    else if(choice == 2){
        char plate[PLATE_LEN];
        printf("Nhap bien so xe: ");
        scanf("%[^\n]", plate);
        int idxArr[MAX_REPAIR_ORDERS];
        int n = findOrdersByPlate(plate, idxArr, MAX_REPAIR_ORDERS);
        if(n == 0){
            printf("Khong tim thay phieu nao voi bien so xe nay\n");
        }
        else{
            for(int i = 0; i < n; i++){
                printOrder(&orders[idxArr[i]]);
            }
        }
    }
}

/* =========================================================
 * SECTION 10: REPORT (NÂNG CAO)
 * ========================================================= */

void reportDailyRevenue(void) {
    /* TODO:
     * 1. Lấy ngày hôm nay qua localtime(time(NULL))
     * 2. Duyệt orders[], chỉ xét STATUS_DONE và createdDate trùng ngày hôm nay
     * 3. Cộng totalAmount, đếm số phiếu
     * 4. In kết quả
     */
}

void reportTopServices(void) {
    /* TODO:
     * 1. Tạo mảng đếm theo serviceId
     * 2. Duyệt tất cả orders[].items[], cộng quantity vào bộ đếm tương ứng
     * 3. Sắp xếp giảm dần
     * 4. In top 5
     */

        
        // 1. Tạo mảng lưu danh sách dịch vụ và mảng đếm song song
        Service localServices[MAX_SERVICES]; // Mảng chứa các dịch vụ duy nhất
        int quantities[MAX_SERVICES] = {0};  // Mảng đếm số lượng tương ứng với từng dịch vụ
        int uniqueCount = 0;        // Số lượng dịch vụ khác nhau tìm thấy

        // 2. Duyệt tất cả orders[].items[], cộng quantity vào bộ đếm tương ứng
        for (int i = 0; i < orderCount; i++) {
            for (int j = 0; j < orders[i].itemCount; j++) {
                RepairItem *item = &orders[i].items[j];
                int found = 0;

                // Kiểm tra dịch vụ đã có trong mảng localServices chưa
                for (int k = 0; k < uniqueCount; k++) {
                    if (strcmp(localServices[k].serviceId, item->serviceId) == 0) {
                        quantities[k] += item->quantity; // Cộng dồn số lượng
                        found = 1;
                        break;
                    }
                }

                // Nếu chưa có, thêm mới vào cả 2 mảng
                if (!found) {
                    strcpy(localServices[uniqueCount].serviceId, item->serviceId);
                    strcpy(localServices[uniqueCount].name, item->serviceName);
                    quantities[uniqueCount] = item->quantity;
                    uniqueCount++;
                }
            }
        }

        // 3. Sắp xếp giảm dần (Bubble Sort trên cả 2 mảng song song)
        for (int i = 0; i < uniqueCount - 1; i++) {
            for (int j = i + 1; j < uniqueCount; j++) {
                if (quantities[i] < quantities[j]) {
                    // Hoán vị mảng số lượng
                    int tempQty = quantities[i];
                    quantities[i] = quantities[j];
                    quantities[j] = tempQty;

                    // Hoán vị mảng dịch vụ tương ứng
                    Service tempSvc = localServices[i];
                    localServices[i] = localServices[j];
                    localServices[j] = tempSvc;
                }
            }
        }

        // 4. In top 5
        printf("\n=================================================================\n");
        printf("                TOP 5 DICH VU DUOC SU DUNG NHIEU NHAT            \n");
        printf("=================================================================\n");
        printf("%-12s | %-35s | %-10s\n", "Ma DV", "Ten dich vu", "Tong SL");
        printf("-----------------------------------------------------------------\n");

        int printCount = (uniqueCount < 5) ? uniqueCount : 5;
        
        if (printCount == 0) {
            printf("Chua co du lieu dich vu!\n");
        } else {
            for (int i = 0; i < printCount; i++) {
                // Sử dụng trường .name từ struct Service của bạn
                printf("%-12s | %-35s | %d\n", 
                    localServices[i].serviceId, 
                    localServices[i].name, 
                    quantities[i]);
            }
        }
        printf("=================================================================\n\n");
    
}

int exportInvoice(const char *orderId) {
    /* 1. Tìm order. Nếu -1 báo lỗi return 0 */
    int orderIndex = findOrderById(orderId);
    if (orderIndex == -1) {
        printf("Loi: Khong tim thay don hang mang ma %s!\n", orderId);
        return 0;
    }

    // Lấy con trỏ trỏ đến đơn hàng để thao tác cho gọn
    RepairOrder *o = &orders[orderIndex];

    /* 2. Tạo tên file "invoice_XXXXXXX.txt" */
    char filename[100];
    sprintf(filename, "invoice_%s.txt", orderId);

    /* 3. Mở file để ghi (mode "w") */
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Loi: Khong the tao file %s de ghi hoa don!\n", filename);
        return 0;
    }

    /* 4. Ghi thông tin hóa đơn đầy đủ vào file */
    fprintf(fp, "========================================================\n");
    fprintf(fp, "                    HOA DON DICH VU                     \n");
    fprintf(fp, "========================================================\n");
    fprintf(fp, "Ma don hang   : %s\n", o->orderId);
    fprintf(fp, "So dien thoai : %s\n", o->customerPhone);
    fprintf(fp, "Yeu cau/Loi   : %s\n", o->symptom);
    fprintf(fp, "--------------------------------------------------------\n");
    
    // Header bảng dịch vụ
    fprintf(fp, "%-4s | %-25s | %-4s | %-12s\n", "STT", "Ten dich vu", "SL", "Thanh tien");
    fprintf(fp, "--------------------------------------------------------\n");

    // Duyệt và in từng dịch vụ trong mảng items
    for (int i = 0; i < o->itemCount; i++) {
        RepairItem *it = &o->items[i];
        fprintf(fp, "%-4d | %-25s | %-4d | %-12.0lf\n", 
                i + 1, 
                it->serviceName, 
                it->quantity, 
                it->subtotal);
    }
    
    fprintf(fp, "--------------------------------------------------------\n");
    // Dùng %.0lf để in tiền VNĐ không lấy số thập phân cho đẹp
    fprintf(fp, "TONG CONG: %.0lf VND\n", o->totalAmount); 
    fprintf(fp, "========================================================\n");
    fprintf(fp, "             XIN CAM ON VA HEN GAP LAI!                 \n");

    /* 5. Đóng file và báo thành công */
    fclose(fp);
    
    // Nếu bạn có viết sẵn hàm printSuccess() thì có thể thay thế dòng printf này
    printf("Da xuat hoa don: %s\n", filename); 
    
    return 1;
}

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
                printf("  Nhap ma phieu: ");
                readLine(oid, ID_LEN);
                exportInvoice(oid);
                break;
            }
            case 0: break;
            default: printError("Lua chon khong hop le.");
        }
    } while (choice != 0);
}

/* =========================================================
 * SECTION 11: MENU CON
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
                printf("  Nhap ma phieu: ");
                readLine(oid, ID_LEN);
                int idx = findOrderById(oid);
                if (idx == -1) printError("Khong tim thay phieu.");
                else printOrder(&orders[idx]);
                break;
            }
            case 4: 
                listOrders(-1);        
                break;
            case 5: {
                int st;
                printf("  Trang thai [0=Tiep nhan, 1=Dang sua, 2=Hoan thanh]: ");
                scanf(" %d", &st);
                while (getchar() != '\n');
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

/* =========================================================
 * SECTION 12: MAIN
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
