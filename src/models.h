#ifndef MODELS_H
#define MODELS_H

/* =========================================================
 * models.h - Định nghĩa các struct dữ liệu trung tâm
 * ========================================================= */

#include <time.h>
#include "constants.h"

/* ---------------------------------------------------------
 * RepairItem - Một dòng dịch vụ trong phiếu sửa
 * --------------------------------------------------------- */
typedef struct {
    char serviceId[ID_LEN];          /* Mã dịch vụ - tham chiếu sang Service */
    char serviceName[SERVICE_NAME_LEN]; /* Tên dịch vụ (snapshot tại thời điểm tạo phiếu) */
    int  quantity;                   /* Số lượng */
    double unitPrice;                /* Đơn giá (snapshot) */
    double subtotal;                 /* = quantity * unitPrice */
} RepairItem;

/* ---------------------------------------------------------
 * Service - Một dịch vụ trong danh mục
 * --------------------------------------------------------- */
typedef struct {
    char   serviceId[ID_LEN];        /* Mã dịch vụ tự sinh - VD: SV000001 */
    char   name[SERVICE_NAME_LEN];   /* Tên dịch vụ */
    double unitPrice;                /* Đơn giá hiện tại */
    int    isActive;                 /* 1 = đang dùng, 0 = đã xoá mềm */
} Service;

/* ---------------------------------------------------------
 * Customer - Thông tin khách hàng
 * --------------------------------------------------------- */
typedef struct {
    char customerId[ID_LEN];         /* Mã KH tự sinh - VD: CU000001 */
    char fullName[NAME_LEN];         /* Họ và tên đầy đủ */
    char phoneNumber[PHONE_LEN];     /* SĐT - định danh duy nhất */
    char carPlate[PLATE_LEN];        /* Biển số xe - VD: 51F-123.45 */
    char carType[CAR_TYPE_LEN];      /* Loại xe - VD: Xe máy, Ô tô */
    int  orderCount;                 /* Tổng số phiếu đã tạo */
} Customer;

/* ---------------------------------------------------------
 * RepairOrder - Phiếu sửa chữa
 * --------------------------------------------------------- */
typedef struct {
    char       orderId[ID_LEN];              /* Mã phiếu tự sinh - VD: RO000001 */
    char       customerPhone[PHONE_LEN];     /* SĐT KH - tham chiếu sang Customer */
    char       symptom[SYMPTOM_LEN];         /* Mô tả triệu chứng */
    int        status;                       /* STATUS_RECEIVED / REPAIRING / DONE */
    time_t     createdDate;                  /* Ngày tạo phiếu */
    time_t     updatedDate;                  /* Lần cập nhật cuối */
    RepairItem items[MAX_ITEMS_PER_ORDER];   /* Danh sách dịch vụ trong phiếu */
    int        itemCount;                    /* Số dịch vụ hiện có */
    double     totalAmount;                  /* Tổng tiền (tự tính) */
} RepairOrder;

typedef struct{
    char orderId[ID_LEN];
    char customerName[NAME_LEN];
    char customerPhone[PHONE_LEN];
    char carPlate[PLATE_LEN];
    char carType[CAR_TYPE_LEN];
    char symptom[SYMPTOM_LEN];
    time_t createdDate;
    RepairItem items[MAX_ITEMS_PER_ORDER];
    int itemCount;
    double totalAmount;
} Invoice;
#endif /* MODELS_H */
