#include "subject.h"

subject* load_subject_information();                // 데이터를 불러와 과목 객체들을 만들고 연결하는 함수 => 연결 방식(binary tree)
void print_connecting_subject(subject* sub);        // 연결 된 선후수 과목들을 전부 출력하는 함수


int main(void){
    // 학년, 학기에 대한 변수
    int user_school_year = 0;
    int user_school_semester = 0;
    subject* sub_ptr = NULL;

    // 선후수 데이터 불러오기 & 과목 객체 연결
    make_subject_diagram();

    // 학년 정보 입력받기 (영문, 한글 입력받아진 경우 예외처리)
    while(1){
        printf("학년을 입력하세요(1 ~ 4): ");
        scanf("%d", &user_school_year);
        if(user_school_year < 5 && user_school_year > 0) break;
        else printf("(1학년 ~ 4학년 중 입력해주시길 바랍니다.)\n");
    }
    // 학기 정보 입력받기
    while(1){
        printf("학기를 입력하세요(1 or 2): ");
        scanf("%d", &user_school_semester);
        if(user_school_semester == 1  || user_school_semester == 2) break;
        else printf("(1학기, 2학기 중 하나를 입력해주시길 바랍니다.)\n");
    }
    // 해당 학년, 학기에 반드시 들어야하는 과목과 추천 과목 list-up

    // list-up 된 과목 중 알아보고 싶은 과목을 검색 ⇒ 필수, 추천 선/후수를 구분하여 출력
    
    // Done
    return 0;
}

subject* load_subject_information(){
    return NULL;
}

void print_connecting_subject(subject* sub){
    return;
}

