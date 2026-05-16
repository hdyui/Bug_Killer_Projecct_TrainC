#ifndef REPAIR_H
#define REPAIR_H

/* =========================================================
 * repair.h - Khai báo hàm quản lý phiếu sửa chữa
 * ========================================================= */

#include "models.h"

/* --- Bộ nhớ toàn cục --- */
extern RepairOrder orders[MAX_REPAIR_ORDERS];
extern int         orderCount;

/* --- Khởi tạo --- */
void initOrders(void);

/* --- Tạo phiếu --- */

/*
 * Tạo phiếu sửa chữa mới.
 *   - Nhập SĐT khách; kiểm tra khách tồn tại.
 *   - Nhập triệu chứng.
 *   - Vòng lặp thêm dịch vụ từ danh mục (tối đa MAX_ITEMS_PER_ORDER).
 *   - Tự tính totalAmount.
 *   - Sinh orderId, status = STATUS_RECEIVED, createdDate = now.
 *   - Tăng orderCount của khách tương ứng.
 *   - Gọi saveOrders() và saveCustomers().
 * Trả về 1 thành công, 0 thất bại.
 */
int createRepairOrder(void);

/*
 * Thêm một RepairItem vào phiếu order tại index idx.
 * Hiển thị danh mục dịch vụ, nhập serviceId và số lượng.
 * Cập nhật totalAmount của phiếu.
 * Trả về 1 thành công, 0 thất bại (phiếu đã đầy hoặc dịch vụ không tồn tại).
 */
int addItemToOrder(int orderIdx, int serviceIdx);

/* --- Cập nhật trạng thái --- */

/*
 * Cập nhật trạng thái phiếu sửa (chỉ cho phép tiến tới, không đảo ngược).
 * Tìm phiếu theo orderId, kiểm tra trạng thái hiện tại rồi chuyển sang tiếp theo.
 * Cảnh báo nếu cố đảo ngược.
 * Gọi saveOrders() sau khi cập nhật.
 * Trả về 1 thành công, 0 thất bại.
 */
int updateOrderStatus(void);

/* --- Tìm kiếm --- */

/*
 * Tìm index của phiếu theo orderId.
 * Trả về index hoặc -1 nếu không tìm thấy.
 */
int findOrderById(const char *orderId);

/*
 * Tìm tất cả phiếu của một khách theo phoneNumber.
 * Ghi các index tìm được vào mảng result[], trả về số lượng.
 */
int findOrdersByPhone(const char *phone, int *result, int maxResult);

/*
 * Tìm tất cả phiếu theo biển số xe (so khớp với thông tin khách hàng).
 * Ghi các index vào result[], trả về số lượng.
 */
int findOrdersByPlate(const char *plate, int *result, int maxResult);

/* --- Hiển thị --- */

/*
 * In chi tiết một phiếu sửa (thông tin khách, xe, từng dịch vụ, tổng tiền, trạng thái).
 */
void printOrder(const RepairOrder *o);

/*
 * In danh sách phiếu dạng bảng tóm tắt.
 * Nếu statusFilter == -1 thì in tất cả; nếu là 0/1/2 thì lọc theo trạng thái đó.
 */
void listOrders(int statusFilter);

/*
 * Hiển thị lịch sử phiếu của một khách (nhập SĐT rồi in danh sách).
 */
void viewCustomerHistory(void);

/*
 * Hỏi người dùng nhập mã phiếu hoặc biển số để tìm kiếm phiếu.
 */
void searchOrderMenu(void);


int createInvoice(const char * orderId);
#endif /* REPAIR_H */
