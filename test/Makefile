.PHONY: all clean

# 默认目标
all: test_proc_image

# 编译生成可执行文件
test_proc_image: test_proc_image.c lock_tracepoint.c
	gcc $^ -o $@ -lpthread -llttng-ust -lm

# 清理生成的文件
clean:
	rm -f test_proc_image lock_tracepoint.o