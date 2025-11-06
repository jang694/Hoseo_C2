#include <stdio.h>
#include <stdlib.h> // malloc, free, exit를 위해 추가
#include <string.h>

// --- 전역 설정 ---

// 상품 정보를 저장할 파일명
#define INVENTORY_FILE "inventory.dat"

// 최대 저장 가능한 상품 갯수
#define MAX_PRODUCTS 5

// 상품 구조체 정의
typedef struct {
    char id[50];         // 상품 ID
    char name[100];      // 상품명
    int stock;           // 재고 수량
    int price;           // 판매 가격
    int cost;            // 입고 가격
    long long sales_amount; // 총 판매금액 (큰 숫자를 위해 long long)
} Product;

// [동적 할당]
// 상품 정보를 담을 전역 포인터와 현재 상품 수
Product* products = NULL; // 동적 할당될 배열을 가리킬 포인터
int product_count = 0;

// --- 함수 선언 ---
void load_data();
void save_data();
Product* find_product(const char* product_id);
Product* find_product_by_name(const char* product_name); // 상품명 검색 함수 추가
void get_string_input(const char* prompt, char* buffer, int buffer_size);
int get_integer_input(const char* prompt);
void handle_receive();
void handle_sell();
void view_individual();
void view_total();
void print_menu();

// --- 데이터 로드/저장 ---

/**
 * @brief 프로그램 시작 시 inventory.dat 파일에서 데이터를 불러옵니다.
 * (바이너리 모드)
 */
void load_data() {
    FILE* f = fopen(INVENTORY_FILE, "rb");
    if (f == NULL) {
        printf("'%s' 파일이 없습니다. 새 목록으로 시작합니다.\n", INVENTORY_FILE);
        product_count = 0;
        return;
    }

    // 1. 저장된 상품 갯수(product_count)를 먼저 읽어옵니다.
    if (fread(&product_count, sizeof(int), 1, f) != 1) {
        printf("'%s' 파일 읽기 오류 (count). 새 목록으로 시작합니다.\n", INVENTORY_FILE);
        product_count = 0;
        fclose(f);
        return;
    }

    // 2. 파일의 데이터가 최대치를 초과하는지 확인 (파일 손상 방지)
    if (product_count > MAX_PRODUCTS) {
        printf("데이터 파일 오류: 상품 갯수(%d)가 최대치(%d)를 초과합니다. 목록을 초기화합니다.\n", product_count, MAX_PRODUCTS);
        product_count = 0;
        fclose(f);
        return;
    }

    // 3. 상품 갯수만큼 상품 데이터(Product 배열)를 읽어옵니다.
    // products 포인터가 가리키는 메모리 위치에 직접 씁니다.
    if (product_count > 0) {
        if (fread(products, sizeof(Product), product_count, f) != product_count) {
            printf("'%s' 파일 읽기 오류 (data). 새 목록으로 시작합니다.\n", INVENTORY_FILE);
            product_count = 0;
        }
        else {
            printf("'%s'에서 데이터를 불러왔습니다. (상품 %d개)\n", INVENTORY_FILE, product_count);
        }
    }
    else {
        printf("'%s'에서 데이터를 불러왔습니다. (상품 0개)\n", INVENTORY_FILE);
    }

    fclose(f);
}

/**
 * @brief 프로그램 종료 시 products 배열을 inventory.dat 파일에 저장합니다.
 * (바이너리 모드)
 */
