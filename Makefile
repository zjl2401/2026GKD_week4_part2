# Makefile for 2026GKD_week4_part2

# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

# 目标可执行文件名
TARGET = 2026GKD_week4_part2

# 源文件
SOURCES = main.cpp TaskManager.cpp TaskBase.cpp

# 对象文件
OBJECTS = $(SOURCES:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接可执行文件
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# 编译源文件为对象文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
ifeq ($(OS),Windows_NT)
	del /Q $(OBJECTS) $(TARGET) 2>nul || true
else
	rm -f $(OBJECTS) $(TARGET)
endif

# 重新编译
rebuild: clean all

# 声明伪目标
.PHONY: all clean rebuild

# 下载并解压 MNIST 数据到 data/mnist
# 用法:
#   make download-mnist    # 只下载 .gz 文件
#   make extract-mnist     # 只解压已有的 .gz 文件
#   make mnist             # 下载并解压
MNIST_DIR = data/mnist
MNIST_FILES = train-images-idx3-ubyte.gz train-labels-idx1-ubyte.gz \
              t10k-images-idx3-ubyte.gz t10k-labels-idx1-ubyte.gz
MNIST_URL = http://yann.lecun.com/exdb/mnist

download-mnist:
	@echo "Creating $(MNIST_DIR) if needed..."
	@mkdir -p $(MNIST_DIR)
	@echo "Downloading MNIST files to $(MNIST_DIR)..."
ifeq ($(OS),Windows_NT)
	@for %%f in ($(MNIST_FILES)) do powershell -NoProfile -Command "\"$u='%s/%%f'; $out='%s\\%%f'; if (-Not (Test-Path $out)) { Invoke-WebRequest -Uri $u -OutFile $out -UseBasicParsing } else { Write-Host 'Skip' $out }\"" \
		"$(MNIST_URL)" "$(MNIST_DIR)"
else
	@for f in $(MNIST_FILES); do \
		if [ ! -f $(MNIST_DIR)/$$f ]; then \
			wget -c $(MNIST_URL)/$$f -P $(MNIST_DIR); \
		else \
			echo "Skip $(MNIST_DIR)/$$f"; \
		fi; \
	done
endif

extract-mnist:
	@echo "Extracting .gz files in $(MNIST_DIR)..."
ifeq ($(OS),Windows_NT)
	@powershell -NoProfile -Command "Get-ChildItem -Path '$(MNIST_DIR)' -Filter '*.gz' -File | ForEach-Object { $in = $_.FullName; $out = [IO.Path]::ChangeExtension($in, $null); if (-Not (Test-Path $out)) { $fsin = [IO.File]::OpenRead($in); $fsout = [IO.File]::Create($out); $gz = New-Object IO.Compression.GzipStream($fsin,[IO.Compression.CompressionMode]::Decompress); $gz.CopyTo($fsout); $gz.Close(); $fsout.Close(); $fsin.Close(); Write-Host 'Extracted' $out } else { Write-Host 'Skip' $out } }"
else
	@gzip -dk $(MNIST_DIR)/*.gz || true
endif

mnist: download-mnist extract-mnist

.PHONY: download-mnist extract-mnist mnist

