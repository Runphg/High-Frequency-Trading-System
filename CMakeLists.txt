cmake_minimum_required(VERSION 3.10)
project(HighFrequencyTradingSystem)

# C++ 17 표준 사용
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Apple Silicon 맥북의 Homebrew 기본 경로 설정
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# ⭐ 수정됨: mysql-client 헤더 경로를 하위 폴더(/mysql)까지 정확하게 지정 ⭐
include_directories(/opt/homebrew/opt/mysql-client/include/mysql)
link_directories(/opt/homebrew/opt/mysql-client/lib)

# libomp (OpenMP) 경로 명시적 지정
include_directories(/opt/homebrew/opt/libomp/include)
link_directories(/opt/homebrew/opt/libomp/lib)

# Mac(AppleClang)을 위한 OpenMP 컴파일러 플래그 강제 주입
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp")

# src/main.cpp 파일을 컴파일하여 'worker'라는 실행 파일 생성
add_executable(worker src/main.cpp)

# 필수 라이브러리들을 실행 파일에 연결
target_link_libraries(worker PRIVATE
    omp
    rdkafka++
    mysqlclient
    pthread
)