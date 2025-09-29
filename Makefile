CC = g++
CFLAGS = -g -fPIC
LDFLAGS = -lncurses -lcheck -lm -lsubunit
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

EXE_NAME = brick_game_cli

SNAKE_LIB_NAME = libsnake.a
SNAKE_SRC = brick_game/snake/controller/wrapper.cpp \
            brick_game/snake/model/game.cpp \
            brick_game/snake/model/snake.cpp \
            brick_game/snake/model/apple.cpp \
            brick_game/snake/controller/contoller.cpp

LIB_NAME = libtetris.a
LIB_DIR = brick_game/tetris
GUI_DIR = gui/cli
TEST_DIR = tests
BUILD_DIR = build

# Файлы
LIB_SRC = $(LIB_DIR)/s21_tetris.c
GUI_SRC = $(GUI_DIR)/gui.c
TEST_SRC = $(TEST_DIR)/test.c
HEADERS = $(LIB_DIR)/s21_tetris.h

# Qt Desktop build
QT_DIR = gui/desktop
QT_SRC = $(QT_DIR)/main_qt.cpp $(QT_DIR)/snakewidget.cpp
QT_OBJ = $(QT_SRC:%.cpp=$(BUILD_DIR)/%.o)
QT_EXE = brick_game_qt

# Qt flags (Qt5)
QT_CFLAGS = $(shell pkg-config --cflags Qt5Widgets)
QT_LDFLAGS = $(shell pkg-config --libs Qt5Widgets)

MOC = moc
MOC_SOURCES = $(wildcard $(QT_DIR)/*.h)
MOC_OBJ = $(MOC_SOURCES:$(QT_DIR)/%.h=$(BUILD_DIR)/%.moc.o)

# Объекты
LIB_OBJ = $(BUILD_DIR)/s21_tetris.o
SNAKE_OBJ = $(SNAKE_SRC:%.cpp=$(BUILD_DIR)/%.o)
GUI_OBJ = $(BUILD_DIR)/gui.o
TEST_OBJ = $(BUILD_DIR)/test.o

# Создаём build папку
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Компиляция общих .c / .cpp
$(LIB_OBJ): $(LIB_SRC) $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(GUI_OBJ): $(GUI_SRC) $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): $(TEST_SRC) $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(QT_CFLAGS) -c $< -o $@

# moc правило
$(BUILD_DIR)/%.moc.o: $(QT_DIR)/%.h | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(MOC) $< -o $(BUILD_DIR)/$*.moc.cpp
	$(CC) $(CFLAGS) $(QT_CFLAGS) -c $(BUILD_DIR)/$*.moc.cpp -o $@

# Desktop (Qt) build
$(QT_EXE): $(QT_OBJ) $(MOC_OBJ) $(SNAKE_LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(QT_OBJ) $(MOC_OBJ) -L. -lsnake $(QT_LDFLAGS)

# Сборка библиотек
$(LIB_NAME): $(LIB_OBJ)
	ar rcs $@ $^

$(SNAKE_LIB_NAME): $(SNAKE_OBJ)
	ar rcs $@ $^

# CLI исполняемый файл
$(EXE_NAME): $(GUI_OBJ) $(LIB_NAME) $(SNAKE_LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(GUI_OBJ) -L. -ltetris -L. -lsnake $(LDFLAGS)
	echo 0 > highscore_tetris.txt
	echo 0 > highscore_snake.txt

# Основная цель
all: $(LIB_NAME) $(SNAKE_LIB_NAME) $(EXE_NAME) $(QT_EXE)

# Qt desktop target
desktop: $(QT_EXE)


install: clean $(EXE_NAME)

uninstall: clean

# Сборка и запуск тестов

test: $(TEST_OBJ) $(LIB_NAME) $(SNAKE_LIB_NAME)
	g++ $(CFLAGS) -o test_exe $(TEST_OBJ) -L. -ltetris -L. -lsnake $(LDFLAGS)
	./test_exe
	echo 0 > highscore.txt

# Gcov отчет
gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) $(LIB_SRC) $(LDFLAGS) -o gcov_exe
	./gcov_exe
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o gcov_report gcov_report.info

# Документация
dvi:
	doxygen Doxyfile

# Арихв
dist:
	tar -czvf tetris.tar.gz \
	Makefile \
	brick_game \
	gui \
	tests

clang_format:
	clang-format -n $(LIB_SRC) $(HEADERS)
	clang-format -i $(LIB_SRC) $(HEADERS)

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME) $(SNAKE_LIB_NAME) $(EXE_NAME) test_exe gcov_exe *.gcno *.gcda *.gcov gcov_report.info gcov_report docs tetris.tar.gz highscore_*.txt brick_game_*

.PHONY: all clean test gcov_report

