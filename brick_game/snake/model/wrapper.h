#ifndef WRAPPER_H_
#define WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

// "ручка" на C++ класс
typedef void* GameHandle;

// управление игрой
GameHandle game_create();
void game_destroy(GameHandle g);

void game_update(GameHandle g);   // обновить состояние игры
void game_set_direction(GameHandle g, int dir); // сменить направление

// данные для отрисовки
int game_get_score(GameHandle g);
int game_get_pause(GameHandle g);
int game_get_snake_length(GameHandle g);

// получаем тело змейки (x,y пары)
// bufSize — сколько элементов можно положить
// возвращает фактическое кол-во точек
int game_get_snake_body(GameHandle g, int* buffer, int bufSize);

// координаты яблока
void game_get_apple(GameHandle g, int* x, int* y);

#ifdef __cplusplus
}
#endif

#endif  // WRAPPER_H_

