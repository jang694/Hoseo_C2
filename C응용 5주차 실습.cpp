#include <stdio.h>
#include <string.h> // strcspn, sprintf 함수 사용을 위해 추가

#define NUM_PRODUCTS 5 // 상품의 총 개수
#define LOW_STOCK_THRESHOLD 3 // 재고 부족 알림 기준

// 상품 데이터 구조체 (이름 필드 추가)
typedef struct {
    int stock;
    int sales;
    char name[51]; // 상품명 저장 공간 (널 문자 포함 51자)
} Product;

// 함수 선언
void inputProductNames(Product products[]);
void restockProducts(Product products[]);
void sellProducts(Product products[]);
void printStatus(const Product products[]);

int main() {
    Product products[NUM_PRODUCTS];
    int menu_choice;
    char line_buffer[100]; // 입력을 받을 문자열 버퍼

    printf("--- 쇼핑몰 재고 관리 프로그램 (Ver. 4) ---\n");

    // 프로그램 시작 시 상품 데이터 초기화
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        sprintf(products[i].name, "상품 %d", i + 1); // "상품 1", "상품 2" ... 기본 이름 설정
        products[i].sales = 0;
    }

    // 초기 재고 설정
    printf("총 %d개 상품의 초기 재고를 순서대로 입력하세요 (예: 10 15 20 25 30): ", NUM_PRODUCTS);
    while (1) {
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d %d %d %d %d",
                &products[0].stock, &products[1].stock, &products[2].stock,
                &products[3].stock, &products[4].stock) == 5) {
                break;
            }
        }
        printf("오류: 5개의 숫자를 공백으로 구분하여 정확히 입력해주세요: ");
    }

    // 메인 루프
    while (1) {
        printf("\n원하는 메뉴를 선택하세요.(1.입고, 2.판매, 3.상품명 입력, 4.상품현황, 5.종료)\n> ");

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &menu_choice) != 1) {
                printf("오류: 메뉴 번호는 숫자여야 합니다.\n");
                continue;
            }
        }
        else { continue; }

        switch (menu_choice) {
        case 1: restockProducts(products); break;
        case 2: sellProducts(products); break;
        case 3: inputProductNames(products); break; // 신규 기능
        case 4: printStatus(products); break;       // 기존 3번 -> 4번
        case 5:                                     // 기존 4번 -> 5번
            printf("프로그램을 종료합니다.\n");
            return 0;
        default:
            printf("잘못된 메뉴 번호입니다. 다시 입력해주세요.\n");
            break;
        }
    }
    return 0;
}

/**
 * @brief [신규] 상품명을 입력받는 함수 (메뉴 3)
 */
void inputProductNames(Product products[]) {
    char name_buffer[100];
    printf("<< 상품명 입력 메뉴 실행 >>\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("> ID %d 상품명 : ", i + 1);
        if (fgets(name_buffer, sizeof(name_buffer), stdin)) {
            // fgets로 읽은 문자열의 끝에 있는 개행문자(\n) 제거
            name_buffer[strcspn(name_buffer, "\n")] = 0;
            // 구조체에 이름 복사
            strcpy(products[i].name, name_buffer);
        }
    }
    printf("상품명 입력이 완료되었습니다.\n");
}

/**
 * @brief 상품 입고 처리 함수 (메뉴 1)
 */
