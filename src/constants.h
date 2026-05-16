#ifndef CONSTANTS_H
#define CONSTANTS_H

/* =========================================================
 * constants.h - Hằng số và định nghĩa dùng chung toàn project
 * ========================================================= */

/* --- Giới hạn hệ thống --- */
#define MAX_CUSTOMERS       10000
#define MAX_REPAIR_ORDERS   50000
#define MAX_SERVICES        200
#define MAX_ITEMS_PER_ORDER 10


/* --- Độ dài chuỗi --- */
#define ID_LEN          10
#define NAME_LEN        100
#define PHONE_LEN       15
#define PLATE_LEN       15
#define CAR_TYPE_LEN    30
#define SYMPTOM_LEN     200
#define SERVICE_NAME_LEN 100

/* --- Trạng thái phiếu sửa --- */
#define STATUS_RECEIVED   0   /* Tiếp nhận  */
#define STATUS_REPAIRING  1   /* Đang sửa   */
#define STATUS_DONE       2   /* Hoàn thành */

/* --- Đường dẫn file dữ liệu --- */
#define FILE_CUSTOMERS    "customers.txt"
#define FILE_ORDERS       "orders.txt"
#define FILE_SERVICES     "services.txt"

/* --- Màu ANSI (tùy chọn) --- */
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"   /* Hoàn thành  */
#define COLOR_YELLOW  "\033[33m"   /* Đang sửa    */
#define COLOR_GRAY    "\033[90m"   /* Tiếp nhận   */
#define COLOR_CYAN    "\033[36m"
#define COLOR_RED     "\033[31m"
#define COLOR_BOLD    "\033[1m"

#endif /* CONSTANTS_H */
