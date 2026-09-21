#include <stdio.h>

/*
    Task:
    Write a function `int is_prime(int n)` that returns 1 if n is prime,
    0 otherwise.

    In main():
      - Ask user for an integer n (>= 2)
      - If invalid, print an error
      - Otherwise, print all prime numbers up to n
*/

int is_prime(int n) {
  if (n < 2) return 0;

  // n sayısının asal olup olmadığını kareköküne kadar kontrol ediyoruz (i * i
  // <= n)
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      return 0;  // Asal değil
    }
  }
  return 1;  // Asal
}

int main(void) {
  int n;

  printf("Enter an integer n (>= 2): ");
  scanf("%d", &n);

  // Girdi doğrulama (n < 2 ise hata ver)
  if (n < 2) {
    printf("Hata: Girilen sayı 2'den büyük veya eşit olmalıdır!\n");
    return 1;
  }

  // 2'den n'e kadar olan tüm asal sayıları bul ve yazdır
  printf("2 ile %d arasındaki asal sayılar:\n", n);
  for (int i = 2; i <= n; i++) {
    if (is_prime(i)) {
      printf("%d ", i);
    }
  }
  printf("\n");

  return 0;
}