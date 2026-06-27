import matplotlib.pyplot as plt

# 실험 데이터
threads = [1, 2, 4, 8, 10, 16]
processing_times = [464.006, 253.327, 148.683, 125.335, 117.166, 109.760]

# 그래프 설정
plt.figure(figsize=(10, 6))
plt.plot(threads, processing_times, marker='o', linestyle='-', color='#1f77b4', linewidth=2, markersize=8)

# 제목 및 라벨
plt.title('OpenMP Thread Scaling Performance (M4 Chip)', fontsize=16, pad=15)
plt.xlabel('Number of Threads', fontsize=12)
plt.ylabel('Average Processing Time (ms)', fontsize=12)
plt.xticks(threads)
plt.grid(True, linestyle='--', alpha=0.7)

# 데이터 수치 표시
for i, txt in enumerate(processing_times):
    plt.annotate(f"{txt:.1f} ms", (threads[i], processing_times[i]), 
                 textcoords="offset points", xytext=(0, 10), ha='center', fontsize=10)

# 이미지 파일로 저장
plt.tight_layout()
plt.savefig('thread_scaling_result.png', dpi=300)
print("✅ 그래프가 'thread_scaling_result.png' 파일로 저장되었습니다!")