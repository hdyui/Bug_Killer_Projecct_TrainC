#ifndef FILEIO_H
#define FILEIO_H

/* =========================================================
 * fileio.h - Khai báo hàm đọc/ghi dữ liệu ra file
 * ========================================================= */

/* --- Khách hàng --- */

/*
 * Ghi toàn bộ mảng customers[] vào FILE_CUSTOMERS bằng fwrite.
 * Format: [int count][Customer * count]
 * Trả về 1 thành công, 0 thất bại.
 */
int saveCustomers(void);

/*
 * Đọc dữ liệu từ FILE_CUSTOMERS vào mảng customers[].
 * Nếu file không tồn tại, bỏ qua (customerCount giữ nguyên 0).
 * Trả về 1 thành công, 0 thất bại.
 */
int loadCustomers(void);

/* --- Phiếu sửa --- */

/*
 * Ghi toàn bộ mảng orders[] vào FILE_ORDERS.
 * Format: [int count][RepairOrder * count]
 * Trả về 1 thành công, 0 thất bại.
 */
int    validateOrders(void);

void   debugValidateOrders(void);
int saveOrders(void);
int    validateServices(void);
void   debugValidateServices(void);

/*
 * Đọc dữ liệu từ FILE_ORDERS vào mảng orders[].
 * Trả về 1 thành công, 0 thất bại.
 */
int loadOrders(void);

/* --- Danh mục dịch vụ --- */

/*
 * Ghi toàn bộ mảng services[] vào FILE_SERVICES.
 * Trả về 1 thành công, 0 thất bại.
 */
int saveServices(void);

/*
 * Đọc dữ liệu từ FILE_SERVICES vào mảng services[].
 * Trả về 1 thành công, 0 thất bại.
 */
int loadServices(void);

/* --- Tiện ích --- */

/*
 * Đảm bảo thư mục "data/" tồn tại.
 * Tạo thư mục nếu chưa có (dùng mkdir từ sys/stat.h).
 */
void ensureDataDir(void);

/*
 * Tải toàn bộ dữ liệu khi khởi động (gọi load cho cả 3 loại).
 */
void loadAllData(void);

/*
 * Lưu toàn bộ dữ liệu (gọi save cho cả 3 loại).
 */
void saveAllData(void);

int validateCustomers(void);

void debugValidateCustomers(void);

#endif /* FILEIO_H */
