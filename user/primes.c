#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

/**
 * @brief 寻找素数
 * @param lpipe 左邻居管道
 */
void primes(int lpipe[2])
{
  close(lpipe[WR]);

  int first;
  if (read(lpipe[RD],  &first, INT_LEN) != INT_LEN) return;
  printf("prime %d\n", first);

  int p[2];
  pipe(p); // 当前的管道

  int data;
  // 从左管道读取数据
  while (read(lpipe[RD], &data, INT_LEN) == INT_LEN) {
    // 将无法整除的数据传递入右管道
    if (data % first)
      write(p[WR], &data, INT_LEN);
  }
  close(lpipe[RD]);
  close(p[WR]);

  if (fork() == 0) {
    primes(p);    // 递归的思想，但这将在一个新的进程中调用
  } else {
    close(p[RD]);
    wait(0);
  }
  exit(0);
}

int main(int argc, char const *argv[])
{
  int p[2];
  pipe(p);//创建管道

  for (int i = 2; i <= 35; ++i) //写入初始数据
    write(p[WR], &i, INT_LEN);

  if (fork() == 0) {
    primes(p);
  } else {
    close(p[WR]);
    close(p[RD]);
    wait(0);
  }

  exit(0);
}