void save_data() {
    FILE* f = fopen(INVENTORY_FILE, "wb");
    if (f == NULL) {
        printf("데이터 저장 중 오류 발생: 파일을 열 수 없습니다.\n");
        return;
    }

    // 1. 현재 상품 갯수(product_count)를 먼저 씁니다.
    if (fwrite(&product_count, sizeof(int), 1, f) != 1) {
        printf("데이터 저장 중 오류 발생 (count).\n");
        fclose(f);
        return;
    }

    // 2. 상품 갯수만큼 상품 데이터(Product 배열)를 씁니다.
    // products 포인터가 가리키는 메모리에서 데이터를 읽어 씁니다.
    if (product_count > 0) {
        if (fwrite(products, sizeof(Product), product_count, f) != product_count) {
            printf("데이터 저장 중 오류 발생 (data).\n");
        }
        else {
            printf("데이터를 '%s'에 저장했습니다.\n", INVENTORY_FILE);
        }
    }
    else {
        printf("데이터를 '%s'에 저장했습니다. (상품 0개)\n", INVENTORY_FILE);
    }

    fclose(f);
}

// --- 유틸리티 함수 ---

/**
 * @brief 상품 ID로 products 배열에서 상품을 찾아 포인터를 반환합니다.
 * @param product_id 검색할 상품 ID
 * @return 찾은 경우 Product 포인터, 못 찾은 경우 NULL
 */
Product* find_product(const char* product_id) {
    for (int i = 0; i < product_count; i++) {
        // C언어에서는 문자열 비교 시 strcmp 사용
        if (strcmp(products[i].id, product_id) == 0) {
            return &products[i]; // 해당 상품의 주소 반환
        }
    }
    return NULL; // 못 찾음
}

/**
 * @brief 상품명으로 products 배열에서 상품을 찾아 포인터를 반환합니다. (중복 검사용)
 * @param product_name 검색할 상품명
 * @return 찾은 경우 Product 포인터, 못 찾은 경우 NULL
 */
Product* find_product_by_name(const char* product_name) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].name, product_name) == 0) {
            return &products[i]; // 해당 상품의 주소 반환
        }
    }
    return NULL; // 못 찾음
}


/**
 * @brief 사용자로부터 한 줄의 문자열 입력을 안전하게 받습니다. (fgets 사용)
 * @param prompt 사용자에게 보여줄 안내 메시지
 * @param buffer 입력받은 문자열을 저장할 버퍼
 * @param buffer_size 버퍼의 크기
 */
void get_string_input(const char* prompt, char* buffer, int buffer_size) {
    printf("%s", prompt);
    fflush(stdout); // 프롬프트가 바로 보이도록 버퍼 비우기
    fgets(buffer, buffer_size, stdin);

    // fgets로 읽은 문자열의 끝에 있는 개행 문자(\n) 제거
    buffer[strcspn(buffer, "\n")] = 0;
}

/**
 * @brief 사용자로부터 0 이상의 정수 입력을 안전하게 받습니다.
 * @param prompt 사용자에게 보여줄 안내 메시지
 * @return 입력받은 0 이상의 정수
 */
int get_integer_input(const char* prompt) {
    char buffer[100];
    int value;
    char remaining; // 입력 버퍼에 남은 문자가 있는지 확인하기 위함

    while (1) {
        get_string_input(prompt, buffer, sizeof(buffer));

        // sscanf를 사용해 문자열에서 정수를 읽어옴
        // %c로 남은 문자가 있는지 확인하여 "123a" 같은 입력을 걸러냄
        if (sscanf(buffer, "%d%c", &value, &remaining) == 1) {
            if (value < 0) {
                printf("오류: 0 이상의 숫자를 입력해야 합니다.\n");
            }
            else {
                return value; // 성공
            }
        }
        else {
            printf("오류: 유효한 숫자를 입력하세요.\n");
        }
    }
}

// --- 메인 메뉴 기능 함수 ---

/**
 * @brief 1. 입고 메뉴 (신규/추가) 처리를 담당합니다.
 */
