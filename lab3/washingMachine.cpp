#include <algorithm>
#include <iostream>

using namespace std;

struct F3 {
  double a, b, c;
  F3() : a(0), b(0), c(0) {}
};

F3 dirt(double d) {
  F3 m;
  if (d <= 25)
    m.a = 1;
  else if (d < 40)
    m.a = (40 - d) / 15;
  if (d > 30 && d <= 50)
    m.b = (d - 30) / 20;
  else if (d > 50 && d < 70)
    m.b = (70 - d) / 20;
  if (d > 60 && d < 75)
    m.c = (d - 60) / 15;
  else if (d >= 75)
    m.c = 1;
  return m;
}

F3 load(double l) {
  F3 m;
  if (l <= 2)
    m.a = 1;
  else if (l < 4)
    m.a = (4 - l) / 2;
  if (l > 3 && l <= 5)
    m.b = (l - 3) / 2;
  else if (l > 5 && l < 7)
    m.b = (7 - l) / 2;
  if (l > 6 && l < 8)
    m.c = (l - 6) / 2;
  else if (l >= 8)
    m.c = 1;
  return m;
}

F3 rules(F3 d, F3 l) {
  F3 w;
  w.a = max({min(d.a, l.a), min(d.a, l.b), min(d.b, l.a)});
  w.b = max({min(d.b, l.b), min(d.b, l.c), min(d.c, l.a), min(d.a, l.c)});
  w.c = max({min(d.c, l.b), min(d.c, l.c)});
  return w;
}

double defuzz(F3 w) {
  double total = w.a + w.b + w.c;
  return total ? (22.5 * w.a + 39 * w.b + 57.5 * w.c) / total : 30;
}

void test(int n, double d, double l) {
  double t = defuzz(rules(dirt(d), load(l)));
  cout << "\nCase " << n << ": Dirt=" << d << " Load=" << l
       << "kg => WashTime=" << t << "min ("
       << (t < 30   ? "Short"
           : t < 50 ? "Medium"
                    : "Long")
       << ")" << endl;
}

int main() {
  cout << "Fuzzy Washing Machine Controller\n";
  cout << "=================================" << endl;

  test(1, 20, 2);
  test(2, 50, 5);
  test(3, 85, 9);
  test(4, 25, 8);
  test(5, 80, 2.5);

  return 0;
} 
