#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include <mysql.h>
#include <librdkafka/rdkafkacpp.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    // 실행 인자: 1.스레드 수 / 2.배치 크기 / 3.테스트 반복 횟수 (새로 추가)
    int TARGET_THREADS = (argc > 1) ? atoi(argv[1]) : 4;
    int TARGET_BATCH_SIZE = (argc > 2) ? atoi(argv[2]) : 5000;
    int MAX_BATCH_COUNT = (argc > 3) ? atoi(argv[3]) : 20; // 기본값: 20번만 처리하고 자동 종료

    omp_set_num_threads(TARGET_THREADS);

    cout << "\n🚀 [자동화 실험 모드 가동]" << endl;
    cout << "👉 스레드 수: " << TARGET_THREADS << "개" << endl;
    cout << "👉 배치 크기: " << TARGET_BATCH_SIZE << "건" << endl;
    cout << "👉 테스트 횟수: " << MAX_BATCH_COUNT << "회 (완료 시 자동 종료)\n"
         << endl;

    // 1. MySQL 연결
    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "127.0.0.1", "hft_user", "hft_password", "hft_db", 3307, NULL, 0))
    {
        cerr << "MySQL 연결 실패: " << mysql_error(conn) << endl;
        return 1;
    }
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS tick_summary (id INT AUTO_INCREMENT PRIMARY KEY, thread_cnt INT, batch_size INT, processing_time_ms DOUBLE, log_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");

    // 2. Kafka Consumer 설정
    string errstr;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", "127.0.0.1:9092", errstr);
    conf->set("group.id", "hft-worker-group-" + to_string(TARGET_THREADS), errstr); // 그룹 ID 분리

    RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);
    vector<string> topics = {"market-ticks"};
    consumer->subscribe(topics);

    int processed_batches = 0;
    double total_processing_time = 0.0;

    // 3. 무한 루프(while true) 대신 지정된 횟수(MAX_BATCH_COUNT)만큼만 반복
    while (processed_batches < MAX_BATCH_COUNT)
    {
        vector<string> batch_messages;

        for (int i = 0; i < TARGET_BATCH_SIZE; ++i)
        {
            RdKafka::Message *msg = consumer->consume(5);
            if (msg->err() == RdKafka::ERR_NO_ERROR)
            {
                batch_messages.push_back(string(static_cast<const char *>(msg->payload()), msg->len()));
            }
            delete msg;
        }

        int actual_batch_size = batch_messages.size();
        if (actual_batch_size > 0)
        {
            double start_time = omp_get_wtime();

#pragma omp parallel for
            for (int i = 0; i < actual_batch_size; ++i)
            {
                volatile double dummy_var = 0.0;
                for (int j = 0; j < 100000; ++j)
                {
                    dummy_var += (j * 0.0001);
                }
            }

            double end_time = omp_get_wtime();
            double processing_time = (end_time - start_time) * 1000.0;

            cout << "[" << processed_batches + 1 << "/" << MAX_BATCH_COUNT << "] 수신: " << actual_batch_size << "건 | ⚡ 처리 시간: " << processing_time << " ms" << endl;

            string query = "INSERT INTO tick_summary (thread_cnt, batch_size, processing_time_ms) VALUES (" + to_string(TARGET_THREADS) + ", " + to_string(actual_batch_size) + ", " + to_string(processing_time) + ")";
            mysql_query(conn, query.c_str());

            total_processing_time += processing_time;
            processed_batches++;
        }
    }

    // 4. 테스트 종료 및 통계 출력
    cout << "\n✅ 테스트 완료! (스레드: " << TARGET_THREADS << "개)" << endl;
    cout << "📊 1회(배치) 평균 처리 시간: " << (total_processing_time / MAX_BATCH_COUNT) << " ms\n"
         << endl;

    consumer->close();
    delete consumer;
    mysql_close(conn);
    return 0;
}