void handle_receive() {
    char product_id[50];
    printf("\n<< 1. 입고 메뉴 실행 >>\n");
    get_string_input("상품 ID: ", product_id, sizeof(product_id));

    if (strlen(product_id) == 0) {
        printf("오류: 상품 ID를 입력해야 합니다.\n");
        return;
    }

    Product* product = find_product(product_id);

    if (product != NULL) {
        // --- 기존 상품 입고 (업데이트) ---
        printf("기존 상품 [%s]의 재고를 추가합니다.\n", product->name);

        int qty = get_integer_input("추가 입고 수량: ");
        product->stock += qty;

        // 입고 시 가격 정보도 업데이트
        printf("(현재 판매 가격: %d)\n", product->price);
        product->price = get_integer_input("새 판매 가격: ");

        printf("(현재 입고 가격: %d)\n", product->cost);
        product->cost = get_integer_input("새 입고 가격: ");

        printf("상품 [%s]의 재고가 %d개로 업데이트되었습니다.\n", product->name, product->stock);

    }
    else {
        // --- 신규 상품 입고 (생성) ---
        if (product_count >= MAX_PRODUCTS) {
            printf("오류: 최대 상품 갯수(%d개)에 도달했습니다. 새 상품을 추가할 수 없습니다.\n", MAX_PRODUCTS);
            return;
        }

        printf("신규 상품을 등록합니다.\n");

        Product* new_product = &products[product_count]; // 새 상품이 저장될 위치
        char new_name[100]; // 임시로 상품명 받을 버퍼

        // 문자열 복사
        strcpy(new_product->id, product_id);

        get_string_input("상품명: ", new_name, sizeof(new_name));
        if (strlen(new_name) == 0) {
            printf("오류: 상품명을 입력해야 합니다.\n");
            return; // product_count가 증가하지 않도록 여기서 중단
        }

        // *** 상품명 중복 검사 추가 ***
        if (find_product_by_name(new_name) != NULL) {
            printf("오류: 동일한 상품명('%s')이 이미 존재합니다.\n", new_name);
            return; // 중복이므로 등록 중단
        }

        // 중복이 아니므로 상품명 복사
        strcpy(new_product->name, new_name);

        new_product->stock = get_integer_input("초기 입고 수량: ");
        new_product->price = get_integer_input("판매 가격: ");
        new_product->cost = get_integer_input("입고 가격: ");
        new_product->sales_amount = 0; // 신규 상품이므로 총 판매금액은 0

        // 상품 등록이 성공적으로 완료되었으므로, 전체 상품 갯수를 1 증가
        product_count++;

        printf("신규 상품 [%s]이(가) 등록되었습니다.\n", new_product->name);
    }
}

/**
 * @brief 2. 판매 메뉴 처리를 담당합니다.
 */
void handle_sell() {
    char product_id[50];
    printf("\n<< 2. 판매 메뉴 실행 >>\n");
    get_string_input("상품 ID: ", product_id, sizeof(product_id));

    Product* product = find_product(product_id);

    if (product == NULL) {
        printf("오류: 해당 ID의 상품이 존재하지 않습니다.\n");
        return;
    }

    int qty = get_integer_input("판매 수량: ");

    if (qty == 0) {
        printf("판매 수량이 0입니다. 작업을 취소합니다.\n");
        return;
    }

    if (qty > product->stock) {
        printf("오류: 재고가 부족합니다. (현재 재고: %d개)\n", product->stock);
    }
    else {
        product->stock -= qty;
        long long sale_value = (long long)product->price * qty;
        product->sales_amount += sale_value;
        printf("상품 [%s] %d개 판매 완료.\n", product->name, qty);
        printf("남은 재고: %d개\n", product->stock);
        // %lld는 long long int를 출력하기 위한 서식 지정자
        printf("해당 상품 총 판매금액: %lld원\n", product->sales_amount);
    }
}

/**
 * @brief 3. 개별 현황 메뉴 처리를 담당합니다.
 */
