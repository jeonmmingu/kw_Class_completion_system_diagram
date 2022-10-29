# kw_Class_completion_system_diagram
고급 c프로그래밍 project

[목표]
- 이수체계도의 접근성 및 사용성을 개편하기 위한 프로그램을 설계


[구현순서]

[1] "subject* load_subject_information()"

# Description
: 이수 체계도에 있는 과목 정보를 txt로부터 불러오고, 이를 이용하여 Linked list를 생성하는 함수

# Function arguments
: None

# Return Value
: linked list의 Entry point 객체를 반환

[2] "void print_connecting_subject(subject* sub)" 
# Description
: 인자로 입력받은 과목과 연결된 선후수 과목들을 모두 출력하는 

# Function arguments
: 선후수 과목을 알고 싶은 과목

# Return Value
: None
