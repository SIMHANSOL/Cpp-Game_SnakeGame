#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#define none -4;

int x_max_size = 50; // X 최대 사이즈.
int y_max_size = 20; // Y 최대 사이즈.
bool game_start = false;
void goto_xy(int _x, int _y)
{
  COORD pos = {
      _x,
      _y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int random_range(int _x1, int _x2)
{
  int range = (_x2 - _x1) + 1;
  return (rand() % range) + _x1;
}
enum direction
{
  LEFT = 0,
  RIGHT = 1,
  UP = 2,
  DOWN = 3
};
class snake
{
public:
  int x,
      temp_x,
      y,
      temp_y,
      move_speed,
      value;     // 좌표와 이동 속도, 그리고 생성 순서.
  char *draw;    // 모양.
  direction dir; // 방향.
  snake *next;
  snake *prev;
  snake()
  {
    x = 0,
    y = 0;
    temp_x = x,
    temp_y = y;
    move_speed = 1;
    value = 0;
    draw = "●\n";
    dir = LEFT;
  }
  void head_update()
  { // 플레이어만 실행되는 함수.
    draw = "●\n";
    if (_kbhit())
    { // 키 입력 처리.
      switch (_getch())
      {
      case 'a':
        dir = LEFT;
        break;
      case 'd':
        dir = RIGHT;
        break;
      case 'w':
        dir = UP;
        break;
      case 's':
        dir = DOWN;
        break;
      }
    }
    temp_x = x;
    temp_y = y;
    switch (dir)
    { // 이동.
    case LEFT:
      x -= move_speed;
      break;
    case RIGHT:
      x += move_speed;
      break;
    case UP:
      y -= move_speed;
      break;
    case DOWN:
      y += move_speed;
      break;
    }
    goto_xy(x, y);
    printf("%c%c", draw[0], draw[1]);
    if ((x <= 1 || x >= x_max_size - 3) || (y <= 0 || y >= y_max_size - 1))
    { // 벽에 닿았을 경우 게임 오버.
      game_start = false;
    }
    Sleep(60);
    // DEBUG
    system("cls");
    for (int m = 0; m < y_max_size; m++)
    {
      goto_xy(0, m);
      if ((m == 0) || (m == y_max_size - 1))
      {
        for (int n = 0; n < x_max_size; n += 2)
        {
          printf("□");
        }
      }
      else
      {
        printf("□");
        goto_xy(x_max_size - 2, m);
        printf("□");
      }
    }
  }
  void tail_update(snake *head)
  { // 꼬리들.
    draw = "○\n";
    // goto_xy(x, y);
    // printf(" ");
    int x_check = abs(x - head->x);
    int y_check = abs(y - head->y);
    if (x_check < 1 && y_check < 1)
    {
      game_start = false;
    }
    temp_x = x;
    temp_y = y;
    x = prev->temp_x;
    y = prev->temp_y;
    goto_xy(x, y);
    printf("%c%c", draw[0], draw[1]);
  }
};
class wall
{
public:
  int x,
      y;
  char *draw;
  wall()
  {
    x = 0,
    y = 0;
    draw = "■\n";
  }
};
class food
{
public:
  bool _switch; // 작동하는지 안 하는지.
  int x,
      y;
  char *draw;
  food()
  {
    _switch = false;
    x = 0,
    y = 0;
    draw = "⊙\n";
  }
  void update()
  {
    goto_xy(x, y);
    printf("%c%c", draw[0], draw[1]);
  }
};
class ds_list_create
{ // LIST 구조.
public:
  snake *head;
  snake *tail;
  snake *temp;

public:
  ds_list_create()
  {
    head = NULL;
    tail = NULL;
    temp = NULL;
  }
  void ds_list_add(int value)
  {
    snake *New = new snake;
    New->value = value;
    New->prev = NULL;
    New->next = NULL;
    if (head == NULL)
    {
      head = New;
    }
    if (temp != NULL)
    {
      New->prev = temp;
      temp->next = New;
      New->x = temp->x;
      New->y = temp->y;
    }
    temp = New;
    tail = New;
  }
  void ds_list_delete(int index)
  {
    int _index = 1;
    snake *New = head;
    while (1)
    {
      if (_index == index)
      {
        snake *prev = NULL;
        snake *next = NULL;
        if (New == head)
        {
          next = New->next;
          next->prev = NULL;
          head = next;
        }
        else if (New == tail)
        {
          prev = New->prev;
          prev->next = NULL;
          tail = prev;
        }
        else
        {
          prev = New->prev;
          next = New->next;
          prev->next = next;
          next->prev = prev;
        }
        delete (New);
        break;
      }
      if (New->next == NULL)
      {
        break;
      }
      else
      {
        New = New->next;
        _index++;
      }
    }
  }
  void ds_list_clear()
  {
    snake *del = head;
    snake *cop = new snake;
    while (1)
    {
      *cop = *del;
      delete (del);
      if (cop->next == NULL)
      {
        break;
      }
      else
      {
        del = cop->next;
      }
    }
    delete (cop);
    head = NULL;
    tail = NULL;
    temp = NULL;
  }
  int ds_list_info()
  {
    int index = 1;
    snake *New = head;
    while (1)
    {
      printf("number %d: %d \n", index, New->value);
      if (New->next == NULL)
      {
        break;
      }
      else
      {
        New = New->next;
        index++;
      }
    }
    return index;
  }
  int ds_list_find_index(int value)
  {
    int index = 1;
    snake *New = head;
    if (New == NULL)
    {
      return none;
    }
    while (1)
    {
      if (New->value == value)
      {
        break;
      }
      if (New->next == NULL)
      {
        index = none;
        break;
      }
      else
      {
        New = New->next;
        index++;
      }
    }
    return index;
  }
  int ds_list_find_value(int index)
  {
    int _index = 1;
    snake *New = head;
    while (1)
    {
      if (_index == index)
      {
        return New->value;
      }
      if (New->next == NULL)
      {
        break;
      }
      else
      {
        New = New->next;
        _index++;
      }
    }
    return none;
  }
  // 계속 업데이트.
  void ds_list_head_update()
  {
    snake *New = head;
    while (1)
    { // 실행.
      if (New->value == 0)
        New->head_update();

      if (New->next == NULL)
      {
        break;
      }
      else
      {
        New = New->next;
      }
    }
  }
  // 계속 업데이트.
  void ds_list_tail_update()
  {
    snake *New = tail;
    while (1)
    { // 실행.
      if (New->value == 1)
        New->tail_update(head);

      if (New->prev == NULL)
      {
        break;
      }
      else
      {
        New = New->prev;
      }
    }
  }
};
int main()
{
  goto_xy(x_max_size / 2, y_max_size / 2);
  printf("1: 게임 시작");
  goto_xy(x_max_size / 2, (y_max_size / 2) + 2);
  printf("2: 종료");
  // 게임 시작 또는 종료.
  switch (_getch())
  {
  case 50:
    goto_xy(x_max_size / 2, (y_max_size / 2) + 3);
    return 0;
  default:
    game_start = true;
  }
GAME:
  if (game_start == true)
  { // 벽 생성.
    system("cls");
    for (int m = 0; m < y_max_size; m++)
    {
      goto_xy(0, m);
      if ((m == 0) || (m == y_max_size - 1))
      {
        for (int n = 0; n < x_max_size; n += 2)
        {
          printf("□");
        }
      }
      else
      {
        printf("□");
        goto_xy(x_max_size - 2, m);
        printf("□");
      }
    }
    // 리스트 생성.
    ds_list_create index = ds_list_create();
    index.ds_list_add(0); // 플레이어 생성.
    index.head->x = (x_max_size / 2) + x_max_size / 4;
    index.head->y = y_max_size / 2;
    // 음식 생성.
    int food_num = 3; // 음식 최대 개수.
    food *eat;
    eat = new food[food_num];
    srand(time(NULL)); // 랜덤 초기화.
    while (game_start == true)
    {
      index.ds_list_head_update();
      index.ds_list_tail_update();
      // 음식 흩뿌리기.
      for (int i = 0; i < food_num; i++)
      {
        if (eat[i]._switch == false)
        {
          int _x = random_range(2, x_max_size - 4);
          int _y = random_range(2, y_max_size - 4);
          if (index.head->x != _x && index.head->y != _y)
          {
            eat[i].x = _x;
            eat[i].y = _y;
            eat[i]._switch = true;
          }
        }
        eat[i].update();
        // 좌표에 그리기.
        // 음식을 플레이어가 먹었는지 확인.
        int x_check = abs(index.head->x - eat[i].x);
        int y_check = abs(index.head->y - eat[i].y);
        if ((x_check < 2) && (y_check < 2))
        { // 먹었네.
          index.ds_list_add(1);
          goto_xy(eat[i].x, eat[i].y);
          printf(" ");
          eat[i]._switch = false;
        }
        // goto_xy(0, 0);
        // printf("X:%d Y:%d\nX:%d, Y:%d", index.head->x, index.head->y, index.tail->x, index.tail->y);
      }
    }
    // 게임 오버 처리.
    goto_xy(x_max_size / 4, y_max_size / 2);
    printf("게임 오버!!\n");
    for (int i = 3; i > 0; i--)
    {
      goto_xy(x_max_size / 4, (y_max_size / 2) + 2);
      printf("%d초 후 다시 시작합니다!!\n", i);
      Sleep(1000);
    }
    game_start = true;
    goto GAME;
  }
}