void view_individual() {
    char product_id[50];
    printf("\n<< 3. 개별 현황 실행 >>\n");
    get_string_input("상품 ID: ", product_id, sizeof(product_id));

    Product* product = find_product(product_id);

    if (product == NULL) {
        printf("오류: 해당 ID의 상품이 존재하지 않습니다.\n");
    }
    else {
        printf("--- 상품 개별 현황 ---\n");
        printf("  상품 ID:    %s\n", product->id);
        printf("  상품명:     %s\n", product->name);
        printf("  재고 수량:  %d 개\n", product->stock);
        printf("  판매 가격:  %d 원\n", product->price);
        printf("  입고 가격:  %d 원\n", product->cost);
        printf("  총 판매금액: %lld 원\n", product->sales_amount); // long long
        printf("------------------------\n");
    }
}

/**
 * @brief 4. 전체 현황 메뉴 처리를 담당합니다.
 */
void view_total() {
    printf("\n<< 4. 전체 현황 실행 >>\n");
    if (product_count == 0) {
        printf("등록된 상품이 없습니다.\n");
        return;
    }

    printf("--- 상품 전체 현황 ---\n");
    long long total_stock_value = 0;
    long long total_sales_amount = 0;

    for (int i = 0; i < product_count; i++) {
        Product* p = &products[i];
        printf("  ID: %s, 이름: %s, 재고: %d, 판매가: %d, 총판매액: %lld\n",
            p->id, p->name, p->stock, p->price, p->sales_amount);

        total_stock_value += (long long)p->cost * p->stock;
        total_sales_amount += p->sales_amount;
    }

    printf("------------------------\n");
    printf("총 상품 가짓수: %d 개\n", product_count);
    printf("총 재고 자산 (입고가 기준): %lld 원\n", total_stock_value);
    printf("모든 상품 총 판매금액 합계: %lld 원\n", total_sales_amount);
}

/**
 * @brief 메인 메뉴를 출력합니다.
 */
void print_menu() {
    printf("\n===== 쇼핑몰 재고 관리 시스템 (C ver.) =====\n");
    printf("1. 입고 (신규/추가)\n");
    printf("2. 판매\n");
    printf("3. 개별 현황 (ID검색)\n");
    printf("4. 전체 현황\n");
    printf("5. 종료 (자동 저장)\n");
    printf("===========================================\n");
}

/**
 * @brief 메인 프로그램 루프를 실행합니다.
 */
int main() {
    // [동적 할당]
    // 프로그램 시작 시 MAX_PRODUCTS 만큼 메모리 할당
    products = (Product*)malloc(sizeof(Product) * MAX_PRODUCTS);
    if (products == NULL) {
        printf("치명적 오류: 메모리를 할당할 수 없습니다. 프로그램을 종료합니다.\n");
        return 1; // 비정상 종료
    }


    load_data(); // 프로그램 시작 시 데이터 로드
    char choice_buffer[10];

    while (1) {
        print_menu();
        get_string_input("원하는 메뉴를 선택하세요 (1-5): ", choice_buffer, sizeof(choice_buffer));

        // 입력이 "1", "2" 처럼 한 글자인지 확인
        if (strlen(choice_buffer) != 1) {
            printf("\n오류: 잘못된 입력입니다. 1-5 사이의 숫자 하나만 입력하세요.\n");
            continue;
        }

        char choice = choice_buffer[0]; // 입력된 첫번째 글자

        switch (choice) {
        case '1':
            handle_receive();
            break;
        case '2':
            handle_sell();
            break;
        case '3':
            view_individual();
            break;
        case '4':
            view_total();
            break;
        case '5':
            save_data(); // 프로그램 종료 시 데이터 저장
            printf("\n프로그램을 종료합니다. 이용해주셔서 감사합니다.\n");

            // [동적 할당]
            // 종료 시 할당된 메모리 해제
            free(products);

            return 0; // 프로그램 종료
        default:
            printf("\n오류: 잘못된 입력입니다. 1-5 사이의 숫자를 입력하세요.\n");
            break;
        }
    }

    // [동적 할당]
    // (보통 이 라인까지 오지 않지만, 만약의 경우를 대비해)
    free(products);
    return 0;
}