#ifndef REPORT_H
#define REPORT_H
#include "models.h"

/* =========================================================
 * report.h - Khai báo hàm thống kê và xuất hóa đơn
 * (Tính năng nâng cao - có điểm cộng)
 * ========================================================= */

/* --- Thống kê --- */

/*
 * Thống kê doanh thu trong ngày hôm nay.
 * Lọc các phiếu có status == STATUS_DONE và createdDate trong ngày hiện tại.
 * In tổng số phiếu hoàn thành và tổng doanh thu.
 */
void reportDailyRevenue(void);

/*
 * Thống kê dịch vụ bán chạy nhất.
 * Đếm tổng quantity của từng serviceId trên tất cả phiếu.
 * In top 5 (hoặc toàn bộ nếu ít hơn 5) dịch vụ theo thứ tự giảm dần.
 */
void reportTopServices(void);

/* --- Xuất hóa đơn --- */

/*
 * Xuất hóa đơn của một phiếu ra file .txt.
 * Tên file: "invoice_RO000001.txt" (dựa theo orderId).
 * Nội dung: timestamp, thông tin khách, danh sách dịch vụ, tổng tiền.
 * Trả về 1 thành công, 0 thất bại.
 */
int exportInvoice(const char *orderId);

/*
 * Menu thống kê và xuất hóa đơn.
 */
int createInvoice(const char * orderId);

void reportMenu(void);
extern Invoice invoices[MAX_REPAIR_ORDERS];
extern int invoiceCount;
#endif /* REPORT_H */
