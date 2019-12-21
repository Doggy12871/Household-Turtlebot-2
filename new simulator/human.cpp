//
// Created by 王泽钧 on 12/21/2019.
//

#include "human.h"
#include "utils.h"
#include "stdlib.h"

human::human(int x0, int y0) {
    // initial position
    this->x = x0;
    this->y = y0;
    last = 0;
    now = 5;
    goalX = x0;
    goalY = y0;
    stepNow = 0;
    // set initial destination
}

human::~human() {

}
//use *x and *y to return the next position. remember to update the inner stats
void human::get_next(int *x, int *y, const int game_map[][150]){
  if (this->x == goalX && this->y == goalY)
  {
    if (game_map[this->x][this->y] == 3)
    {
      *x = this->x;
      *y = this->y;
      return; // wait
    }
    else if (game_map[this->x][this->y] == 4) // when turtle catch up with human
    {
      last = now;
      now = judge_room(this->x, this->y);
      next_dst(&goalX, &goalY);
      path_search(game_map);
    }
  }
  if (stepNow >= 2)
  {
    stepNow -= 2;
    *x = next[stepNow].x;
    *y = next[stepNow].y;
  }
  else if (stepNow == 1)
  {
    stepNow--;
    *x = next[stepNow].x;
    *y = next[stepNow].y;
  }
  else
  {
    *x = next[stepNow].x;
    *y = next[stepNow].y;
  }
  return;
}

void human::path_search(const int maze[][150]) {
  int already[150][150] = { 0 };
  way ways[22600];
  way *head = ways, *tail = ways + 1;
  ways[0] = way(this->x, this->y, NULL);
  while (head != tail)
  {
    way pos = *head;
    if (pos.x == goalX && pos.y == goalY)
      break;
    already[pos.x][pos.y] = 1;
    if (pos.x < 100 && maze[pos.x + 1][pos.y] != 1 && already[pos.x + 1][pos.y] == 0)
    {
      *tail = way(pos.x + 1, pos.y, head);
      ++tail;
    }
    if (pos.y < 120 && maze[pos.x][pos.y + 1] != 1 && already[pos.x][pos.y + 1] == 0)
    {
      *tail = way(pos.x, pos.y + 1, head);
      ++tail;
    }
    if (pos.x > 0 && maze[pos.x - 1][pos.y] != 1 && already[pos.x - 1][pos.y] == 0)
    {
      *tail = way(pos.x - 1, pos.y, head);
      ++tail;
    }
    if (pos.y > 0 && maze[pos.x][pos.y - 1] != 1 && already[pos.x][pos.y - 1] == 0)
    {
      *tail = way(pos.x, pos.y - 1, head);
      ++tail;
    }
    ++head;
  }
  stepNow = 0;
  do
  {
    next[stepNow] = *head;
    ++stepNow;
    head = head->prev;
  } while (head != NULL);
  stepNow--;
}

//next_dst get a new destination out of the human action model
void human::next_dst(int *x, int *y) {
    double model[][][] = 
      [[[0.8857142857142857, 0.0, 0.11428571428571428, 0.0, 0.0, 0.0], [0, 0, 0, 0, 0, 0], 
      [0.08333333333333333, 0.16666666666666666, 0.08333333333333333, 0.3333333333333333, 0.3333333333333333, 0.0], 
      [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]], [[1.0, 0.0, 0.0, 0.0, 0.0, 0.0], 
      [0.028985507246376812, 0.6666666666666666, 0.11594202898550725, 0.08695652173913043, 0.028985507246376812, 0.07246376811594203], 
      [0.4444444444444444, 0.3333333333333333, 0.0, 0.1111111111111111, 0.1111111111111111, 0.0], 
      [0.0, 0.0, 0.0, 0.8333333333333334, 0.0, 0.16666666666666666], [0.0, 0.0, 0.0, 1.0, 0.0, 0.0], [0.0, 0.6, 0.0, 0.2, 0.2, 0.0]], 
      [[1.0, 0.0, 0.0, 0.0, 0.0, 0.0], [0.0, 1.0, 0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 1.0, 0.0, 0.0], 
      [0.0, 0.16666666666666666, 0.0, 0.6666666666666666, 0.0, 0.16666666666666666], [0.0, 0.0, 0.0, 0.8, 0.2, 0.0], 
      [0.0, 0.0, 0.0, 0.0, 0.0, 1.0]], [[1.0, 0.0, 0.0, 0.0, 0.0, 0.0], [0.0, 0.8, 0.1, 0.0, 0.1, 0.0], 
      [0.6666666666666666, 0.3333333333333333, 0.0, 0.0, 0.0, 0.0], 
      [0.0, 0.13043478260869565, 0.13043478260869565, 0.391304347826087, 0.21739130434782608, 0.13043478260869565], 
      [0.0, 0.0, 0.0, 1.0, 0.0, 0.0], [0.06666666666666667, 0.26666666666666666, 0.2, 0.4, 0.06666666666666667, 0.0]], 
      [[0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], 
      [0.06666666666666667, 0.26666666666666666, 0.0, 0.0, 0.0, 0.6666666666666666], [0.0, 0.0, 0.0, 0.5, 0.5, 0.0], 
      [0, 0, 0, 0, 0, 0]], [[1.0, 0.0, 0.0, 0.0, 0.0, 0.0], [0.0, 1.0, 0.0, 0.0, 0.0, 0.0], [0.25, 0.5, 0.0, 0.0, 0.0, 0.25], 
      [0.0, 0.2857142857142857, 0.0, 0.7142857142857143, 0.0, 0.0], [0.0, 0.0, 0.0, 1.0, 0.0, 0.0], [0.0, 0.0, 1.0, 0.0, 0.0, 0.0]]];
    srand((unsigned int)(time(NULL));
    int num = rand() % 10000;
    double prob = num / 10000.0;
    int dstRoom = 0;
    for (int i = 0; i < 6; ++i) // choose a certain room according to the model
    {
      dstRoom = i;
      if (prob <= model[last][now][i])
        break;
      prob -= model[last][now][i];
    }
    if (dstRoom > 5)
      dstRoom = 0;
    while (true)
    {
      int tmp_x, tmp_y;
      switch (dstRoom)
      {
      case 0:
        tmp_x = 1;
        tmp_y = 1;
        break;
      case 1:
        tmp_x = 1;
        tmp_y = 61;
        break;
      case 2:
        tmp_x = 41;
        tmp_y = 1;
        break
      case 3:
        tmp_x = 41;
        tmp_y = 21;
        break;
      case 4:
        tmp_x = 41;
        tmp_y = 101;
        break;
      default:
        tmp_x = 41;
        tmp_y = 101;
        break;
      }
      *x = tmp_x + rand() % 60;
      *y = tmp_y + rand() % 80;
      if (judge_room(*x, *y) - 1 == dstRoom)
        return;
    }
}