#include <stdio.h>
#include <string.h> // 문자열 처리를 위해 추가

#define NUM_PRODUCTS 5 // 상품의 총 개수

// 상품 데이터 구조체
typedef struct {
    int stock;
    int sales;
} Product;

// 함수 선언
// 이제 clearInputBuffer는 필요 없으므로 제거합니다.
void restockProducts(Product products[]);
void sellProducts(Product products[]);
void printStatus(const Product products[]);

int main() {
    Product products[NUM_PRODUCTS];
    int menu_choice;
    char line_buffer[100]; // 입력을 받을 문자열 버퍼

    printf("--- 쇼핑몰 재고 관리 프로그램 (최종 안정화 Ver.) ---\n");

    // 초기 재고 설정
    printf("총 %d개 상품의 초기 재고를 순서대로 입력하세요 (예: 10 15 20 25 30): ", NUM_PRODUCTS);
    while (1) {
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            // sscanf를 사용해 문자열에서 5개의 숫자를 읽어옵니다.
            if (sscanf(line_buffer, "%d %d %d %d %d",
                &products[0].stock, &products[1].stock, &products[2].stock,
                &products[3].stock, &products[4].stock) == 5) {
                break; // 5개 숫자를 성공적으로 읽으면 루프 탈출
            }
        }
        printf("오류: 5개의 숫자를 공백으로 구분하여 정확히 입력해주세요: ");
    }

    for (int i = 0; i < NUM_PRODUCTS; i++) {
        products[i].sales = 0;
    }

    // 메인 루프
    while (1) {
        printf("\n원하는 메뉴를 선택하세요.(1. 입고, 2. 판매, 3. 상황현황, 4. 종료)\n> ");

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &menu_choice) != 1) {
                printf("오류: 메뉴 번호는 숫자여야 합니다.\n");
                continue;
            }
        }
        else { continue; } // 입력이 없으면 계속

        switch (menu_choice) {
        case 1: restockProducts(products); break;
        case 2: sellProducts(products); break;
        case 3: printStatus(products); break;
        case 4:
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
 * @brief 상품 입고 처리 함수 (fgets/sscanf 방식으로 전면 수정)
 */
void restockProducts(Product products[]) {
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

    if (choice == 1) { // 전체 상품 입고
        printf("<<< 1을 선택했을 때\n");
        printf("> 전체 상품의 입고수량을 입력하세요 (%d개) : ", NUM_PRODUCTS);

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            int r[NUM_PRODUCTS];
            if (sscanf(line_buffer, "%d %d %d %d %d", &r[0], &r[1], &r[2], &r[3], &r[4]) == NUM_PRODUCTS) {
                for (int i = 0; i < NUM_PRODUCTS; i++) {
                    products[i].stock += r[i];
                }
                printf("전체 상품 입고가 완료되었습니다.\n");
            }
            else {
                printf("\n오류: %d개의 숫자를 정확히 입력해야 합니다. (예: 10 20 30 40 50)\n", NUM_PRODUCTS);
            }
        }

    }
    else if (choice == 2) { // 개별 상품 입고
        int product_id, restock_qty;
        printf("<<< 2를 선택했을 때\n");

        printf("> 상품ID (1-%d) : ", NUM_PRODUCTS);
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &product_id) != 1) {
                printf("오류: 상품 ID는 숫자여야 합니다.\n");
                return;
            }
        }
        else { return; }

        if (product_id < 1 || product_id > NUM_PRODUCTS) {
            printf("잘못된 상품 ID입니다.\n");
            return;
        }

        printf("> 입고수량 : ");
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &restock_qty) != 1) {
                printf("오류: 입고 수량은 숫자여야 합니다.\n");
                return;
            }
        }
        else { return; }

        products[product_id - 1].stock += restock_qty;
        printf("ID %d 상품 입고가 완료되었습니다.\n", product_id);
    }
    else {
        printf("잘못된 선택입니다.\n");
    }
}

// 판매 함수도 안정성을 위해 fgets/sscanf 방식으로 수정
void sellProducts(Product products[]) {
    int product_id, sell_qty;
    char line_buffer[100];

    printf("<<< 2. 판매를 선택했을 때\n");
    printf("> 상품ID (1-%d) : ", NUM_PRODUCTS);

    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &product_id) != 1) {
            printf("오류: 상품 ID는 숫자여야 합니다.\n"); return;
        }
    }
    else { return; }

    if (product_id < 1 || product_id > NUM_PRODUCTS) {
        printf("잘못된 상품 ID입니다.\n"); return;
    }

    printf("> 판매수량 : ");
    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &sell_qty) != 1) {
            printf("오류: 판매 수량은 숫자여야 합니다.\n"); return;
        }
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

// 상황 출력 함수는 입력 부분이 없으므로 그대로입니다.
void printStatus(const Product products[]) {
    printf("<<< 3. 상황현황을 선택했을 때\n");
    int total_sales = 0;
    int total_available = 0;
    int max_sales_id = 1, min_sales_id = 1;
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        total_sales += products[i].sales;
        total_available += products[i].stock + products[i].sales;
    }
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
    printf("총 판매량 : %d\n", total_sales);
    if (total_available > 0) {
        printf("전체 판매율 : %.2f%%\n", ((double)total_sales / total_available) * 100.0);
    }
    else {
        printf("전체 판매율 : 0.00%%\n");
    }
    printf("가장 많이 판매된 상품 : ID %d, 판매량 %d\n", max_sales_id, max_sales);
    printf("가장 적게 판매된 상품 : ID %d, 판매량 %d\n", min_sales_id, min_sales);
    printf("--- 상품별 남은 재고 ---\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("상품 ID %d 재고 : %d\n", i + 1, products[i].stock);
    }
}