void restockProducts(Product products[]) {
    // 이전 버전과 동일
    int choice;
    char line_buffer[256];

    printf("> (입력)입고수량 입력 : 전체 상품 입고 1, 개별 상품 입고 2를 선택\n> ");
    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &choice) != 1) {
            printf("오류: 1 또는 2를 숫자로 입력해야 합니다.\n");
            return;
        }
    }
    else { return; }

    if (choice == 1) {
        printf("<<< 1을 선택했을 때\n");
        printf("> 전체 상품의 입고수량을 입력하세요 (%d개) : ", NUM_PRODUCTS);

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            int r[NUM_PRODUCTS];
            if (sscanf(line_buffer, "%d %d %d %d %d", &r[0], &r[1], &r[2], &r[3], &r[4]) == NUM_PRODUCTS) {
                for (int i = 0; i < NUM_PRODUCTS; i++) { products[i].stock += r[i]; }
                printf("전체 상품 입고가 완료되었습니다.\n");
            }
            else {
                printf("\n오류: %d개의 숫자를 정확히 입력해야 합니다.\n", NUM_PRODUCTS);
            }
        }
    }
    else if (choice == 2) {
        int product_id, restock_qty;
        printf("<<< 2를 선택했을 때\n");
        printf("> 상품ID (1-%d) : ", NUM_PRODUCTS);
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &product_id) != 1) { printf("오류: 상품 ID는 숫자여야 합니다.\n"); return; }
        }
        else { return; }

        if (product_id < 1 || product_id > NUM_PRODUCTS) { printf("잘못된 상품 ID입니다.\n"); return; }

        printf("> 입고수량 : ");
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &restock_qty) != 1) { printf("오류: 입고 수량은 숫자여야 합니다.\n"); return; }
        }
        else { return; }

        products[product_id - 1].stock += restock_qty;
        printf("ID %d 상품 입고가 완료되었습니다.\n", product_id);
    }
    else {
        printf("잘못된 선택입니다.\n");
    }
}

/**
 * @brief 상품 판매 처리 함수 (메뉴 2)
 */
void sellProducts(Product products[]) {
    // 이전 버전과 동일
    int product_id, sell_qty;
    char line_buffer[100];

    printf("<<< 2. 판매를 선택했을 때\n");
    printf("> 상품ID (1-%d) : ", NUM_PRODUCTS);

    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &product_id) != 1) { printf("오류: 상품 ID는 숫자여야 합니다.\n"); return; }
    }
    else { return; }

    if (product_id < 1 || product_id > NUM_PRODUCTS) { printf("잘못된 상품 ID입니다.\n"); return; }

    printf("> 판매수량 : ");
    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &sell_qty) != 1) { printf("오류: 판매 수량은 숫자여야 합니다.\n"); return; }
    }
    else { return; }

    if (sell_qty > 0 && products[product_id - 1].stock >= sell_qty) {
        products[product_id - 1].stock -= sell_qty;
        products[product_id - 1].sales += sell_qty;
        printf("ID %d 상품 %d개 판매가 완료되었습니다.\n", product_id, sell_qty);
    }
    else if (sell_qty <= 0) {
        printf("판매 수량은 0보다 커야 합니다.\n");
    }
    else {
        printf("재고가 부족합니다. (현재 재고: %d개)\n", products[product_id - 1].stock);
    }
}

/**
 * @brief [개선] 상황 현황 출력 함수 (메뉴 4)
 */
void printStatus(const Product products[]) {
    printf("<< 상품 현황 메뉴 실행 >>\n");

    int total_stock = 0;
    int total_sales = 0;
    int total_available = 0;
    int max_sales_id = 1, min_sales_id = 1;

    for (int i = 0; i < NUM_PRODUCTS; i++) {
        total_stock += products[i].stock;
        total_sales += products[i].sales;
    }
    total_available = total_stock + total_sales;

    int max_sales = products[0].sales;
    int min_sales = products[0].sales;
    for (int i = 1; i < NUM_PRODUCTS; i++) {
        if (products[i].sales > max_sales) {
            max_sales = products[i].sales;
            max_sales_id = i + 1;
        }
        if (products[i].sales < min_sales) {
            min_sales = products[i].sales;
            min_sales_id = i + 1;
        }
    }

    printf("총 재고수량 : %d\n", total_stock);
    printf("총 판매량 : %d (판매율: %.2f%%)\n", total_sales,
        (total_available > 0) ? ((double)total_sales / total_available) * 100.0 : 0.0);
    printf("가장 많이 판매된 상품 : ID %d, 상품명 : %s, 판매량 %d\n", max_sales_id, products[max_sales_id - 1].name, max_sales);
    printf("가장 적게 판매된 상품 : ID %d, 상품명 : %s, 판매량 %d\n", min_sales_id, products[min_sales_id - 1].name, min_sales);

    printf("--- 상품별 재고 현황 ---\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("상품 ID %d : %s - ", i + 1, products[i].name);
        if (products[i].stock == 0) {
            printf("품절(0)\n");
        }
        else if (products[i].stock <= LOW_STOCK_THRESHOLD) {
            printf("재고부족(%d)\n", products[i].stock);
        }
        else {
            printf("재고(%d)\n", products[i].stock);
        }
    }

}
