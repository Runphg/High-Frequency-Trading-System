import json
import time
import random
from datetime import datetime
from kafka import KafkaProducer

# 1. Kafka Producer 설정 (도커 컨테이너의 9092 포트로 연결)
producer = KafkaProducer(
    bootstrap_servers=['localhost:9092'],
    value_serializer=lambda v: json.dumps(v).encode('utf-8')
)

# 2. 가상의 주식 종목 리스트
SYMBOLS = ['AAPL', 'TSLA', 'MSFT', 'NVDA', 'SAMSUNG']

print("🚀 실시간 주식 시세 데이터 전송을 시작합니다... (종료하려면 Ctrl+C)")

try:
    while True:
        # 3. 실시간 틱(Tick) 데이터 무작위 생성
        tick_data = {
            "symbol": random.choice(SYMBOLS),
            "price": round(random.uniform(100.0, 500.0), 2),
            "volume": random.randint(1, 100),
            "timestamp": datetime.now().isoformat()
        }

        # 4. 'market-ticks'라는 토픽(파이프라인)으로 데이터 전송
        producer.send('market-ticks', value=tick_data)
        
        print(f"[전송 완료] {tick_data}")
        
        # 0.001초 대기 
        time.sleep(0.001)
        
except KeyboardInterrupt:
    print("\n데이터 전송을 중단합니다.")
finally:
    producer.close()